#pragma once
#include "lue/framework/algorithm/policy.hpp"
#include "lue/framework/core/annotate.hpp"
#include "lue/framework/core/component.hpp"
#include "lue/framework/io/dataset.hpp"
#include "lue/framework/io/lue.hpp"
#include "lue/framework/io/util.hpp"
#include "lue/data_model/hl/util.hpp"
#include "lue/configure.hpp"


/*!
    @file

    Thiѕ header contains the implementation of the various to_lue function overloads.

    The goal is to return an array as soon as possible, allowing the caller to create more work, while
    the write-tasks start the writing.
*/


// Terminology:
// - A LUE dataset is represented by a HDF5 file
// - A LUE array is represented by a HDF5 dataset

// All I/O:
// - Writing to a dataset must happen after any previous calls to writing or reading to that same dataset
//   have finished. See usage of to_lue_order and from_lue_order.
// - No two processes can have the same dataset open for writing
// - HDF5 is threadsafe:
//     - Writing to an array can be done concurrently. Any serialization happens in the HDF5 library.
// - HDF5 is not threadsafe:
//     - Writing to an array must be done serialized.
// - Don't write while a read is still ongoing

// Serial I/O:
// - Notes:
//     - Don't write from multiple processes at the same time
//     - When the last read is done, we can assume that no reading is ongoing anymore (dataset is closed / not
//       locked)
//     - When a previous write is done, we can assume that no writing is ongoing anymore (dataset is closed /
//       not locked)
//     - In the case of serial I/O icw non-threadsafe HDF5, all HDF5 API calls must be done from the same OS
//       thread
//
// Root process:
//     - Group partitions by process
//     - Obtain to_lue_order to be able to serialize open / close calls to the same dataset in worker
//       processes
//     - Obtain from_lue_order to be able to wait on last from_lue call (if any) to the same dataset
//     - Asynchronously wait on previous to_lue_call and last from_lue call to finish
//     - Spawn a task per process to write from its partitions. Only spawn a new task once the previous write
//       has finished (the dataset is closed again)
//
// Worker process:
//     - Obtain a promise to be able to signal any successors once we are done opening the file
//     - Obtain a future representing a previous to_lue open dataset call done by the same process
//     - Obtain a future representing a previous from_lue close dataset call (if any) done by the same process
//     - Asynchronously wait on previous from_lue close dataset call, previous to_lue open dataset call, and
//       one of the partitions to become ready
//     - Open dataset for writing
//     - Set value of open dataset promise
//     - Asynchronously write each partition individually to the dataset, once it becomes ready and the task
//       gets scheduled.
//     - Asynchronously wait for all partitions to be written
//     - Obtain a promise to be able to signal any successors once we are done closing the file
//     - Obtain a future representing a previous to_lue close dataset call done by the same process
//     - Asynchronously wait on previous to_lue close dataset call
//     - Close dataset
//     - Set value of close dataset promise
//
// - Opening and closing a LUE dataset (HDF5 file) are collective operations. They need to happen in the same
//   order in all processes. See usage of to_lue_order.
// - Writing to a LUE array (HDF5 dataset) can be done independently


/*
Root process:
- Wait for all previous from_lue / to_lue calls using the same file have finished
- Parallel I/O:
    - Tell each process to write its partitions
- Serial I/O:
    - Tell each process in turn to write its partitions
- Return a future which becomes ready once all processes have finished writing their partitions and the
  dataset is closed again

Worker process:
- Wait for all partitions to write have become ready
- Launch a task which writes all partitions serially

- [ ] Refactor the local count stuff with the global count stuff
- [ ] Refactor the constant / variable code as much as possible
- [ ] Can we used barriers instead of our own synchronization stuff?
- [ ] Try to keep the size of counting stuff datastructures within bounds
*/


namespace lue {
    namespace detail {

