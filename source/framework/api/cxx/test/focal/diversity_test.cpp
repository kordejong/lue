#define BOOST_TEST_MODULE lue framework api cxx focal diversity
#include "unit_test.hpp"
#include "lue/framework/api/cxx/focal/diversity.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(focal_diversity_raster_kernel)
{
    using Element = lue::SmallestIntegralElement;
    using Weight = lue::BooleanElement;
    using InputArray = lue::PartitionedArray<Element, 2>;
    using OutputArray = lue::PartitionedArray<lue::CountElement, 2>;
    using Kernel = lue::Kernel<Weight, 2>;

    lue::api::Field const raster = InputArray{};
    lue::api::Kernel const kernel = Kernel{};

    lue::api::Field result = lue::api::focal_diversity(raster, kernel);

    BOOST_CHECK(std::holds_alternative<OutputArray>(result.variant()));
}
