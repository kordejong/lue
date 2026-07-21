#define BOOST_TEST_MODULE lue framework api cxx focal convolve
#include "unit_test.hpp"
#include "lue/framework/api/cxx/focal/convolve.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(convolve)
{
    using Element = lue::FloatingPointElement<0>;
    using Weight = Element;
    using Array = lue::PartitionedArray<Element, 2>;
    using Kernel = lue::Kernel<Weight, 2>;

    lue::api::Field const raster = Array{};
    lue::api::Kernel const kernel = Kernel{};
    lue::api::Field const result = lue::api::convolve(raster, kernel);

    BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
}
