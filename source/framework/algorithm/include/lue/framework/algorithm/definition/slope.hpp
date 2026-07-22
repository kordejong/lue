#pragma once
#include "lue/framework/algorithm/definition/add.hpp"
#include "lue/framework/algorithm/definition/pow.hpp"
#include "lue/framework/algorithm/definition/sqrt.hpp"
#include "lue/framework/algorithm/focal_operation_export.hpp"
#include "lue/framework/algorithm/gradients.hpp"
#include "lue/framework/algorithm/slope.hpp"


namespace lue {

    template<typename Policies>
        requires std::floating_point<policy::InputElementT<Policies, 0>> &&
                 std::floating_point<policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 0>, policy::OutputElementT<Policies, 0>>
    auto slope(
        [[maybe_unused]] Policies const& policies,
        Gradients<policy::InputElementT<Policies, 0>> const& gradients)
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>
    {
        using Element = policy::InputElementT<Policies, 0>;

        auto const& [dz_dx, dz_dy] = gradients;

        using PowPolicies = policy::Policies<
            policy::pow::DomainPolicy<Element>,
            policy::OutputsPolicies<policy::OutputPoliciesT<Policies, 0, Element>>,
            policy::InputsPolicies<
                policy::InputPoliciesT<Policies, 0, Element>,
                policy::InputPolicies<policy::SkipNoData<Element>>>>;
        using AddPolicies = policy::Policies<
            policy::add::DomainPolicy<Element>,
            policy::OutputsPolicies<policy::OutputPoliciesT<Policies, 0, Element>>,
            policy::InputsPolicies<
                policy::InputPoliciesT<Policies, 0, Element>,
                policy::InputPoliciesT<Policies, 0, Element>>>;
        using SqrtPolicies = policy::Policies<
            policy::sqrt::DomainPolicy<Element>,
            policy::OutputsPolicies<policy::OutputPolicies<
                // policy::OutputPoliciesT<Policies, 0, Element>
                policy::OutputNoDataPolicy3T<Policies, 0, Element>,
                policy::AllValuesWithinRange<Element, Element>>>,
            policy::InputsPolicies<policy::InputPoliciesT<Policies, 0, Element>>>;

        PowPolicies pow_policies{};
        AddPolicies add_policies{};
        SqrtPolicies sqrt_policies{};

        // TODO Already use atan to get slope angle in radians? All angles (aspect, slope,
        //      ...) in radians seems to make sense.

        return sqrt(
            sqrt_policies,
            add(add_policies, pow(pow_policies, dz_dx, Element{2}), pow(pow_policies, dz_dy, Element{2})));
    }

    /*!
        @brief      Return slope of elevation surface
        @param      elevation Elevation of the terrain
        @param      cell_size Cell size
        @return     Slope field with slope in percentage
        @warning    The result is not correct yet for the cells at the border
                    of the array and the handling of no-data cells (#gh193).

        The units of @a elevation and @a cell_size should be equal.

        To convert the result to degrees, use:

        @code
        slope_in_degrees = atan(slope_in_percentage)
        @endcode

        To convert to radians, use:

        @code
        slope_in_radians = slope_in_degrees * 180 / M_PI
        @endcode

        This algorithm implements Horne's slope algorithm (Horn, B.K.P. (1981)
        Hill shading and the reflectance map. Proceedings of IEEE 69(1), 14-47).
        In pseudo-code this works as follows:

        @code
        dz_dx = convolve(value, dz_dx_kernel) / (8 * cell_size)
        dz_dy = convolve(value, dz_dy_kernel) / (8 * cell_size)
        result = sqrt(pow(dz_dx, 2) + pow(dz_dy, 2))
        @endcode

        where dz_dx_kernel is:

        @code
        +----+----+----+
        |  1 |  0 | -1 |
        +----+----+----+
        |  2 |  0 | -2 |
        +----+----+----+
        |  1 |  0 | -1 |
        +----+----+----+
        @endcode

        and dz_dy_kernel is:

        @code
        +----+----+----+
        | -1 | -2 | -1 |
        +----+----+----+
        |  0 |  0 |  0 |
        +----+----+----+
        |  1 |  2 |  1 |
        +----+----+----+
        @endcode
    */
    template<typename Policies>
        requires std::floating_point<policy::InputElementT<Policies, 0>> &&
                 std::floating_point<policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 0>, policy::OutputElementT<Policies, 0>>
    auto slope(
        Policies const& policies,
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const& elevation,
        policy::InputElementT<Policies, 0> const cell_size)
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>
    {
        return slope(policies, gradients(policies, elevation, cell_size));
    }

}  // namespace lue


#define LUE_INSTANTIATE_SLOPE(Policies)                                                                      \
                                                                                                             \
    template LUE_FOCAL_OPERATION_EXPORT auto slope<ArgumentType<void(Policies)>>(                            \
        ArgumentType<void(Policies)> const&,                                                                 \
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const&,                                      \
        policy::InputElementT<Policies, 0>) -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;
