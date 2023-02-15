#include "lue/framework/algorithm/definition/zonal_maximum.hpp"
#include "lue/framework/algorithm/value_policies/zonal_maximum.hpp"


#define LUE_INSTANTIATE(Policies, Element, Zone)              \
                                                              \
    LUE_INSTANTIATE_ZONAL_MAXIMUM(                            \
        ESC(policy::zonal_maximum::Policies<Element, Zone>),  \
        Element,                                              \
        Zone);


namespace lue {

    LUE_INSTANTIATE(DefaultValuePolicies, std::uint8_t, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint8_t, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint8_t, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint8_t, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint8_t, std::int64_t);

    LUE_INSTANTIATE(DefaultValuePolicies, std::int32_t, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int32_t, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int32_t, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int32_t, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int32_t, std::int64_t);

    LUE_INSTANTIATE(DefaultValuePolicies, std::uint32_t, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint32_t, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint32_t, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint32_t, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint32_t, std::int64_t);

    LUE_INSTANTIATE(DefaultValuePolicies, std::int64_t, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int64_t, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int64_t, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int64_t, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::int64_t, std::int64_t);

    LUE_INSTANTIATE(DefaultValuePolicies, std::uint64_t, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint64_t, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint64_t, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint64_t, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, std::uint64_t, std::int64_t);

    LUE_INSTANTIATE(DefaultValuePolicies, float, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, float, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, float, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, float, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, float, std::int64_t);

    LUE_INSTANTIATE(DefaultValuePolicies, double, std::uint8_t);
    LUE_INSTANTIATE(DefaultValuePolicies, double, std::uint32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, double, std::uint64_t);
    LUE_INSTANTIATE(DefaultValuePolicies, double, std::int32_t);
    LUE_INSTANTIATE(DefaultValuePolicies, double, std::int64_t);

}  // namespace lue