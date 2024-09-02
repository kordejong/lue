#define BOOST_TEST_MODULE lue framework api cxx local add
#include "test_suite.hpp"
#include "lue/framework/api/cxx/operator.hpp"


BOOST_AUTO_TEST_CASE(add_array_array)
{
    using Element = std::int32_t;
    using Array = lue::PartitionedArray<Element, 2>;

    lue::api::Field const array1 = Array{};
    lue::api::Field const array2 = Array{};

    {
        lue::api::Field result = lue::api::add(array1, array2);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field result = array1 + array2;

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_array_scalar)
{
    using Element = std::int32_t;
    using Array = lue::PartitionedArray<Element, 2>;
    using Scalar = lue::Scalar<Element>;

    lue::api::Field const array = Array{};
    lue::api::Field const scalar = Scalar{};

    {
        lue::api::Field result = lue::api::add(array, scalar);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field result = array + scalar;

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_scalar_array)
{
    using Element = std::int32_t;
    using Scalar = lue::Scalar<Element>;
    using Array = lue::PartitionedArray<Element, 2>;

    lue::api::Field const scalar = Scalar{};
    lue::api::Field const array = Array{};

    {
        lue::api::Field result = lue::api::add(scalar, array);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field result = scalar + array;

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_scalar_scalar)
{
    using Element = std::int32_t;
    using Scalar = lue::Scalar<Element>;

    lue::api::Field const scalar1 = Scalar{};
    lue::api::Field const scalar2 = Scalar{};

    {
        lue::api::Field result = lue::api::add(scalar1, scalar2);

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }

    {
        lue::api::Field result = scalar1 + scalar2;

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_array_value)
{
    using Element = std::int32_t;
    using Array = lue::PartitionedArray<Element, 2>;
    using Element = Element;

    lue::api::Field const array = Array{};
    lue::api::Field const value = Element{};

    {
        lue::api::Field result = lue::api::add(array, value);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field result = array + value;

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_value_array)
{
    using Element = std::int32_t;
    using Array = lue::PartitionedArray<std::int32_t, 2>;

    lue::api::Field const value = Element{};
    lue::api::Field const array = Array{};

    {
        lue::api::Field result = lue::api::add(value, array);

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }

    {
        lue::api::Field result = value + array;

        BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_scalar_value)
{
    using Element = std::int32_t;
    using Scalar = lue::Scalar<Element>;

    lue::api::Field const scalar = Scalar{};
    lue::api::Field const value = Element{};

    {
        lue::api::Field result = lue::api::add(scalar, value);

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }

    {
        lue::api::Field result = scalar + value;

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_value_scalar)
{
    using Element = std::int32_t;
    using Scalar = lue::Scalar<Element>;

    lue::api::Field const scalar = Scalar{};
    lue::api::Field const value = Element{};

    {
        lue::api::Field result = lue::api::add(value, scalar);

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }

    {
        lue::api::Field result = value + scalar;

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }
}


BOOST_AUTO_TEST_CASE(add_value_value)
{
    using Element = std::int32_t;
    using Scalar = lue::Scalar<Element>;

    lue::api::Field const value1 = Element{};
    lue::api::Field const value2 = Element{};

    {
        lue::api::Field result = lue::api::add(value1, value2);

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }

    {
        lue::api::Field result = value1 + value2;

        BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
    }
}
