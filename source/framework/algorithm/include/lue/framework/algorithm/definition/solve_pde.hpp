#pragma once
#include "lue/framework/algorithm/definition/focal_operation.hpp"
#include "lue/framework/algorithm/focal_operation_export.hpp"
#include "lue/framework/algorithm/kernel.hpp"
#include "lue/framework/algorithm/solve_pde.hpp"


namespace lue {
    namespace detail {

        // Central difference requires Upwind kernel as backup for when no-data cells are encountered

        // Functor for solving first-order derivative
        // This may not matter. Try to generalize it to the Upwind method. We use it for the first-order
        // derivative but we can potentially change methods later.
        template<std::floating_point Element>
        class Upwind  // Upwind: single kernel, multiply by -1 depending on values
        {

            public:

                static constexpr char const* name{"upwind"};

                using InputElement = Element;
                using OutputElement = Element;


                // Operator for solving first-order derivative
                template<typename Kernel, typename OutputPolicies, typename InputPolicies, typename Subspan>
                auto operator()(
                    Kernel const& kernel,
                    [[maybe_unused]] OutputPolicies const& output_policies,
                    [[maybe_unused]] InputPolicies const& c_x_policies,
                    [[maybe_unused]] InputPolicies const& c_y_policies,
                    [[maybe_unused]] InputPolicies const& dirichlet_boundary_condition_policies,
                    [[maybe_unused]] InputPolicies const& neumann_boundary_condition_policies,
                    [[maybe_unused]] Subspan const& c_x_window,
                    [[maybe_unused]] Subspan const& c_y_window,
                    [[maybe_unused]] Subspan const& dirichlet_boundary_condition_window,
                    [[maybe_unused]] Subspan const& neumann_boundary_condition_window) const -> OutputElement
                {
                    // using Weight = ElementT<Kernel>;

                    // TODO: Skip computation where dirichlet is_valid
                    // TODO: Neumann / fixed rate cells should use value passed
                    // TODO: Only pass Neumann for first order derivatives

                    // Since neumann is passed, we are solving the first-order derivative

                    lue_hpx_assert(c_x_window.extent(0) == kernel.size());
                    lue_hpx_assert(c_x_window.extent(1) == kernel.size());

                    // auto const& indp = input_policies.input_no_data_policy();
                    // auto const& ondp = output_policies.output_no_data_policy();

                    OutputElement sum{0};
                    // Count sum_of_weights{0};

                    // for (Index idx0 = 0; idx0 < c_x_window.extent(0); ++idx0)
                    // {
                    //     for (Index idx1 = 0; idx1 < c_x_window.extent(1); ++idx1)
                    //     {
                    //         InputElement const value{c_x_window[idx0, idx1]};
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
        @param      c_t Time coefficient
        @param      c_x First order derivative wrt x
        @param      c_y First order derivative wrt y
        @param      c_xx Second order derivative wrt x
        @param      c_yy Second order derivative wrt y
        @param      c_xy Second order derivative wrt x and y
        @param      rhs
        @param      dirichlet_boundary_condition Fixed value boundary condition
        @param      neumann_boundary_condition Fixed rate boundary condition
        @ingroup    focal_operation

        TODO
    */
    template<typename Policies>
        requires std::floating_point<policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 0>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 1>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 2>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 3>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 4>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 5>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 6>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 7>, policy::OutputElementT<Policies, 0>> &&
                 std::same_as<policy::InputElementT<Policies, 8>, policy::OutputElementT<Policies, 0>>
    auto solve_pde(
        [[maybe_unused]] Policies const& policies,
        [[maybe_unused]] Count const nr_ticks,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 0>, 2> const&
            c_t,  // TODO: c_t is time coefficient
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 1>, 2> const& c_x,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 2>, 2> const& c_y,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 3>, 2> const& c_xx,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 4>, 2> const& c_yy,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 5>, 2> const& c_xy,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 6>, 2> const& rhs,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 7>, 2> const&
            dirichlet_boundary_condition,
        [[maybe_unused]] PartitionedArray<policy::InputElementT<Policies, 8>, 2> const&
            neumann_boundary_condition) -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>
    {
        using Element = lue::policy::OutputElementT<Policies, 0>;
        using UpwindFunctor = detail::Upwind<Element>;
        using Array = PartitionedArray<Element, 2>;

        // TODO: Configure kernel correctly
        // TODO: pass kernel combining backward and forward kernel weights. In the functor, the weights can be
        //       inverted based on the values.
        // https://en.wikipedia.org/wiki/Finite_difference_coefficient
        // First order deriv:
        // - upwind, accuracy 1 (oh^1)
        //     - Switch between forward / backward
        // Second order deriv:
        // - central difference, accuracy 2 (oh2)
        //     - Switch to forward / backward if needed
        auto const upwind_kernel = lue::box_kernel<Element, 2>(1, 1);

        // - Boundary cells are always insіde of the area, outside of the boundary

        Array result{};

        for (Count tick = 0; tick < nr_ticks; ++tick)
        {
            auto /* const */ first_order_derivative = focal_operation(
                policies,
                upwind_kernel,
                UpwindFunctor{/* xxxxx */},
                c_x,
                c_y,
                dirichlet_boundary_condition,
                neumann_boundary_condition);

            // TODO: solve second derivatives for x, y, and xy. Take input policies into account.
            // auto const derivative2 =
            //     focal_operation(policies, kernel, Functor{}, dirichlet_boundary_condition, argument3,
            //     argument4);
            // auto const derivative3 =
            //     focal_operation(policies, kernel, Functor{}, dirichlet_boundary_condition, argument5,
            //     argument6);

            // TODO: Assemble for internal time step. Take input policies into account.

            result = std::move(first_order_derivative);
        }

        return result;
    }

}  // namespace lue


#define LUE_INSTANTIATE_SOLVE_PDE(Policies)                                                                  \
                                                                                                             \
    template LUE_FOCAL_OPERATION_EXPORT auto solve_pde<ArgumentType<void(Policies)>>(                        \
        ArgumentType<void(Policies)> const&,                                                                 \
        Count,                                                                                               \
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 1>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 2>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 3>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 4>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 5>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 6>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 7>, 2> const&,                                      \
        PartitionedArray<policy::InputElementT<Policies, 8>, 2> const&)                                      \
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;
