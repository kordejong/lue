#define BOOST_TEST_MODULE lue framework api cxx cast
#include "unit_test.hpp"
#include "lue/framework/api/cxx/miscellaneous/cast.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(cast)
{
    using InputElement = lue::LargestIntegralElement;
    using OutputElement = lue::SmallestFloatingPointElement;
    using InputArray = lue::PartitionedArray<InputElement, 2>;
    using OutputArray = lue::PartitionedArray<OutputElement, 2>;

    {
        lue::api::Field const input_field = InputArray{};
        lue::ElementType const output_element_type = lue::TypeTraits<OutputElement>::element_type;
        lue::api::Field const output_field = lue::api::cast(input_field, output_element_type);

        BOOST_CHECK(std::holds_alternative<OutputArray>(output_field.variant()));
    }
}
