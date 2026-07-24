#include "lue/framework/io/lue.hpp"
#include "lue/framework/core/define.hpp"
#include "lue/framework/core/locality.hpp"
#include "lue/framework/core/serialize_resource_usage.hpp"
#include "lue/framework/io/serializer.hpp"
#include <hpx/runtime.hpp>


namespace lue::detail {

    auto normalize(std::string const& pathname) -> std::filesystem::path
    {
        return std::filesystem::canonical(pathname);
    }


    namespace root {
        namespace {

            using CountByPath = ResourceUseCountByKey<std::filesystem::path>;


            auto from_lue_count_by_dataset_path() -> CountByPath&
            {
                static CountByPath count_by_path{};

                return count_by_path;
            }


            auto to_lue_count_by_dataset_path() -> CountByPath&
            {
                static CountByPath count_by_path{};

                return count_by_path;
            }

        }  // Anonymous namespace


        /*!
            @brief      Return the number of times the dataset pointed to by @a path is opened for reading
            @sa         count_by_path()

            This count can be used to order / serialize open_dataset calls. The count is incremented each time
            this function is called.
        */
        auto from_lue_order(std::filesystem::path const& path) -> Count
        {
            return resource_use_count_by(from_lue_count_by_dataset_path(), path);
        }


        /*!
            @brief      Return the number of times the dataset pointed to by @a path is opened for writing
            @sa         count_by_path()

            This count can be used to order / serialize open_dataset calls. The count is incremented each time
            this function is called.
        */
        auto to_lue_order(std::filesystem::path const& path) -> Count
        {
            return resource_use_count_by(to_lue_count_by_dataset_path(), path);
        }


        /*!
            @brief      Return the number of times the dataset pointed to by @a path has been opened for
                        reading

            This count can be used to order / serialize open_dataset calls
        */
        auto current_from_lue_order(std::filesystem::path const& dataset_path) -> Count
        {
            return current_resource_use_count(from_lue_count_by_dataset_path(), dataset_path);
        }


        /*!
            @brief      Return the number of times the dataset pointed to by @a path has been opened for
                        writing

            This count can be used to order / serialize open_dataset calls
        */
        auto current_to_lue_order(std::filesystem::path const& dataset_path) -> Count
        {
            return current_resource_use_count(to_lue_count_by_dataset_path(), dataset_path);
        }


        namespace {

            // TODO: Add logic to remove information again. When adding a future, previous futures can be
            //       removed, right?

            using CallFinished = ResourceUseFinished<std::filesystem::path>;


            auto to_lue_finished() -> CallFinished&
            {
                static CallFinished to_lue_finished{};

                return to_lue_finished;
            }


            auto from_lue_finished() -> CallFinished&
            {
                static CallFinished from_lue_finished;

                return from_lue_finished;
            }

        }  // Anonymous namespace


        /*!
            @brief      Add a @ future which becomes ready once call @a count to to_lue to @a path
                        finishes

            This function must be called from the root locality, on the main thread.
        */
        void add_to_lue_finished(
            std::filesystem::path const& path, Count const count, hpx::future<void> future)
        {
            add_resource_use_finished(to_lue_finished(), path, count, std::move(future));
        }


        /*!
            @brief      Add a @ future which becomes ready once call @a count to from_lue from @a path
                        finishes

            This function must be called from the root locality, on the main thread.
        */
        void add_from_lue_finished(
            std::filesystem::path const& path, Count const count, hpx::future<void> future)
        {
            add_resource_use_finished(from_lue_finished(), path, count, std::move(future));
        }


        /*!
            @brief      Return a future which becomes ready once call @a count to to_lue finishes

            This function must be called from the root locality, on the main thread.
        */
        auto to_lue_finished(std::filesystem::path const& path, Count const count) -> hpx::shared_future<void>
        {
            return resource_use_finished(to_lue_finished(), path, count);
        }


        /*!
            @brief      Return a future which becomes ready once call @a count to from_lue from @a path
                        finishes

            This function must be called from the root locality, on the main thread.
        */
        auto from_lue_finished(std::filesystem::path const& path, Count const count)
            -> hpx::shared_future<void>
        {
            return resource_use_finished(from_lue_finished(), path, count);
        }

    }  // namespace root


    namespace worker {


        /*!
            @brief      Type for serializing accesses to files

            This type is intended to be used on the worker locality only.
        */
        using FileSerializer = Serializer<std::filesystem::path, Count>;


        namespace {

            auto to_lue_open_dataset_serializer() -> FileSerializer&
            {
                static Serializer<std::filesystem::path, Count> serializer{};

                return serializer;
            }


            auto to_lue_close_dataset_serializer() -> FileSerializer&
            {
                static Serializer<std::filesystem::path, Count> serializer{};

                return serializer;
            }


            auto from_lue_open_dataset_serializer() -> FileSerializer&
            {
                static Serializer<std::filesystem::path, Count> serializer{};

                return serializer;
            }


