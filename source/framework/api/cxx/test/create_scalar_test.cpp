#define BOOST_TEST_MODULE lue framework api cxx create_scalar
#include "test_suite.hpp"
#include "lue/framework/algorithm/scalar.hpp"
#include "lue/framework/api/cxx/create_scalar.hpp"


BOOST_AUTO_TEST_CASE(create_scalar)
{
    using Element = std::int32_t;
    using Scalar = lue::Scalar<Element>;

    lue::api::Literal const value = Element{};
    lue::api::Field const result = lue::api::create_scalar(value);

    BOOST_CHECK(std::holds_alternative<Scalar>(result.variant()));
}
