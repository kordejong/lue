#define BOOST_TEST_MODULE lue framework api cxx focal aspect
#include "unit_test.hpp"
#include "lue/framework/api/cxx/focal/aspect.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(aspect)
{
    using Element = lue::FloatingPointElement<0>;
    using Array = lue::PartitionedArray<Element, 2>;

    lue::api::Field const elevation = Array{};
    lue::api::Field const aspect = lue::api::aspect(elevation);

    BOOST_CHECK(std::holds_alternative<Array>(aspect.variant()));
}
