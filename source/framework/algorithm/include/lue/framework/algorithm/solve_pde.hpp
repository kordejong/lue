#pragma once
#include "lue/framework/algorithm/policy.hpp"
#include "lue/framework/partitioned_array.hpp"


namespace lue {

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
        /* , Kernel const& kernel */) -> PartitionedArray<policy::OutputElementT<Policies, 0>, 2>;

}  // namespace lue
