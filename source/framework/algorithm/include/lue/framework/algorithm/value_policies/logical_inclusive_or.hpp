#pragma once
#include "lue/framework/algorithm/logical_inclusive_or.hpp"
#include "lue/framework/algorithm/operator.hpp"
#include "lue/framework/configure.hpp"


namespace lue {
    namespace policy::logical_inclusive_or {

        template<typename OutputElement, typename InputElement>
        using DefaultValuePolicies = policy::DefaultValuePolicies<
            AllValuesWithinDomain<InputElement, InputElement>,
            OutputElements<OutputElement>,
            InputElements<InputElement, InputElement>>;

    }  // namespace policy::logical_inclusive_or


    namespace value_policies {

        LUE_BINARY_LOCAL_OPERATION_OVERLOADS_WITHOUT_POLICIES_DIFFERENT_OUTPUT_ELEMENT(
            logical_inclusive_or, policy::logical_inclusive_or::DefaultValuePolicies)
        LUE_BINARY_LOGICAL_OPERATOR(||, logical_inclusive_or, BooleanElement)

    }  // namespace value_policies
}  // namespace lue