        template<typename Policies, typename Partitions, typename CreateHyperslab>
        void write_partitions(
            [[maybe_unused]] Policies const& policies,
            Partitions const& partitions,
            CreateHyperslab create_hyperslab,
            data_model::Array& array)
        {
            // Synchronously write all partitions, from the same OS thread
            lue_hpx_assert(std::all_of(
                partitions.begin(),
                partitions.end(),
                [](auto const& partition) -> auto { return partition.is_ready(); }));

            AnnotateFunction const annotate{"to_lue: partitions"};

            // Open value. Configure for use of parallel I/O if relevant.
            hdf5::Dataset::TransferPropertyList transfer_property_list{};

#ifdef LUE_FRAMEWORK_WITH_PARALLEL_IO
            transfer_property_list.set_transfer_mode(H5FD_MPIO_INDEPENDENT);
#endif

            using Partition = typename Partitions::value_type;
            using Element = ElementT<Partition>;

            for (auto& partition : partitions)
            {
                // write_partition(policies, partition, create_hyperslab, array);

                auto partition_ptr{detail::ready_component_ptr(partition)};
                auto& partition_server{*partition_ptr};
                Element* buffer{partition_server.data().data()};

                array.write(create_hyperslab(partition_server), transfer_property_list, buffer);
            }
        }


        template<typename Policies, typename Partitions>
        auto write_partitions_constant(
            Policies const& policies,
            hdf5::Offset const& array_hyperslab_start,  // Only needed to offset block written to array
            Partitions const& partitions,
            std::string const& array_pathname,
            Count const to_lue_order,
            [[maybe_unused]] Count const from_lue_order,
            data_model::ID const object_id) -> hpx::future<void>
        {
            print_debug("to_lue/open {} (start trying)", to_lue_order);

            using Partition = typename Partitions::value_type;
            using PartitionServer = Partition::Server;

            auto const [dataset_pathname, phenomenon_name, property_set_name, property_name] =
                parse_array_pathname(array_pathname);

            std::filesystem::path const dataset_path{normalize(dataset_pathname)};

            hpx::promise<void> to_lue_open_dataset_p =
                worker::to_lue_open_dataset_promise_for(dataset_path, to_lue_order);
            hpx::promise<void> to_lue_close_dataset_p =
                worker::to_lue_close_dataset_promise_for(dataset_path, to_lue_order);

#ifndef NDEBUG
            // This must be true as the root locality waits for a previous read to have finished
            lue_hpx_assert(
                from_lue_order == 0 ||
                worker::from_lue_close_dataset_done_available(dataset_path, from_lue_order));
            hpx::shared_future<void> from_lue_close_dataset_done_f =
                worker::from_lue_close_dataset_done(dataset_path, from_lue_order);
            lue_hpx_assert(from_lue_close_dataset_done_f.is_ready());

            // This must be true as the root locality waits for a previous write to have finished
            lue_hpx_assert(
                to_lue_order == 1 ||
                worker::to_lue_close_dataset_done_available(dataset_path, to_lue_order - 1));
            hpx::shared_future<void> to_lue_close_dataset_predecessor_done_f =
                worker::to_lue_close_dataset_done(dataset_path, to_lue_order - 1);
            lue_hpx_assert(to_lue_close_dataset_predecessor_done_f.is_ready());
#endif

            // Wait for all partitions to have become ready before opening the dataset. In the serial /
            // non-threadsafe context it is important that all HDF5 API calls are done from the same OS
            // thread. We therefore make sure we don't have to wait anywhere else (and potentially get
            // rescheduled on another OS thread) during the write. In other contexts it is a fine approach as
            // well. It doesn't help to write in parallel from within a process.

            return hpx::dataflow(
                hpx::launch::async,
                [policies,
                 array_hyperslab_start,
                 dataset_path,
                 to_lue_order,
                 to_lue_open_dataset_p = std::move(to_lue_open_dataset_p),
                 to_lue_close_dataset_p = std::move(to_lue_close_dataset_p),
                 phenomenon_name,
                 property_set_name,
                 property_name,
                 object_id](auto&& partitions_f) mutable -> void
                {
                    AnnotateFunction const annotate{"to_lue: partitions constant"};

                    print_debug("to_lue/open {}", to_lue_order);

                    auto dataset = open_dataset(dataset_path.string(), H5F_ACC_RDWR);

                    {
                        // Find the array to write to (the HDF5 dataset)
                        auto& phenomenon{dataset.phenomena()[phenomenon_name]};
                        auto& property_set{phenomenon.property_sets()[property_set_name]};
                        lue_hpx_assert(property_set.properties().contains(property_name));
                        lue_hpx_assert(
                            property_set.properties().shape_per_object(property_name) ==
                            data_model::ShapePerObject::different);
                        lue_hpx_assert(
                            property_set.properties().value_variability(property_name) ==
                            data_model::ValueVariability::constant);
                        using Properties = data_model::different_shape::Properties;
                        // data_model::different_shape::Property
                        auto& property{property_set.properties().collection<Properties>()[property_name]};
                        // constant: data_model::different_shape::Value
                        auto const& value{property.value()};
                        // constant: data_model::Array: hdf5::Dataset
                        auto array{value[object_id]};

                        // Done with the collective calls. Note that H5DOpen is collective in case of write.
                        // Writing partitions can happen independently now.
                        to_lue_open_dataset_p.set_value();

                        lue_hpx_assert(
                            worker::to_lue_open_dataset_done(dataset_path, to_lue_order).is_ready());

                        auto create_hyperslab =
                            [array_hyperslab_start](PartitionServer const& partition_server) -> auto
                        { return hyperslab(array_hyperslab_start, partition_server); };

                        // Synchronous
                        write_partitions(policies, partitions_f.get(), create_hyperslab, array);
                    }

                    lue_hpx_assert(
                        worker::to_lue_close_dataset_when_predecessor_done(dataset_path, to_lue_order)
                            .is_ready());

                    // The dataset must go out of scope before we set the promise's value
                    [](auto&& dataset) -> auto { HPX_UNUSED(dataset); }(std::move(dataset));

                    // Closing a dataset is a collective operation: only close the dataset (let it go out
                    // of scope) when it is our turn to do so.
                    to_lue_close_dataset_p.set_value();

                    lue_hpx_assert(worker::to_lue_close_dataset_done(dataset_path, to_lue_order).is_ready());

                    print_debug("to_lue/close {}", to_lue_order);
                },
                hpx::when_all(partitions));
        }


