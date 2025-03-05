#pragma once
#include <netcdf.h>
#include <cstdint>


namespace lue::netcdf4 {

    template<typename T>
    struct TypeTraits
    {
    };


    template<>
    struct TypeTraits<char>
    {
            using ValueType = char;

            static constexpr nc_type type_id{NC_CHAR};

            static constexpr char const* name{"NC_CHAR"};

            static constexpr ValueType fill{NC_FILL_CHAR};

            static constexpr ValueType max{static_cast<ValueType>(NC_MAX_CHAR)};

            static constexpr bool is_numeric{false};
    };


    template<>
    struct TypeTraits<std::int8_t>
    {
            using ValueType = std::int8_t;

            static constexpr nc_type type_id{NC_BYTE};

            static constexpr char const* name{"NC_BYTE"};

            static constexpr ValueType fill{NC_FILL_BYTE};

            static constexpr ValueType min{NC_MIN_BYTE};

            static constexpr ValueType max{NC_MAX_BYTE};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<std::uint8_t>
    {
            using ValueType = std::uint8_t;

            static constexpr nc_type type_id{NC_UBYTE};

            static constexpr char const* name{"NC_UBYTE"};

            static constexpr ValueType fill{NC_FILL_UBYTE};

            static constexpr ValueType max{NC_MAX_UBYTE};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<std::int32_t>
    {
            using ValueType = std::int32_t;

            static constexpr nc_type type_id{NC_INT};

            static constexpr char const* name{"NC_INT"};

            static constexpr ValueType fill{NC_FILL_INT};

            static constexpr ValueType min{NC_MIN_INT};

            static constexpr ValueType max{NC_MAX_INT};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<std::uint32_t>
    {
            using ValueType = std::uint32_t;

            static constexpr nc_type type_id{NC_UINT};

            static constexpr char const* name{"NC_UINT"};

            static constexpr ValueType fill{NC_FILL_UINT};

            static constexpr ValueType max{NC_MAX_UINT};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<std::int64_t>
    {
            using ValueType = std::int64_t;

            static constexpr nc_type type_id{NC_INT64};

            static constexpr char const* name{"NC_INT64"};

            static constexpr ValueType fill{NC_FILL_INT64};

            static constexpr ValueType min{NC_MIN_INT64};

            static constexpr ValueType max{NC_MAX_INT64};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<std::uint64_t>
    {
            using ValueType = std::uint64_t;

            static constexpr nc_type type_id{NC_UINT64};

            static constexpr char const* name{"NC_UINT64"};

            static constexpr ValueType fill{NC_FILL_UINT64};

            static constexpr ValueType max{NC_MAX_UINT64};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<float>
    {
            using ValueType = float;

            static constexpr nc_type type_id{NC_FLOAT};

            static constexpr char const* name{"NC_FLOAT"};

            static constexpr ValueType fill{NC_FILL_FLOAT};

            static constexpr ValueType min{NC_MIN_FLOAT};

            static constexpr ValueType max{NC_MAX_FLOAT};

            static constexpr bool is_numeric{true};
    };


    template<>
    struct TypeTraits<double>
    {
            using ValueType = double;

            static constexpr nc_type type_id{NC_DOUBLE};

            static constexpr char const* name{"NC_DOUBLE"};

            static constexpr ValueType fill{NC_FILL_DOUBLE};

            static constexpr ValueType min{NC_MIN_DOUBLE};

            static constexpr ValueType max{NC_MAX_DOUBLE};

            static constexpr bool is_numeric{true};
    };


    template<typename T>
    constexpr bool is_numeric_v = TypeTraits<T>::is_numeric;


    template<typename T>
    constexpr char const* name_v = TypeTraits<T>::name;

}  // namespace lue::netcdf4
