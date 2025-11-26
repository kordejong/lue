#define BOOST_TEST_MODULE lue framework algorithm concept
#include "lue/framework/algorithm/concept.hpp"
#include "lue/concept.hpp"
#include <boost/test/included/unit_test.hpp>
#include <utility>


// Order matters: make sure generic code knowns / sees as little code as possible
// - Types:
//     - my_ns::Raster<Array>
//     - my_ns::Array<Partition>
//     - my_ns::Partition<Element>
// - Functions to bridge concepts with types:
//     - my_ns::f(Raster const&)
//     - my_ns::f(Array const&)
//     - my_ns::f(Partition const&)
// - Traits to bridge concepts with types:
//     - lue::TypeTraits<Raster>
//     - lue::TypeTraits<Array>
//     - lue::TypeTraits<Element>
// - Generic algorithms, in terms of types with requirements (concepts)

namespace my_ns {

    template<lue::Array Array_>
    class MyRaster
    {

        public:

            using MyArray = Array_;

            using MyCount = typename MyArray::MyCount;

            using MyExtent = double;

            //! Shape in real-world distance
            using MyShape = std::vector<MyExtent>;


            MyRaster(std::string crs, MyArray&& array):
                _crs{std::move(crs)},
                _array{std::move(array)}
            {
            }


            auto my_crs() const -> std::string const&
            {
                return _crs;
            }


            auto my_array() const -> MyArray const&
            {
                return _array;
            }

        private:

            std::string _crs;

            MyArray _array;
    };

    // Global functions to map the functions expected by the Raster concept to our member functions

    template<typename Array>
    auto crs(MyRaster<Array> const& raster) -> std::string const&
    {
        return raster.my_crs();
    }


    template<typename Array>
    auto array(MyRaster<Array> const& raster) -> Array const&
    {
        return raster.my_array();
    }


    template<typename Array>
    auto cell_shape(MyRaster<Array> const& raster) -> MyRaster<Array>::MyShape const&
    {
        return raster.my_cell_shape();
    }


    template<typename Array>
    auto shape(MyRaster<Array> const& raster) -> MyRaster<Array>::MyShape const&
    {
        return raster.my_shape();
    }


    template<lue::ArrayPartition Partition_>
    class MyArray
    {

        public:

            using MyPartition = Partition_;

            using MyPartitions = std::vector<MyPartition>;

            using MyIterator = MyPartitions::iterator;

            using MyConstIterator = MyPartitions::const_iterator;

            using MyRank = MyPartitions::size_type;

            using MyCount = MyPartitions::size_type;

            using MyIndex = MyPartitions::size_type;

            //! Shape in cells
            using MyShape = std::vector<MyCount>;


            MyArray() = default;


            MyArray(MyShape const& shape, MyPartitions&& partitions):

                _shape{shape},
                _partitions{std::move(partitions)}

            {
            }

            auto my_rank() const -> MyRank
            {
                return _shape.size();
            }


            auto my_shape() const -> MyShape const&
            {
                return _shape;
            }


            auto my_nr_elements() const -> MyCount
            {
                // TODO:
                return 0;
            }


            auto my_empty() const -> bool
            {
                return my_nr_elements() == 0;
            }


            auto my_nr_partitions() const -> MyCount
            {
                return _partitions.size();
            }


            auto my_partitions() const -> MyPartitions const&
            {
                return _partitions;
            }


            auto my_begin() const -> MyConstIterator
            {
                return _partitions.begin();
            }


            auto my_end() const -> MyConstIterator
            {
                return _partitions.end();
            }


            auto my_begin() -> MyIterator
            {
                return _partitions.begin();
            }


            auto my_end() -> MyIterator
            {
                return _partitions.end();
            }


            auto operator[](MyIndex const idx) const -> MyPartition const&
            {
                return _partitions[idx];
            }


            auto operator[](MyIndex const idx) -> MyPartition&
            {
                return _partitions[idx];
            }


        private:

            MyShape _shape;

            std::vector<MyPartition> _partitions;
    };


    template<typename Partition>
    auto rank(MyArray<Partition> const& array) -> MyArray<Partition>::MyRank
    {
        return array.my_rank();
    }


    template<typename Partition>
    auto nr_partitions(MyArray<Partition> const& array) -> MyArray<Partition>::MyCount
    {
        return array.my_nr_partitions();
    }


    template<typename Partition>
    auto shape(MyArray<Partition> const& array) -> MyArray<Partition>::MyShape const&
    {
        return array.my_shape();
    }


    template<typename Partition>
    auto partitions(MyArray<Partition> const& array) -> MyArray<Partition>::MyPartitions const&
    {
        return array.my_partitions();
    }


    template<typename Partition>
    auto begin(MyArray<Partition>& array) -> MyArray<Partition>::MyIterator
    {
        return array.my_begin();
    }


    template<typename Partition>
    auto end(MyArray<Partition>& array) -> MyArray<Partition>::MyIterator
    {
        return array.my_end();
    }