        /*!
            @brief      .
            @tparam     .
            @param      .
            @return     .
            @exception  .

            The future returned becomes ready once all partitions have been written and the dataset is closed
            again.
        */
        template<typename Policies, typename Partitions>
        auto write_partitions_variable(
            Policies const& policies,
            hdf5::Offset const& array_hyperslab_start,  // Only needed to offset block written to array
            Partitions const& partitions,
            std::string const& array_pathname,
            Count const to_lue_order,
            [[maybe_unused]] Count const from_lue_order,
            data_model::ID const object_id,
            Index const time_step_idx) -> hpx::future<void>
        {
            print_debug("to_lue/open {} (start trying)", to_lue_order);

            using Partition = typename Partitions::value_type;
            using PartitionServer = Partition::Server;

            auto const [dataset_pathname, phenomenon_name, property_set_name, property_name] =
                parse_array_pathname(array_pathname);

            std::filesystem::path const dataset_path{normalize(dataset_pathname)};

            hpx::promise<void> to_lue_open_dataset_p =
                worker::to_lue_open_dataset_promise_for(dataset_path, to_lue_order);
            hpx::promise<void> to_lue_close_dataset_p =
                worker::to_lue_close_dataset_promise_for(dataset_path, to_lue_order);

#ifndef NDEBUG
            // This must be true as the root locality waits for a previous read to have finished
            lue_hpx_assert(
                from_lue_order == 0 ||
                worker::from_lue_close_dataset_done_available(dataset_path, from_lue_order));
            hpx::shared_future<void> from_lue_close_dataset_done_f =
                worker::from_lue_close_dataset_done(dataset_path, from_lue_order);
            lue_hpx_assert(from_lue_close_dataset_done_f.is_ready());

            // This must be true as the root locality waits for a previous write to have finished
            lue_hpx_assert(
                to_lue_order == 1 ||
                worker::to_lue_close_dataset_done_available(dataset_path, to_lue_order - 1));
            hpx::shared_future<void> to_lue_close_dataset_predecessor_done_f =
                worker::to_lue_close_dataset_done(dataset_path, to_lue_order - 1);
            lue_hpx_assert(to_lue_close_dataset_predecessor_done_f.is_ready());
#endif

            // Wait for all partitions to have become ready before opening the dataset. In the serial /
            // non-threadsafe context it is important that all HDF5 API calls are done from the same OS
            // thread. We therefore make sure we don't have to wait anywhere else (and potentially get
            // rescheduled on another OS thread) during the write. In other contexts it is a fine approach as
            // well. It doesn't help to write in parallel from within a process.

            return hpx::dataflow(
                hpx::launch::async,
                [policies,
                 array_hyperslab_start,
                 dataset_path,
                 to_lue_order,
                 to_lue_open_dataset_p = std::move(to_lue_open_dataset_p),
                 to_lue_close_dataset_p = std::move(to_lue_close_dataset_p),
                 phenomenon_name,
                 property_set_name,
                 property_name,
                 object_id,
                 time_step_idx](auto&& partitions_f) mutable -> void
                {
                    AnnotateFunction const annotate{"to_lue: partitions variable"};

                    print_debug("to_lue/open {}", to_lue_order);

                    auto dataset = open_dataset(dataset_path.string(), H5F_ACC_RDWR);

                    {
                        // Find the array to write to (the HDF5 dataset)
                        auto& phenomenon{dataset.phenomena()[phenomenon_name]};
                        auto& property_set{phenomenon.property_sets()[property_set_name]};
                        lue_hpx_assert(property_set.properties().contains(property_name));
                        lue_hpx_assert(
                            property_set.properties().shape_per_object(property_name) ==
                            data_model::ShapePerObject::different);
                        lue_hpx_assert(
                            property_set.properties().value_variability(property_name) ==
                            data_model::ValueVariability::variable);
                        lue_hpx_assert(
                            property_set.properties().shape_variability(property_name) ==
                            data_model::ShapeVariability::constant);
                        using Properties = data_model::different_shape::constant_shape::Properties;
                        // data_model::different_shape::constant_shape::Property
                        auto const& property{
                            property_set.properties().collection<Properties>()[property_name]};
                        // variable: data_model::different_shape::constant_shape::Value
                        auto const& value{property.value()};
                        // variable: data_model::same_shape::constant_shape::Value: data_model::Array:
                        // hdf5::Dataset
                        auto array{value[object_id]};

                        // Done with the collective calls. Note that H5DOpen is collective in case of write.
                        // Writing partitions can happen independently now.
                        to_lue_open_dataset_p.set_value();

                        lue_hpx_assert(
                            worker::to_lue_open_dataset_done(dataset_path, to_lue_order).is_ready());

                        auto create_hyperslab = [array_hyperslab_start, time_step_idx](
                                                    PartitionServer const& partition_server) -> auto
                        { return hyperslab(array_hyperslab_start, partition_server, 0, time_step_idx); };

                        // Synchronous
                        write_partitions(policies, partitions_f.get(), create_hyperslab, array);
                    }

                    lue_hpx_assert(
                        worker::to_lue_close_dataset_when_predecessor_done(dataset_path, to_lue_order)
                            .is_ready());

                    // The dataset must go out of scope before we set the promise's value
                    [](auto&& dataset) -> auto { HPX_UNUSED(dataset); }(std::move(dataset));

                    // Closing a dataset is a collective operation: only close the dataset (let it go out
                    // of scope) when it is our turn to do so.
                    to_lue_close_dataset_p.set_value();

                    lue_hpx_assert(worker::to_lue_close_dataset_done(dataset_path, to_lue_order).is_ready());

                    print_debug("to_lue/close {}", to_lue_order);
                },
                hpx::when_all(partitions));
        }


