#pragma once
#include "lue/framework/algorithm/binary_local_operation.hpp"
#include "lue/framework/algorithm/operator.hpp"
#include "lue/framework/algorithm/policy.hpp"


namespace lue {
    namespace detail {

        template<typename InputElement, typename OutputElement_>
        class LessThan
        {

            public:

                static_assert(std::is_integral_v<OutputElement_>);

                static constexpr char const* name{"less_than"};

                using OutputElement = OutputElement_;


                constexpr auto operator()(
                    InputElement const& input_element1,
                    InputElement const& input_element2) const noexcept -> OutputElement
                {
                    return input_element1 < input_element2;
                }
        };

    }  // namespace detail


    LUE_BINARY_LOCAL_OPERATION_OVERLOADS_WITH_POLICIES_DIFFERENT_OUTPUT_ELEMENT(less_than, detail::LessThan)

}  // namespace lue