    template<typename Partition>
    auto begin(MyArray<Partition> const& array) -> MyArray<Partition>::MyConstIterator
    {
        return array.my_begin();
    }


    template<typename Partition>
    auto end(MyArray<Partition> const& array) -> MyArray<Partition>::MyConstIterator
    {
        return array.my_end();
    }


    template<lue::Arithmetic Element_>
    class MyPartition
    {

        public:

            using MyElement = Element_;

            using MyElements = std::vector<MyElement>;

            using MyIterator = MyElements::iterator;

            using MyConstIterator = MyElements::const_iterator;

            using MyRank = MyElements::size_type;

            using MyIndex = MyElements::size_type;

            using MyCount = MyElements::size_type;

            //! Shape in cells
            using MyShape = std::vector<MyCount>;


            auto my_rank() const -> MyRank
            {
                return _shape.size();
            }


            auto my_shape() const -> MyShape const&
            {
                return _shape;
            }


            auto my_nr_elements() const -> MyCount
            {
                return 0;
            }


            auto my_empty() -> bool
            {
                return my_nr_elements() == 0;
            }


            auto my_begin() const -> MyConstIterator
            {
                return _elements.begin();
            }


            auto my_end() const -> MyConstIterator
            {
                return _elements.end();
            }


            auto my_begin() -> MyIterator
            {
                return _elements.begin();
            }


            auto my_end() -> MyIterator
            {
                return _elements.end();
            }


            auto operator[](MyIndex const idx) const -> MyElement const&
            {
                return _elements[idx];
            }


            auto operator[](MyIndex const idx) -> MyElement&
            {
                return _elements[idx];
            }

        private:

            MyShape _shape;

            std::vector<MyElement> _elements;
    };


    template<lue::Arithmetic Element>
    auto nr_elements(MyPartition<Element> const& partition) -> MyPartition<Element>::MyCount
    {
        return partition.my_nr_elements();
    }


    template<lue::Arithmetic Element>
    auto empty(MyPartition<Element> const& partition) -> bool
    {
        return partition.my_empty();
    }


    template<lue::Arithmetic Element>
    auto rank(MyPartition<Element> const& partition) -> MyPartition<Element>::MyRank
    {
        return partition.my_rank();
    }


    template<lue::Arithmetic Element>
    auto shape(MyPartition<Element> const& partition) -> MyPartition<Element>::MyShape const&
    {
        return partition.my_shape();
    }


    template<lue::Arithmetic Element>
    auto begin(MyPartition<Element>& partition) -> MyPartition<Element>::MyIterator
    {
        return partition.my_begin();
    }


    template<lue::Arithmetic Element>
    auto end(MyPartition<Element>& partition) -> MyPartition<Element>::MyIterator
    {
        return partition.my_end();
    }


    template<lue::Arithmetic Element>
    auto begin(MyPartition<Element> const& partition) -> MyPartition<Element>::MyConstIterator
    {
        return partition.my_begin();
    }


    template<lue::Arithmetic Element>
    auto end(MyPartition<Element> const& partition) -> MyPartition<Element>::MyConstIterator
    {
        return partition.my_end();
    }

}  // namespace my_ns


namespace lue {

