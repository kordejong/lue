#pragma once
#include "lue/framework/algorithm/policy.hpp"
#include "lue/framework/algorithm/unary_local_operation.hpp"


namespace lue {
    namespace detail {

        template<typename InputElement>
        class Negate
        {

            public:

                static_assert(std::is_signed_v<InputElement> || std::is_floating_point_v<InputElement>);

                static constexpr char const* name{"negate"};

                using OutputElement = InputElement;

                auto operator()(InputElement const& input_element) const noexcept -> OutputElement
                {
                    return -input_element;
                }
        };

    }  // namespace detail


    LUE_UNARY_LOCAL_OPERATION_OVERLOADS_WITH_POLICIES_SAME_OUTPUT_ELEMENT(negate, detail::Negate)

}  // namespace lue