        template<typename Policies, typename Partitions>
        struct WritePartitionsConstantAction:
            hpx::actions::make_action<
                decltype(&write_partitions_constant<Policies, Partitions>),
                &write_partitions_constant<Policies, Partitions>,
                WritePartitionsConstantAction<Policies, Partitions>>::type
        {
        };


        template<typename Policies, typename Partitions>
        struct WritePartitionsVariableAction:
            hpx::actions::make_action<
                decltype(&write_partitions_variable<Policies, Partitions>),
                &write_partitions_variable<Policies, Partitions>,
                WritePartitionsVariableAction<Policies, Partitions>>::type
        {
        };


        template<typename Policies, Rank rank>
        auto to_lue(
            Policies const& policies,
            PartitionedArray<policy::InputElementT<Policies>, rank> const& array,
            std::string const& array_pathname,
            data_model::ID const object_id) -> hpx::future<void>
        {
            AnnotateFunction const annotate{"to_lue"};

            using Element = policy::InputElementT<Policies>;
            using Array = PartitionedArray<Element, rank>;
            using Partition = PartitionT<Array>;
            using Action = detail::WritePartitionsConstantAction<Policies, std::vector<Partition>>;

            auto const [dataset_pathname, phenomenon_name, property_set_name, property_name] =
                parse_array_pathname(array_pathname);
            auto const dataset_path{normalize(dataset_pathname)};

            // Management of dependencies between calls
            auto const to_lue_order = root::to_lue_order(dataset_path);
            auto const from_lue_order = root::current_from_lue_order(dataset_path);

            auto precondition_f = hpx::when_all(
                root::to_lue_finished(dataset_path, to_lue_order - 1),
                root::from_lue_finished(dataset_path, from_lue_order));

            // Partitions and localities
            auto const partition_idxs_by_locality{detail::partition_idxs_by_locality(array)};

            hpx::future<void> to_lue_finished_f = precondition_f.then(
                [policies,
                 array_shape = array.shape(),
                 array_partitions = array.partitions(),
                 partition_idxs_by_locality = std::move(partition_idxs_by_locality),
                 array_pathname,
                 dataset_path,
                 to_lue_order,
                 from_lue_order,
                 object_id]([[maybe_unused]] auto const& precondition_f) mutable -> hpx::future<void>
                {
                    std::vector<hpx::future<void>> localities_finished{};

#ifndef LUE_FRAMEWORK_WITH_PARALLEL_IO
                    localities_finished.reserve(partition_idxs_by_locality.size() + 1);
                    localities_finished.push_back(hpx::make_ready_future());
#else
                    localities_finished.reserve(partition_idxs_by_locality.size());
#endif

                    Action action{};

                    for (auto const& [locality, partition_idxs] : partition_idxs_by_locality)
                    {
                        // Copy current selection of partitions from input array to a new collection
                        std::vector<Partition> locality_partitions(partition_idxs.size());

                        for (std::size_t idx = 0; auto const partition_idx : partition_idxs)
                        {
                            lue_hpx_assert(!locality_partitions[idx].valid());
                            lue_hpx_assert(array_partitions[partition_idx].valid());

                            locality_partitions[idx++] = array_partitions[partition_idx];

                            lue_hpx_assert(locality_partitions[idx - 1]);
                            lue_hpx_assert(array_partitions[partition_idx].valid());
                        }

#ifndef LUE_FRAMEWORK_WITH_PARALLEL_IO
                        // Writing gets serialized. Localities get to write their partitions in turn.
                        // Don't allow a process to open the dataset before the previous process has closed it
                        // again
                        auto& precondition_f = localities_finished.back();
#else
                        // Writing by all processes only depends on the dataset being closed by any previous
                        // read or write. This has happened already (see precondition).
                        auto precondition_f = hpx::make_ready_future();
#endif

                        lue_hpx_assert(localities_finished.size() < localities_finished.capacity());

                        // Spawn a task that writes the partitions in the current process to the dataset. This
                        // returns a future which becomes ready once these partitions have been written and
                        // the dataset has been closed again.
                        localities_finished.push_back(precondition_f.then(
                            [locality,
                             action,
                             policies,
                             array_hyperslab = shape_to_hyperslab(array_shape),
                             partitions = std::move(locality_partitions),
                             array_pathname,
                             to_lue_order,
                             from_lue_order,
                             object_id]([[maybe_unused]] auto const& precondition_f) -> hpx::future<void>
                            {
                                return hpx::async(
                                    action,
                                    locality,
                                    std::move(policies),
                                    array_hyperslab.start(),
                                    std::move(partitions),
                                    array_pathname,
                                    to_lue_order,
                                    from_lue_order,
                                    object_id);
                            }));
                    }

#ifndef LUE_FRAMEWORK_WITH_PARALLEL_IO
                    // When the last process has finished, all processes have finished
                    hpx::future<void>& to_lue_finished_f = localities_finished.back();
#else
                    hpx::future<void> to_lue_finished_f =
                        hpx::when_all(localities_finished.begin(), localities_finished.end());
#endif

                    lue_hpx_assert(to_lue_finished_f.valid());

                    return std::move(to_lue_finished_f);
                });

            root::add_to_lue_finished(dataset_path, to_lue_order, std::move(to_lue_finished_f));

            return root::to_lue_finished(dataset_path, to_lue_order)
                .then([]([[maybe_unused]] hpx::shared_future<void> const& to_lue_finished_f) -> void {});
        }


