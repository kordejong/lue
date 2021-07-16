#pragma once
#include "lue/framework/algorithm/divide.hpp"


namespace lue {
    namespace policy::divide {

        template<
            typename OutputElement,
            typename InputElement>
        using DefaultValuePolicies = policy::DefaultValuePolicies<
            DomainPolicy<InputElement>,
            OutputElements<OutputElement>,
            InputElements<InputElement, InputElement>>;

    }  // namespace divide::policy


    namespace value_policies {

        LUE_BINARY_LOCAL_OPERATION_OVERLOADS2(
            divide, detail::Divide, policy::divide::DefaultValuePolicies)
        LUE_BINARY_ARITHMETIC_OPERATOR(/, divide)

    }  // namespace value_policies
}  // namespace lue
