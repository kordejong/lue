#define BOOST_TEST_MODULE lue netcdf4 attribute
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dataset.hpp"
#include <boost/test/included/unit_test.hpp>


namespace {

    template<lue::Arithmetic T>
    void test_array(
        lue::netcdf4::Group& group, std::string const& name, std::size_t const nr_values, T const* values)
    {
        BOOST_REQUIRE(!group.has_attribute(name));

        {
            auto const attribute = group.add_attribute(name, nr_values, values);
            BOOST_CHECK_EQUAL(attribute.type(), lue::netcdf4::TypeTraits<T>::type_id);
            BOOST_REQUIRE_EQUAL(attribute.length(), nr_values);
            std::vector<T> values_read(nr_values);
            attribute.values(values_read.data());
            BOOST_CHECK_EQUAL_COLLECTIONS(values_read.begin(), values_read.end(), values, values + nr_values);
        }

        BOOST_REQUIRE(group.has_attribute(name));
        auto const attribute = group.attribute(name);
        BOOST_CHECK_EQUAL(attribute.type(), lue::netcdf4::TypeTraits<T>::type_id);
        BOOST_REQUIRE_EQUAL(attribute.length(), nr_values);
        std::vector<T> values_read(nr_values);
        attribute.values(values_read.data());
        BOOST_CHECK_EQUAL_COLLECTIONS(values_read.begin(), values_read.end(), values, values + nr_values);
    }


    template<lue::Arithmetic T>
    void test_scalar(lue::netcdf4::Group& group, std::string const& name, T const value)
    {
        BOOST_REQUIRE(!group.has_attribute(name));

        {
            auto const attribute = group.add_attribute(name, value);
            BOOST_CHECK_EQUAL(attribute.type(), lue::netcdf4::TypeTraits<T>::type_id);
            BOOST_REQUIRE_EQUAL(attribute.length(), 1);
            auto const value_read = attribute.template value<T>();
            BOOST_CHECK_EQUAL(value_read, value);
        }

        BOOST_REQUIRE(group.has_attribute(name));
        auto const attribute = group.attribute(name);
        BOOST_CHECK_EQUAL(attribute.type(), lue::netcdf4::TypeTraits<T>::type_id);
        BOOST_REQUIRE_EQUAL(attribute.length(), 1);
        auto const value_read = attribute.template value<T>();
        BOOST_CHECK_EQUAL(value_read, value);
    }


    template<lue::Arithmetic T>
    void test_array(
        lue::netcdf4::Group& group, std::string const& name, std::size_t const nr_values, T const value)
    {
        std::vector<T> const values(nr_values, value);
        test_array(group, name, nr_values, values.data());
    }

}  // Anonymous namespace


BOOST_AUTO_TEST_CASE(scalar)
{
    std::string const dataset_name = "attribute_scalar.nc";
    auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);

    test_scalar(dataset, "attribute_int8", std::int8_t{-8});
    test_scalar(dataset, "attribute_uint8", std::uint8_t{8});
    test_scalar(dataset, "attribute_int32", std::int32_t{-32});
    test_scalar(dataset, "attribute_uint32", std::uint32_t{32});
    test_scalar(dataset, "attribute_int64", std::int64_t{-64});
    test_scalar(dataset, "attribute_uint64", std::uint64_t{64});
    test_scalar(dataset, "attribute_float", float{5.5});
    test_scalar(dataset, "attribute_double", double{6.6});

    // TODO string
}


BOOST_AUTO_TEST_CASE(array)
{
    std::string const dataset_name = "attribute_array.nc";
    auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);

    test_array(dataset, "attribute_int8", 5, std::int8_t{-8});
    test_array(dataset, "attribute_uint8", 5, std::uint8_t{8});
    test_array(dataset, "attribute_int32", 5, std::int32_t{-32});
    test_array(dataset, "attribute_uint32", 5, std::uint32_t{32});
    test_array(dataset, "attribute_int64", 5, std::int64_t{-64});
    test_array(dataset, "attribute_uint64", 5, std::uint64_t{64});
    test_array(dataset, "attribute_float", 5, float{5.5});
    test_array(dataset, "attribute_double", 5, double{6.6});

    // TODO string
}