        template<typename Policies, Rank rank>
        auto to_lue(
            Policies const& policies,
            PartitionedArray<policy::InputElementT<Policies>, rank> const& array,
            std::string const& array_pathname,
            data_model::ID const object_id,
            Index const time_step_idx) -> hpx::future<void>
        {
            AnnotateFunction const annotate{"to_lue"};

            using Element = policy::InputElementT<Policies>;
            using Array = PartitionedArray<Element, rank>;
            using Partition = PartitionT<Array>;
            using Action = WritePartitionsVariableAction<Policies, std::vector<Partition>>;

            auto const [dataset_pathname, phenomenon_name, property_set_name, property_name] =
                parse_array_pathname(array_pathname);
            auto const dataset_path{normalize(dataset_pathname)};

            // Management of dependencies between calls
            auto const to_lue_order = root::to_lue_order(dataset_path);
            auto const from_lue_order = root::current_from_lue_order(dataset_path);

            auto precondition_f = hpx::when_all(
                root::to_lue_finished(dataset_path, to_lue_order - 1),
                root::from_lue_finished(dataset_path, from_lue_order));

            // Partitions and localities
            auto const partition_idxs_by_locality{detail::partition_idxs_by_locality(array)};

            hpx::future<void> to_lue_finished_f = precondition_f.then(
                [policies,
                 array_shape = array.shape(),
                 array_partitions = array.partitions(),
                 partition_idxs_by_locality = std::move(partition_idxs_by_locality),
                 array_pathname,
                 dataset_path,
                 to_lue_order,
                 from_lue_order,
                 object_id,
                 time_step_idx]([[maybe_unused]] auto const& precondition_f) mutable -> hpx::future<void>
                {
                    std::vector<hpx::future<void>> localities_finished{};

#ifndef LUE_FRAMEWORK_WITH_PARALLEL_IO
                    localities_finished.reserve(partition_idxs_by_locality.size() + 1);
                    localities_finished.push_back(hpx::make_ready_future());
#else
                    localities_finished.reserve(partition_idxs_by_locality.size());
#endif

                    Action action{};

                    for (auto const& [locality, partition_idxs] : partition_idxs_by_locality)
                    {
                        // Copy current selection of partitions from input array to a new collection
                        std::vector<Partition> locality_partitions(partition_idxs.size());

                        for (std::size_t idx = 0; auto const partition_idx : partition_idxs)
                        {
                            lue_hpx_assert(!locality_partitions[idx].valid());
                            lue_hpx_assert(array_partitions[partition_idx].valid());

                            locality_partitions[idx++] = array_partitions[partition_idx];

                            lue_hpx_assert(locality_partitions[idx - 1]);
                            lue_hpx_assert(array_partitions[partition_idx].valid());
                        }

#ifndef LUE_FRAMEWORK_WITH_PARALLEL_IO
                        // Writing gets serialized. Localities get to write their partitions in turn.
                        // Don't allow a process to open the dataset before the previous process has closed it
                        // again
                        auto& precondition_f = localities_finished.back();
#else
                        // Writing by all processes only depends on the dataset being closed by any previous
                        // read or write. This has happened already (see precondition).
                        auto precondition_f = hpx::make_ready_future();
#endif

                        lue_hpx_assert(localities_finished.size() < localities_finished.capacity());

                        // Spawn a task that writes the partitions in the current process to the dataset. This
                        // returns a future which becomes ready once these partitions have been written and
                        // the dataset has been closed again.
                        localities_finished.push_back(precondition_f.then(
                            [locality,
                             action,
                             policies,
                             array_hyperslab = shape_to_hyperslab(array_shape),
                             partitions = std::move(locality_partitions),
                             array_pathname,
                             to_lue_order,
                             from_lue_order,
                             object_id,
                             time_step_idx]([[maybe_unused]] auto const& precondition_f) -> hpx::future<void>
                            {
                                return hpx::async(
                                    action,
                                    locality,
                                    std::move(policies),
                                    array_hyperslab.start(),
                                    std::move(partitions),
                                    array_pathname,
                                    to_lue_order,
                                    from_lue_order,
                                    object_id,
                                    time_step_idx);
                            }));
                    }

#ifndef LUE_FRAMEWORK_WITH_PARALLEL_IO
                    // When the last process has finished, all processes have finished
                    hpx::future<void>& to_lue_finished_f = localities_finished.back();
#else
                    hpx::future<void> to_lue_finished_f =
                        hpx::when_all(localities_finished.begin(), localities_finished.end());
#endif

                    lue_hpx_assert(to_lue_finished_f.valid());

                    return std::move(to_lue_finished_f);
                });

            root::add_to_lue_finished(dataset_path, to_lue_order, std::move(to_lue_finished_f));

            return root::to_lue_finished(dataset_path, to_lue_order)
                .then([]([[maybe_unused]] hpx::shared_future<void> const& to_lue_finished_f) -> void {});
        }

    }  // namespace detail


