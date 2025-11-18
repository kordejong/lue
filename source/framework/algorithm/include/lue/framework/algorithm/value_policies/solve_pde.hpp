#pragma once
#include "lue/framework/algorithm/solve_pde.hpp"


namespace lue {
    namespace policy::solve_pde {

        template<std::floating_point Element>
        using DefaultValuePolicies = policy::DefaultSpatialOperationValuePolicies<
            AllValuesWithinDomain<Element, Element, Element, Element>,
            OutputElements<Element>,
            InputElements<Element, Element, Element, Element>>;

    }  // namespace policy::solve_pde


    namespace value_policies {

        template<std::floating_point Element>
        auto solve_pde(
            PartitionedArray<Element, 2> const& dirichlet_boundary_condition,
            PartitionedArray<Element, 2> const& argument1,
            PartitionedArray<Element, 2> const& argument2,
            PartitionedArray<Element, 2> const& argument3) -> PartitionedArray<Element, 2>
        {
            using Policies = policy::solve_pde::DefaultValuePolicies<Element>;

            // TODO: revisit: no-data equals Neumann boundary condition, right?
            Element const fill_value{policy::no_data_value<Element>};

            return solve_pde(
                Policies{fill_value, fill_value, fill_value, fill_value},
                dirichlet_boundary_condition,
                argument1,
                argument2,
                argument3);
        }

    }  // namespace value_policies
}  // namespace lue
