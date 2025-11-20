#pragma once
#include "lue/framework/algorithm/concept/array.hpp"
#include <string>


namespace lue {


    /*!
        @brief      Geographic data is data that has a coordinate reference system (CRS) associated
                    with it

        - crs: Coordinate reference system
    */
    template<typename T>
    concept GeographicData = requires(T object) {
        { crs(object) } -> std::same_as<std::string const&>;
    };


    template<typename T>
    using ExtentT = TypeTraits<T>::Extent;

    // template<typename T>
    // using ArrayT = typename T::Array;

    template<typename T>
    using ArrayT = TypeTraits<T>::Array;


    /*!
        @brief      A raster is a "geographic array"

        - shape: Shape of raster, in real-world distances
        - cell_shape: Shape of cells, in real-world distances
        - array: Underlying collection of cells
        - Constructable by Raster{crs, array}
    */
    template<typename T>
    concept Raster = GeographicData<T> && Array<ArrayT<T>> && requires(T object) {
        { shape(object) } -> std::same_as<ShapeT<T> const&>;
        { cell_shape(object) } -> std::same_as<ShapeT<T> const&>;
        { array(object) } -> std::same_as<ArrayT<T> const&>;
        { T{crs(object), ArrayT<T>{}} };
    };

}  // namespace lue