    namespace policy::to_lue {

        template<typename InputElement>
        using DefaultPolicies =
            policy::DefaultPolicies<AllValuesWithinDomain<>, OutputElements<>, InputElements<InputElement>>;

        template<typename InputElement>
        using DefaultValuePolicies = policy::
            DefaultValuePolicies<AllValuesWithinDomain<>, OutputElements<>, InputElements<InputElement>>;

    }  // namespace policy::to_lue


    /*!
        @brief      Write an array to an array in a LUE dataset
        @tparam     Policies Policies type
        @tparam     Rank Rank of the array
        @param      policies Policies to use
        @param      array Array to write
        @param      array_pathname Pathname of the property to write to, formatted as
                    `<dataset_pathname>/<phenomenon_name>/<property_set_name>/<property_name>`
        @param      object_id ID of object whose property value to write
        @return     A future which becomes ready once the writing is done
    */
    template<typename Policies, Rank rank>
    auto to_lue(
        Policies const& policies,
        PartitionedArray<policy::InputElementT<Policies>, rank> const& array,
        std::string const& array_pathname,
        data_model::ID const object_id) -> hpx::future<void>
    {
        return detail::to_lue(policies, array, array_pathname, object_id);
    }


    /*!
        @overload

        Default policies will be used.
    */
    template<typename Element, Rank rank>
    auto to_lue(
        PartitionedArray<Element, rank> const& array,
        std::string const& array_pathname,
        data_model::ID const object_id) -> hpx::future<void>
    {
        using Policies = policy::to_lue::DefaultPolicies<Element>;

        return to_lue(Policies{}, array, array_pathname, object_id);
    }


