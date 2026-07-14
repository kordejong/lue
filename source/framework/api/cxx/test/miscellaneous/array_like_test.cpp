#define BOOST_TEST_MODULE lue framework api cxx create_array
#include "unit_test.hpp"
#include "lue/framework/api/cxx/miscellaneous/array_like.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(array_like)
{
    using InputElement = lue::SmallestFloatingPointElement;
    using OutputElement = lue::LargestIntegralElement;
    using InputArray = lue::PartitionedArray<InputElement, 2>;
    using Scalar = lue::Scalar<OutputElement>;
    using OutputArray = lue::PartitionedArray<OutputElement, 2>;

    {
        lue::api::Array const input_array = InputArray{};
        lue::api::Scalar const fill_value = Scalar{};
        lue::api::Array const output_array = lue::api::array_like(input_array, fill_value);

        BOOST_CHECK(std::holds_alternative<OutputArray>(output_array.variant()));
    }
}
