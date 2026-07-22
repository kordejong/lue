#define BOOST_TEST_MODULE lue framework api cxx focal slope
#include "unit_test.hpp"
#include "lue/framework/api/cxx/focal/slope.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(slope)
{
    using Element = lue::FloatingPointElement<0>;
    using Array = lue::PartitionedArray<Element, 2>;
    using Literal = Element;

    lue::api::Field const elevation = Array{};
    lue::api::Literal const cell_size = Literal{};
    lue::api::Field const slope = lue::api::slope(elevation, cell_size);

    BOOST_CHECK(std::holds_alternative<Array>(slope.variant()));
}
