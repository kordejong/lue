#pragma once
#include <concepts>
#include <ranges>


// TODO: How to specialize alias templates?
// TODO: How to test return types without cv-ness?


namespace lue {

    /*!
        @brief      Primary template, to be specialized
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    template<typename P>
    struct TypeTraits
    {
            // TODO: Put is separate header?
    };


    // IsRange
    // viewable_range


    template<typename T>
    using CountT = TypeTraits<T>::Count;

    template<typename T>
    using ElementT = TypeTraits<T>::Element;

    template<typename T>
    using IndexT = TypeTraits<T>::Index;

    template<typename T>
    using PartitionT = TypeTraits<T>::Partition;

    template<typename T>
    using PartitionsT = TypeTraits<T>::Partitions;

    template<typename T>
    using RankT = TypeTraits<T>::Rank;

    template<typename T>
    using ShapeT = TypeTraits<T>::Shape;


    /*!
        @brief      Something is MultiDimensional if it has a rank
    */
    template<typename T>
    concept MultiDimensional = requires(T object) {
        { rank(object) } -> std::same_as<RankT<T>>;
    };


    /*!
        @brief      A regular grid is a tesselation of multidimensional Euclidian space

        - shape: Shape of array, in cells
        - [idx]: Element access, by linear index
        - TODO: Element access by rank idxs
    */
    template<typename T>
    concept RegularGrid =
        MultiDimensional<T> && std::ranges::contiguous_range<T> && requires(T object, IndexT<T> idx) {
            { shape(object) } -> std::same_as<ShapeT<T> const&>;
            { CountT<T>{} };
            // TODO: Make this work
            // { object[idx] } -> std::same_as<ElementT<T> const&>;
        };


    // /*!
    //     @brief      A Cartesian grid is a regular grid where the elements are unit squares (cubes, ...)
    //
    //     - extent: The size of the "unit of tesselation", along each of the dimensions
    // */
    // template<typename T>
    // concept CartesianGrid = requires(T object) {
    //     typename T::Extent;
    //     { extent(object) } -> std::same_as<typename T::Extent>;
    // };


    /*!
        @brief      An array partition is a rectangular subset of arithmetic array elements

        - TODO: locality_id
    */
    template<typename T>
    concept ArrayPartition = std::default_initializable<T> && RegularGrid<T> &&
                             std::is_arithmetic_v<ElementT<T>>; /* && requires(T object) {
                                 { locality(object) } -> std::same_as<hpx::locality_id>;
                             }; */


    /*!
        @brief      An array in this context is a partitioned array

        A partitioned arrays is an array whose elements are partitions. Each partition contains a subset of
        the array elements.
    */
    template<typename T>
    concept Array = std::default_initializable<T> && RegularGrid<T> && ArrayPartition<PartitionT<T>> &&
                    requires(T object, ShapeT<T> shape, PartitionsT<T> partitions_) {
                        { partitions(object) } -> std::same_as<PartitionsT<T> const&>;
                        { nr_partitions(object) } -> std::same_as<CountT<T>>;
                        { T{shape, std::move(partitions_)} };
                        { partitions_.reserve(CountT<T>{}) };
                        { partitions_.push_back(PartitionT<T>{}) };
                    };

    // { object[idx] } -> std::same_as<ElementT<T> const&>;

    // std::is_class_v<PartitionT<T>>;

    // template<Array A>
    // auto nr_partitions(A const&) -> CountT<A> const;
    //
    // template<Array A>
    // auto shape(A const&) -> ShapeT<A> const&;

}  // namespace lue
