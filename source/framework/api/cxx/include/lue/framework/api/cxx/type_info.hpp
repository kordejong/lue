#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include "lue/framework/api/cxx/field.hpp"
#include "lue/framework/api/cxx/literal.hpp"
#include "lue/framework/api/cxx/scalar.hpp"
#include <cstdint>


namespace lue {

    enum class DataModel : std::uint8_t {
        Array,
        Scalar,
        Literal,
    };


    namespace api {

        LUE_FAPI_EXPORT auto data_model(Field const& field) -> DataModel;

        LUE_FAPI_EXPORT auto element_type(Field const& field) -> ElementType;

        LUE_FAPI_EXPORT auto element_type(Literal const& literal) -> ElementType;

        LUE_FAPI_EXPORT auto element_type(Scalar const& scalar) -> ElementType;

    }  // namespace api
}  // namespace lue