            auto from_lue_close_dataset_serializer() -> FileSerializer&
            {
                static Serializer<std::filesystem::path, Count> serializer{};

                return serializer;
            }

        }  // Anonymous namespace


        /*!
            @brief      Return a promise to set once to_lue has opened dataset @a path for call @a open_count

            This function must be called from a worker locality.
        */
        auto to_lue_open_dataset_promise_for(std::filesystem::path const& path, Count const open_count)
            -> hpx::promise<void>
        {
            return to_lue_open_dataset_serializer().promise_for(path, open_count);
        }


        /*!
            @brief      Return a promise to set once to_lue has closed dataset @a path for call @a open_count

            This function must be called from a worker locality.
        */
        auto to_lue_close_dataset_promise_for(std::filesystem::path const& path, Count const open_count)
            -> hpx::promise<void>
        {
            return to_lue_close_dataset_serializer().promise_for(path, open_count);
        }


        /*!
            @brief      Return a promise to set once from_lue has opened dataset @a path for call
                        @a open_count

            This function must be called from a worker locality.
        */
        auto from_lue_open_dataset_promise_for(std::filesystem::path const& path, Count const open_count)
            -> hpx::promise<void>
        {
            return from_lue_open_dataset_serializer().promise_for(path, open_count);
        }


        /*!
            @brief      Return a promise to set once from_lue has opened dataset @a path for call
                        @a open_count

            This function must be called from a worker locality.
        */
        auto from_lue_close_dataset_promise_for(std::filesystem::path const& path, Count const open_count)
            -> hpx::promise<void>
        {
            return from_lue_close_dataset_serializer().promise_for(path, open_count);
        }


#ifndef NDEBUG

        /*!
            @brief      Return a future which will become ready once to_lue has closed dataset @a path for
                        call @a open_count - 1

            This function must be called from a worker locality.
        */
        auto to_lue_close_dataset_when_predecessor_done(
            std::filesystem::path const& path, Count const open_count) -> hpx::shared_future<void>
        {
            return to_lue_close_dataset_serializer().when_predecessor_done(path, open_count);
        }


        /*!
            @brief      Return a future which will become ready once from_lue has closed dataset @a path for
                        call @a open_count - 1

            This function must be called from a worker locality.
        */
        auto from_lue_close_dataset_when_predecessor_done(
            std::filesystem::path const& path, Count const open_count) -> hpx::shared_future<void>
        {
            return from_lue_close_dataset_serializer().when_predecessor_done(path, open_count);
        }


        /*!
            @brief      Return a future which will become ready once from_lue has closed dataset @a path for
                        call @a count

            This function must be called from a worker locality.
        */
        auto from_lue_open_dataset_done(std::filesystem::path const& path, Count const count)
            -> hpx::shared_future<void>
        {
            return count > 0 ? from_lue_open_dataset_serializer().when_done(path, count)
                             : hpx::make_ready_future().share();
        }


        /*!
            @brief      Return a future which will become ready once to_lue has closed dataset @a path for
           call
                        @a count

            This function must be called from a worker locality.
        */
        auto to_lue_open_dataset_done(std::filesystem::path const& path, Count const count)
            -> hpx::shared_future<void>
        {
            return count > 0 ? to_lue_open_dataset_serializer().when_done(path, count)
                             : hpx::make_ready_future().share();
        }


        /*!
            @brief      Return whether a future is available which will become ready once from_lue has closed
                        dataset @a path for call @a count

            This function must be called from a worker locality.
        */
        auto from_lue_close_dataset_done_available(std::filesystem::path const& path, Count count) -> bool
        {
            lue_hpx_assert(count > 0);

            return from_lue_close_dataset_serializer().contains(path, count);
        }


        /*!
            @brief      Return whether a future is available which will become ready once to_lue has closed
                        dataset @a path for call @a count

            This function must be called from a worker locality.
        */
        auto to_lue_close_dataset_done_available(std::filesystem::path const& path, Count count) -> bool
        {
            lue_hpx_assert(count > 0);

            return to_lue_close_dataset_serializer().contains(path, count);
        }


        /*!
            @brief      Return a future which will become ready once from_lue has closed dataset @a path
                        for call @a count

            This function must be called from a worker locality.
        */
        auto from_lue_close_dataset_done(std::filesystem::path const& path, Count const count)
            -> hpx::shared_future<void>
        {
            return count > 0 ? from_lue_close_dataset_serializer().when_done(path, count)
                             : hpx::make_ready_future().share();
        }


        /*!
            @brief      Return a future which will become ready once to_lue has closed dataset @a path for
                        call @a count

            This function must be called from a worker locality.
        */
        auto to_lue_close_dataset_done(std::filesystem::path const& path, Count const count)
            -> hpx::shared_future<void>
        {
            return count > 0 ? to_lue_close_dataset_serializer().when_done(path, count)
                             : hpx::make_ready_future().share();
        }
#endif

    }  // namespace worker

}  // namespace lue::detail
