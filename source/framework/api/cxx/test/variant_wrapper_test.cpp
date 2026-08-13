#define BOOST_TEST_MODULE lue framework api cxx variant_wrapper
#include "unit_test.hpp"
#include "lue/framework/api/cxx/variant_wrapper.hpp"


BOOST_AUTO_TEST_CASE(is_one_of)
{
    using MyVariant = std::variant<int, float, std::string>;

    static_assert(lue::api::detail::is_one_of<int, MyVariant>::value);
    static_assert(lue::api::detail::is_one_of<float, MyVariant>::value);
    static_assert(lue::api::detail::is_one_of<std::string, MyVariant>::value);

    static_assert(!lue::api::detail::is_one_of<bool, MyVariant>::value);
    static_assert(!lue::api::detail::is_one_of<double, MyVariant>::value);
}


BOOST_AUTO_TEST_CASE(is_allowed)
{
    using MyWrapper = lue::api::VariantWrapper<int, float, std::string>;

    static_assert(MyWrapper::is_allowed_v<int>);
    static_assert(MyWrapper::is_allowed_v<float>);
    static_assert(MyWrapper::is_allowed_v<std::string>);

    static_assert(!MyWrapper::is_allowed_v<bool>);
    static_assert(!MyWrapper::is_allowed_v<int const>);
    static_assert(!MyWrapper::is_allowed_v<float*>);
    static_assert(!MyWrapper::is_allowed_v<double>);
}


BOOST_AUTO_TEST_CASE(implicit_conversion)
{
    using MyWrapper = lue::api::VariantWrapper<int, float, std::string>;

    {
        MyWrapper wrapped_int{5};
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        MyWrapper wrapped_int = 5;
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int five{5};
        MyWrapper wrapped_int{five};
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int five{5};
        MyWrapper wrapped_int = five;
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int const five{5};
        MyWrapper wrapped_int{five};
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int const five{5};
        MyWrapper wrapped_int = five;
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int five{5};
        int& five_ref{five};
        MyWrapper wrapped_int{five_ref};
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int five{5};
        int& five_ref{five};
        MyWrapper wrapped_int = five_ref;
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int const five{5};
        int const& five_const_ref{five};
        MyWrapper wrapped_int{five_const_ref};
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }

    {
        int const five{5};
        int const& five_const_ref{five};
        MyWrapper wrapped_int = five_const_ref;
        BOOST_CHECK(std::holds_alternative<int>(wrapped_int.variant()));
    }
}
