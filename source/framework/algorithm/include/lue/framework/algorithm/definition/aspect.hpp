#pragma once
#include "lue/framework/algorithm/aspect.hpp"
#include "lue/framework/algorithm/definition/binary_local_operation.hpp"
#include "lue/framework/algorithm/focal_operation_export.hpp"
#include "lue/framework/algorithm/gradients.hpp"
#include "lue/framework/core/math.hpp"


namespace lue {
    namespace detail {

        template<std::floating_point InputElement>
        class Aspect
        {

            public:

                static_assert(std::is_floating_point_v<InputElement>);

                static constexpr char const* name{"aspect"};

                using OutputElement = InputElement;

                OutputElement operator()(
                    InputElement const gradient_x, InputElement const gradient_y) const noexcept
                {
                    OutputElement result{};

                    if (gradient_x == 0)
                    {
                        if (gradient_y < 0)
                        {
                            // Dip to the north
                            result = 0;
                        }
                        else if (gradient_y > 0)
                        {
                            // Dip to the south
                            result = pi<OutputElement>;
                        }
                    }
                    else if (gradient_y == 0)
                    {
                        if (gradient_x < 0)
                        {
                            // Dip to the west
                            result = pi<OutputElement> + half_pi<OutputElement>;
                        }
                        else if (gradient_x > 0)
                        {
                            // Dip to the east
                            result = half_pi<OutputElement>;
                        }
                    }
                    else
                    {
                        result = std::atan(gradient_x / gradient_y) +
                                 (gradient_x < 0 ? pi<OutputElement> + half_pi<OutputElement>
                                                 : half_pi<OutputElement>);
                    }

                    HPX_ASSERT(std::isnan(result) || (result >= 0 && result <= two_pi<OutputElement>));

                    return result;
                }
        };

    }  // namespace detail


    template<typename Policies>
        requires std::floating_point<policy::InputElementT<Policies, 0>> &&
                 std::floating_point<policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 0>, policy::OutputElementT<Policies, 0>>
    auto aspect(
        [[maybe_unused]] Policies const& policies,
        Gradients<policy::InputElementT<Policies, 0>> const& gradients)
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>
    {
        using Element = policy::InputElementT<Policies, 0>;

        auto const& [dz_dx, dz_dy] = gradients;

        using AspectPolicies = policy::Policies<
            policy::AllValuesWithinDomain<Element, Element>,
            policy::OutputsPolicies<policy::OutputPolicies<
                policy::OutputNoDataPolicy3T<Policies, 0>,
                policy::AllValuesWithinRange<Element, Element, Element>>>,
            policy::InputsPolicies<
                policy::InputPoliciesT<Policies, 0, Element>,
                policy::InputPoliciesT<Policies, 0, Element>>>;

        return binary_local_operation(AspectPolicies{}, dz_dx, dz_dy, detail::Aspect<Element>{});
    }


    template<typename Policies>
        requires std::floating_point<policy::InputElementT<Policies, 0>> &&
                 std::floating_point<policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 0>, policy::OutputElementT<Policies, 0>>
    auto aspect(
        Policies const& policies, PartitionedArray<policy::InputElementT<Policies, 0>, 2> const& elevation)
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>
    {
        using Element = policy::InputElementT<Policies, 0>;

        // TODO Somehow deduce this type from the policy passed in. This must result in one of
        //      the policy types for which we instantiate the gradient operation. For now,
        //      we just use the same policy as used in policy::gradients::DefaultValuePolicies.
        using GradientsPolicies = policy::DefaultSpatialOperationValuePolicies<
            policy::AllValuesWithinDomain<Element, Element>,
            policy::OutputElements<Element>,
            policy::InputElements<Element, Element>>;

        // TODO We are making cell_size up here. For aspect this is OK, right?
        return aspect(policies, gradients(GradientsPolicies{}, elevation, Element{1}));
    }

}  // namespace lue


#define LUE_INSTANTIATE_ASPECT(Policies)                                                                     \
                                                                                                             \
    template LUE_FOCAL_OPERATION_EXPORT auto aspect<ArgumentType<void(Policies)>>(                           \
        ArgumentType<void(Policies)> const&, PartitionedArray<policy::InputElementT<Policies, 0>, 2> const&) \
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;