    /*!
        @brief      Write an array to an array in a LUE dataset
        @tparam     Policies Policies type
        @tparam     Rank Rank of the array
        @param      policies Policies to use
        @param      array Array to write
        @param      array_pathname Pathname of the property to write to, formatted as
                    `<dataset_pathname>/<phenomenon_name>/<property_set_name>/<property_name>`
        @param      object_id ID of object whose property value to write
        @param      time_step_idx Index of time step to write
        @return     A future which becomes ready once the writing is done and the dataset is closed again
    */
    template<typename Policies, Rank rank>
    auto to_lue(
        Policies const& policies,
        PartitionedArray<policy::InputElementT<Policies>, rank> const& array,
        std::string const& array_pathname,
        data_model::ID const object_id,
        Index const time_step_idx) -> hpx::future<void>
    {
        return detail::to_lue(policies, array, array_pathname, object_id, time_step_idx);
    }


    /*!
        @overload

        Default policies will be used.
    */
    template<typename Element, Rank rank>
    auto to_lue(
        PartitionedArray<Element, rank> const& array,
        std::string const& array_pathname,
        data_model::ID const object_id,
        Index const time_step_idx) -> hpx::future<void>
    {
        using Policies = policy::to_lue::DefaultPolicies<Element>;

        return to_lue(Policies{}, array, array_pathname, object_id, time_step_idx);
    }

}  // namespace lue
