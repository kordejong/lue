#pragma once
#include "lue/framework/algorithm/policy.hpp"
#include "lue/framework/partitioned_array.hpp"


namespace lue {

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
        Policies const& policies,
        Count nr_ticks,
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const& c_t,
        PartitionedArray<policy::InputElementT<Policies, 1>, 2> const& c_x,
        PartitionedArray<policy::InputElementT<Policies, 2>, 2> const& c_y,
        PartitionedArray<policy::InputElementT<Policies, 3>, 2> const& c_xx,
        PartitionedArray<policy::InputElementT<Policies, 4>, 2> const& c_yy,
        PartitionedArray<policy::InputElementT<Policies, 5>, 2> const& c_xy,
        PartitionedArray<policy::InputElementT<Policies, 6>, 2> const& rhs,
        PartitionedArray<policy::InputElementT<Policies, 7>, 2> const& dirichlet_boundary_condition,
        PartitionedArray<policy::InputElementT<Policies, 8>, 2> const& neumann_boundary_condition)
        -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;

    // TODO:
    // Overloads for scalar coefficients, potentially:
    // - c_x / c_y both scalar
    // - c_xx / c_yy both scalar
    // - c_xy scalar

    // TODO:
    // Overloads with optional boundary conditions(?)

}  // namespace lue
