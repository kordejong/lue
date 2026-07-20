#define BOOST_TEST_MODULE lue framework api cxx focal majority
#include "unit_test.hpp"
#include "lue/framework/api/cxx/focal/majority.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(focal_majority_raster_kernel)
{
    using Element = lue::SmallestIntegralElement;
    using Weight = lue::BooleanElement;
    using Array = lue::PartitionedArray<Element, 2>;
    using Kernel = lue::Kernel<Weight, 2>;

    lue::api::Field const raster = Array{};
    lue::api::BooleanKernel const kernel = Kernel{};

    lue::api::Field result = lue::api::focal_majority(raster, kernel);

    BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
}
