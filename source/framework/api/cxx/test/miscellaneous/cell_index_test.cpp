#define BOOST_TEST_MODULE lue framework api cxx cell_index
#include "unit_test.hpp"
#include "lue/framework/api/cxx/miscellaneous/cell_index.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(cell_index)
{
    using ConditionElement = lue::BooleanElement;
    using InputArray = lue::PartitionedArray<ConditionElement, 2>;
    using OutputArray = lue::PartitionedArray<lue::IndexElement, 2>;

    lue::api::Field const raster = InputArray{};
    lue::api::Field result = lue::api::cell_index(raster, 0);

    BOOST_CHECK(std::holds_alternative<OutputArray>(result.variant()));
}
