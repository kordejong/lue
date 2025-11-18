#pragma once
#include "lue/framework/algorithm/definition/focal_operation.hpp"
#include "lue/framework/algorithm/focal_operation_export.hpp"
#include "lue/framework/algorithm/kernel.hpp"
#include "lue/framework/algorithm/solve_pde.hpp"


namespace lue {
    namespace detail {

        template<std::floating_point Element>
        class SolvePDE
        {

            public:

                static constexpr char const* name{"solve_pde"};

                using InputElement = Element;
                using OutputElement = Element;


                template<typename Kernel, typename OutputPolicies, typename InputPolicies, typename Subspan>
                auto operator()(
                    Kernel const& kernel,
                    [[maybe_unused]] OutputPolicies const& output_policies,
                    [[maybe_unused]] InputPolicies const& dirichlet_boundary_condition_policies,
                    [[maybe_unused]] InputPolicies const& argument1_policies,
                    [[maybe_unused]] InputPolicies const& argument2_policies,
                    [[maybe_unused]] InputPolicies const& argument3_policies,
                    [[maybe_unused]] Subspan const& dirichlet_boundary_condition_window,
                    [[maybe_unused]] Subspan const& argument1_window,
                    [[maybe_unused]] Subspan const& argument2_window,
                    [[maybe_unused]] Subspan const& argument3_window) const -> OutputElement
                {
                    // using Weight = ElementT<Kernel>;

                    lue_hpx_assert(argument1_window.extent(0) == kernel.size());
                    lue_hpx_assert(argument1_window.extent(1) == kernel.size());

                    // auto const& indp = input_policies.input_no_data_policy();
                    // auto const& ondp = output_policies.output_no_data_policy();

                    OutputElement sum{0};
                    // Count sum_of_weights{0};

                    // for (Index idx0 = 0; idx0 < argument1_window.extent(0); ++idx0)
                    // {
                    //     for (Index idx1 = 0; idx1 < argument1_window.extent(1); ++idx1)
                    //     {
                    //         InputElement const value{argument1_window[idx0, idx1]};
                    //
                    //         if (!indp.is_no_data(value))
                    //         {
                    //             Weight const weight{kernel(idx0, idx1)};
                    //
                    //             if (weight)
                    //             {
                    //                 sum += value;
                    //                 ++sum_of_weights;
                    //             }
                    //         }
                    //     }
                    // }
                    //
                    // if (sum_of_weights == 0)
                    // {
                    //     ondp.mark_no_data(sum);
                    // }
                    // else
                    // {
                    //     sum /= sum_of_weights;
                    // }

                    return sum;
                }
        };

    }  // namespace detail


    /*!
        @brief      TODO
        @ingroup    focal_operation

        TODO
    */
    template<typename Policies /* , typename Kernel */>
        requires std::floating_point<policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 0>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 1>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 2>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 3>, policy::OutputElementT<Policies, 0>>
    // && std::integral<ElementT<Kernel>> && (rank<Kernel> == 2)
    auto solve_pde(
        Policies const& policies,
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const& dirichlet_boundary_condition,
        PartitionedArray<policy::InputElementT<Policies, 1>, 2> const& argument1,
        PartitionedArray<policy::InputElementT<Policies, 2>, 2> const& argument2,
        PartitionedArray<policy::InputElementT<Policies, 3>, 2> const& argument3
        /* , Kernel const& kernel */) -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>
    {
        using Element = lue::policy::OutputElementT<Policies, 0>;
        using Functor = detail::SolvePDE<Element>;

        // TODO: Configure kernel correctly
        auto kernel = lue::box_kernel<Element, 2>(1, 1);

        return focal_operation(
            policies,
            std::move(kernel),
            Functor{},
            dirichlet_boundary_condition,
            argument1,
            argument2,
            argument3);
    }

}  // namespace lue


#define LUE_INSTANTIATE_SOLVE_PDE(Policies)                                                                  \
                                                                                                             \
    template LUE_FOCAL_OPERATION_EXPORT auto solve_pde<ArgumentType<void(Policies)>>(                        \
        ArgumentType<void(Policies)> const&,                                                                 \
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 1>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 2>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 3>, 2> const&)                                      \
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;
