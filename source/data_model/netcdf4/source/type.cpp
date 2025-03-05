#include "lue/netcdf4/type.hpp"
#include "lue/netcdf4/type_traits.hpp"
#include <cassert>


namespace lue::netcdf4 {

    /*!
        @brief      Return whether @a type_id represents a numeric type
    */
    auto is_numeric(nc_type const type_id) -> bool
    {
        bool result{false};

        switch (type_id)
        {
            case NC_CHAR:
            {
                result = is_numeric_v<char>;
                break;
            }
            case NC_BYTE:
            {
                result = is_numeric_v<std::int8_t>;
                break;
            }
            case NC_UBYTE:
            {
                result = is_numeric_v<std::uint8_t>;
                break;
            }
            case NC_INT:
            {
                result = is_numeric_v<std::int32_t>;
                break;
            }
            case NC_UINT:
            {
                result = is_numeric_v<std::uint32_t>;
                break;
            }
            case NC_INT64:
            {
                result = is_numeric_v<std::int64_t>;
                break;
            }
            case NC_UINT64:
            {
                result = is_numeric_v<std::uint64_t>;
                break;
            }
            case NC_FLOAT:
            {
                result = is_numeric_v<float>;
                break;
            }
            case NC_DOUBLE:
            {
                result = is_numeric_v<double>;
                break;
            }
        }

        return result;
    }


    /*!
        @brief      Return a string representation of @a type_id

        For example, for type ID NC_DOUBLE the string "NC_DOUBLE" is returned.
    */
    auto as_string(nc_type const type_id) -> char const*
    {
        char const* result{nullptr};

        switch (type_id)
        {
            case NC_CHAR:
            {
                result = name_v<char>;
                break;
            }
            case NC_BYTE:
            {
                result = name_v<std::int8_t>;
                break;
            }
            case NC_UBYTE:
            {
                result = name_v<std::uint8_t>;
                break;
            }
            case NC_INT:
            {
                result = name_v<std::int32_t>;
                break;
            }
            case NC_UINT:
            {
                result = name_v<std::uint32_t>;
                break;
            }
            case NC_INT64:
            {
                result = name_v<std::int64_t>;
                break;
            }
            case NC_UINT64:
            {
                result = name_v<std::uint64_t>;
                break;
            }
            case NC_FLOAT:
            {
                result = name_v<float>;
                break;
            }
            case NC_DOUBLE:
            {
                result = name_v<double>;
                break;
            }
        }

        // We have handled all type IDs, right?
        assert(result != nullptr);

        return result;
    }

}  // namespace lue::netcdf4
