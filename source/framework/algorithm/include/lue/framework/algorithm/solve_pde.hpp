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
                 std::same_as<policy::InputElementT<Policies, 6>, policy::OutputElementT<Policies, 0>>
    auto solve_pde(
        Policies const& policies,
        // Put boundaries at the end
        // Fixed values, to copy to output
        // Optional!
        PartitionedArray<policy::InputElementT<Policies, 0>, 2> const& dirichlet_boundary_condition,
        // First order derivative, fixed values, to use in equation
        // Optional!
        // PartitionedArray<policy::InputElementT<Policies, 0>, 2> const& neumann_boundary_condition,
        // c_t is time coefficient
        PartitionedArray<policy::InputElementT<Policies, 1>, 2> const& c_t,  // 3.
        PartitionedArray<policy::InputElementT<Policies, 2>, 2> const& c_x,  // ...
        PartitionedArray<policy::InputElementT<Policies, 3>, 2> const& c_y,
        PartitionedArray<policy::InputElementT<Policies, 4>, 2> const& c_xx,
        PartitionedArray<policy::InputElementT<Policies, 5>, 2> const& c_yy,
        PartitionedArray<policy::InputElementT<Policies, 6>, 2> const& c_xy
        // , PartitionedArray<policy::InputElementT<Policies, 7>, 2> const& rhs
        // 1. duration (units known)
        // 2. timestep_duration (units known)
        // duration % timestep must be zero (? error or do the best we can)
        ) -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;

    // Overloads for scalar coefficients, potentially:
    // - c_x / c_y both scalar
    // - c_xx / c_yy both scalar
    // - c_xy scalar

}  // namespace lue
