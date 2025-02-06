#define BOOST_TEST_MODULE lue framework api cxx create_array
#include "test_suite.hpp"
#include "lue/framework/api/cxx/create_array.hpp"


BOOST_AUTO_TEST_CASE(create_array)
{
    using Element = lue::LargestIntegralElement;
    using Scalar = lue::Scalar<Element>;
    using Array = lue::PartitionedArray<Element, 2>;
    using Shape = lue::ShapeT<Array>;

    Shape const array_shape{1, 1};
    Shape const partition_shape{1, 1};

    {
        lue::api::Field const fill_value = Element{};
        lue::api::Field result = lue::api::create_array(array_shape, partition_shape, fill_value);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field const fill_value = Scalar{};
        lue::api::Field result = lue::api::create_array(array_shape, partition_shape, fill_value);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(create_array_without_partition_shape)
{
    using Element = lue::LargestIntegralElement;
    using Scalar = lue::Scalar<Element>;
    using Array = lue::PartitionedArray<Element, 2>;
    using Shape = lue::ShapeT<Array>;

    Shape const array_shape{1, 1};

    {
        lue::api::Field const fill_value = Element{};
        lue::api::Field result = lue::api::create_array(array_shape, fill_value);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field const fill_value = Scalar{};
        lue::api::Field result = lue::api::create_array(array_shape, fill_value);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}
