#pragma once
#include "lue/framework/algorithm/flow_accumulation.hpp"
#include "lue/framework/partitioned_array.hpp"


namespace lue {
    namespace policy::accu {

        template<typename FlowDirectionElement, typename MaterialElement>
        using DefaultPolicies = flow_accumulation::DefaultPolicies<FlowDirectionElement, MaterialElement>;

        template<typename FlowDirectionElement, typename MaterialElement>
        using DefaultValuePolicies =
            flow_accumulation::DefaultValuePolicies<FlowDirectionElement, MaterialElement>;

    }  // namespace policy::accu


    template<typename Policies, typename FlowDirectionElement, typename MaterialElement, Rank rank>
    auto accu(
        Policies const& policies,
        PartitionedArray<FlowDirectionElement, rank> const& flow_direction,
        PartitionedArray<MaterialElement, rank> const& material) -> PartitionedArray<MaterialElement, rank>;


    template<typename FlowDirectionElement, typename MaterialElement, Rank rank>
    auto accu(
        PartitionedArray<FlowDirectionElement, rank> const& flow_direction,
        PartitionedArray<MaterialElement, rank> const& material) -> PartitionedArray<MaterialElement, rank>
    {
        using Policies = policy::accu::DefaultPolicies<FlowDirectionElement, MaterialElement>;

        return accu(Policies{}, flow_direction, material);
    }

}  // namespace lue
