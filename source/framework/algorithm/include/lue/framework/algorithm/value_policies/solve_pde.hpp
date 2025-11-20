#pragma once
#include "lue/framework/algorithm/solve_pde.hpp"


namespace lue {
    namespace policy::solve_pde {

        template<std::floating_point Element>
        using DefaultValuePolicies = policy::DefaultSpatialOperationValuePolicies<
            AllValuesWithinDomain<
                Element,
                Element,
                Element,
                Element,
                Element,
                Element,
                Element,
                Element,
                Element>,
            OutputElements<Element>,
            InputElements<Element, Element, Element, Element, Element, Element, Element, Element, Element>>;

    }  // namespace policy::solve_pde


    namespace value_policies {

        template<std::floating_point Element>
        auto solve_pde(
            Count nr_ticks,
            PartitionedArray<Element, 2> const& c_t,
            PartitionedArray<Element, 2> const& c_x,
            PartitionedArray<Element, 2> const& c_y,
            PartitionedArray<Element, 2> const& c_xx,
            PartitionedArray<Element, 2> const& c_yy,
            PartitionedArray<Element, 2> const& c_xy,
            PartitionedArray<Element, 2> const& rhs,
            PartitionedArray<Element, 2> const& dirichlet_boundary_condition,
            PartitionedArray<Element, 2> const& neumann_boundary_condition) -> PartitionedArray<Element, 2>
        {
            using Policies = policy::solve_pde::DefaultValuePolicies<Element>;

            Element const fill_value{policy::no_data_value<Element>};

            return solve_pde(
                Policies{
                    fill_value,
                    fill_value,
                    fill_value,
                    fill_value,
                    fill_value,
                    fill_value,
                    fill_value,
                    fill_value,
                    fill_value},
                nr_ticks,
                c_t,
                c_x,
                c_y,
                c_xx,
                c_yy,
                c_xy,
                rhs,
                dirichlet_boundary_condition,
                neumann_boundary_condition);
        }

    }  // namespace value_policies
}  // namespace lue
