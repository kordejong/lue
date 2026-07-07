#include "lue/framework/api/cxx/miscellaneous/array_like.hpp"
#include "detail/unsupported_overload.hpp"
#include "overload.hpp"
#include "lue/framework/algorithm/value_policies/array_like.hpp"


namespace lue {

    namespace value_policies {

        auto array_like([[maybe_unused]] auto const& array, auto const& fill_value) -> api::Field
        {
            api::detail::unsupported_overload("array_like", array, fill_value);

            return {};
        }

    }  // namespace value_policies


    namespace api {

        auto array_like(Field const& array, Field const& fill_value) -> Field
        {
            return std::visit(
                overload{
                    [](auto const& array, auto const& fill_value) -> Field
                    { return value_policies::array_like(array, fill_value); }},
                array.variant(),
                fill_value.variant());
        }

    }  // namespace api
}  // namespace lue