    /*!
        @brief      Type traits to map the types expected by the Raster concept to our types
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    template<typename Array_>
    struct TypeTraits<my_ns::MyRaster<Array_>>
    {
            using Array = typename my_ns::MyRaster<Array_>::MyArray;
            using Count = typename my_ns::MyRaster<Array_>::MyCount;
            using Extent = typename my_ns::MyRaster<Array_>::MyExtent;
            using Shape = typename my_ns::MyRaster<Array_>::MyShape;
    };


    /*!
        @brief      Type traits to map the types expected by the Array concept to our types
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    template<typename Partition_>
    struct TypeTraits<my_ns::MyArray<Partition_>>
    {
            using Count = typename my_ns::MyArray<Partition_>::MyCount;
            using Index = typename my_ns::MyArray<Partition_>::MyIndex;
            using Partition = typename my_ns::MyArray<Partition_>::MyPartition;
            using Partitions = typename my_ns::MyArray<Partition>::MyPartitions;
            using Rank = typename my_ns::MyArray<Partition_>::MyRank;
            using Shape = typename my_ns::MyArray<Partition_>::MyShape;
    };


    /*!
        @brief      Type traits to map the types expected by the Partition concept to our types
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    template<typename Element_>
    struct TypeTraits<my_ns::MyPartition<Element_>>
    {
            using Count = typename my_ns::MyPartition<Element_>::MyCount;
            using Element = typename my_ns::MyPartition<Element_>::MyElement;
            using Index = typename my_ns::MyPartition<Element_>::MyIndex;
            using Rank = typename my_ns::MyPartition<Element_>::MyRank;
            using Shape = typename my_ns::MyPartition<Element_>::MyShape;
    };

}  // namespace lue


namespace my_ns {
    namespace detail {

        template<lue::Array Array>
        auto add([[maybe_unused]] Array const& array1, [[maybe_unused]] Array const& array2) -> Array
        {
            // Only implement the algorithm in terms of the Array concept's properties / requirements
            // If something is missing, add it to the concept
            // TODO: Use a namespace for that? naive, nocheck, compatible, ...?
            // TODO: How to differentiatie between compatibility checks that are cheap and ones that are
            // expensive / require synchronization? immediate vs delayed checks

            // Arrays are assumed to be compatible here

            // TODO: implement and extend concept

            using Index = lue::IndexT<Array>;
            using Count = lue::CountT<Array>;
            using Partition = lue::PartitionT<Array>;
            using Partitions = lue::PartitionsT<Array>;
            using Shape = lue::ShapeT<Array>;

            Shape const& shape{my_ns::shape(array1)};
            Count const nr_partitions{my_ns::nr_partitions(array1)};
            [[maybe_unused]] Partitions const& partitions1{partitions(array1)};
            [[maybe_unused]] Partitions const& partitions2{partitions(array2)};
            Partitions partitions{};
            partitions.reserve(nr_partitions);

            for (Index partition_idx = 0; partition_idx < nr_partitions; ++partition_idx)
            {
                Partition partition{};

                partitions.push_back(std::move(partition));

                // partitions.push_back(
                //     hpx::dataflow(
                //         [](hpx::locality_id const locality_id, Partition const& partition1, Partition
                //         const& partition2) { return action(locality_id, partition1, partition2); },
                //         partitions1[partition_idx].locality(),
                //         partitions1[partition_idx],
                //         partitions2[partition_idx]));
            }

            return Array{shape, std::move(partitions)};
        }

    }  // namespace detail


    template<lue::Array Array>
    auto compatible(Array const& array1, Array const& array2) -> bool
    {
        return shape(array1) == shape(array2);
    }


    template<lue::Array Array, lue::Array... Arrays>
    auto compatible(Array const& array, Arrays const&... arrays) -> bool
    {
        if constexpr (sizeof...(Arrays) == 0)
        {
            return true;
        }

        return compatible(array, std::get<0>(arrays...)) && compatible(arrays...);
    }


    template<lue::Raster Raster>
    auto compatible(Raster const& raster1, Raster const& raster2) -> bool
    {
        // TODO: Compare cell_size, crs, ...
        return compatible(array(raster1), array(raster2));
    }


    template<lue::Raster Raster, lue::Raster... Rasters>
    auto compatible(Raster const& raster, Rasters const&... rasters) -> bool
    {
        if constexpr (sizeof...(Rasters) == 0)
        {
            return true;
        }

        return compatible(raster, std::get<0>(rasters...)) && compatible(rasters...);
    }


    template<lue::Array Array>
    auto add(Array const& array1, Array const& array2) -> Array
    {
        // Only implement the algorithm in terms of the Array concept's properties / requirements
        // If something is missing, add it to the concept

        if (!compatible(array1, array2))
        {
            throw std::runtime_error("arrays not compatible");
        }

        return detail::add(array1, array2);
    }


    template<lue::Raster Raster>
    auto add(Raster const& raster1, Raster const& raster2) -> Raster
    {
        // Only implement the algorithm in terms of the Raster concept's properties / requirements
        // If something is missing, add it to the concept

        if (!compatible(raster1, raster2))
        {
            throw std::runtime_error("rasters not compatible");
        }

        return Raster{crs(raster1), detail::add(array(raster1), array(raster2))};
    }

}  // namespace my_ns


// TODO: No need for the concept tests to be run at runtime. No need for this test to be dependent on UTF.

BOOST_AUTO_TEST_CASE(concept_)
{
    using Element = std::int32_t;
    using Partition = my_ns::MyPartition<Element>;
    using Array = my_ns::MyArray<Partition>;
    using Raster = my_ns::MyRaster<Array>;

    // Partition
    static_assert(lue::ArrayPartition<Partition>);
    static_assert(!lue::Array<Partition>);
    static_assert(!lue::Raster<Partition>);

    // Array
    static_assert(!lue::ArrayPartition<Array>);
    static_assert(lue::Array<Array>);
    static_assert(!lue::Raster<Array>);

    // Raster
    static_assert(!lue::ArrayPartition<Raster>);
    static_assert(!lue::Array<Raster>);
    static_assert(lue::Raster<Raster>);
}


BOOST_AUTO_TEST_CASE(add_array)
{
    using Element = std::int32_t;
    using Partition = my_ns::MyPartition<Element>;
    using Array = my_ns::MyArray<Partition>;

    [[maybe_unused]] Array const array = my_ns::add(Array{}, Array{});
}


BOOST_AUTO_TEST_CASE(add_raster)
{
    using Element = std::int32_t;
    using Partition = my_ns::MyPartition<Element>;
    using Array = my_ns::MyArray<Partition>;
    using Raster = my_ns::MyRaster<Array>;

    std::string const crs{};

    [[maybe_unused]] Raster const raster = my_ns::add(Raster{crs, Array{}}, Raster{crs, Array{}});
}
