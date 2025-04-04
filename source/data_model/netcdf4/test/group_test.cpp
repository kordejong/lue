#define BOOST_TEST_MODULE lue netcdf4 group
#include "lue/netcdf4/dataset.hpp"
#include "lue/netcdf4/group.hpp"
#include <boost/test/included/unit_test.hpp>


namespace {

    template<lue::Arithmetic T>
    void test_attribute(
        lue::netcdf::Group& group, std::string const& name, std::size_t const nr_values, T const* values)
    {
        BOOST_REQUIRE(!group.has_attribute(name));

        {
            auto const attribute = group.add_attribute(name, nr_values, values);
            BOOST_CHECK_EQUAL(attribute.type(), lue::netcdf::TypeTraits<T>::type_id);
            BOOST_REQUIRE_EQUAL(attribute.length(), nr_values);
            std::vector<T> values_read(nr_values);
            attribute.values(values_read.data());
            BOOST_CHECK_EQUAL_COLLECTIONS(values_read.begin(), values_read.end(), values, values + nr_values);
        }

        BOOST_REQUIRE(group.has_attribute(name));
        auto const attribute = group.attribute(name);
        BOOST_CHECK_EQUAL(attribute.type(), lue::netcdf::TypeTraits<T>::type_id);
        BOOST_REQUIRE_EQUAL(attribute.length(), nr_values);
        std::vector<T> values_read(nr_values);
        attribute.values(values_read.data());
        BOOST_CHECK_EQUAL_COLLECTIONS(values_read.begin(), values_read.end(), values, values + nr_values);
    }


    template<lue::Arithmetic T>
    void test_attribute(lue::netcdf::Group& group, std::string const& name, T const value)
    {
        test_attribute(group, name, 1, &value);
    }


    template<lue::Arithmetic T>
    void test_attribute(
        lue::netcdf::Group& group, std::string const& name, std::size_t const nr_values, T const value)
    {
        std::vector<T> const values(nr_values, value);
        test_attribute(group, name, nr_values, values.data());
    }

}  // Anonymous namespace


BOOST_AUTO_TEST_CASE(child)
{
    std::string const dataset_name = "group_child_group.nc";

    {
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
        auto group1 = dataset.add_child_group("child_group1");
        [[maybe_unused]] auto group2 = group1.add_child_group("child_group2");
    }

    {
        auto dataset = lue::netcdf::Dataset::open(dataset_name);

        BOOST_CHECK(dataset.has_child_group("child_group1"));
        BOOST_CHECK(!dataset.has_child_group("child_group2"));

        auto group1 = dataset.child_group("child_group1");

        BOOST_CHECK(!group1.has_child_group("child_group1"));
        BOOST_CHECK(group1.has_child_group("child_group2"));
    }
}


BOOST_AUTO_TEST_CASE(dimension)
{
    std::string const dataset_name = "group_dimension.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    {
        [[maybe_unused]] auto dimension1 = dataset.add_dimension("dimension1", 6);
        [[maybe_unused]] auto dimension2 = dataset.add_dimension("dimension2", 4);
        [[maybe_unused]] auto dimension3 = dataset.add_dimension("dimension3", NC_UNLIMITED);
    }

    {
        BOOST_REQUIRE_EQUAL(dataset.nr_dimensions(), 3);

        BOOST_REQUIRE(dataset.has_dimension("dimension1"));
        auto dimension1 = dataset.dimension("dimension1");
        BOOST_CHECK_EQUAL(dimension1.name(), "dimension1");
        BOOST_CHECK_EQUAL(dimension1.length(), 6);

        BOOST_REQUIRE(dataset.has_dimension("dimension2"));
        auto dimension2 = dataset.dimension("dimension2");
        BOOST_CHECK_EQUAL(dimension2.name(), "dimension2");
        BOOST_CHECK_EQUAL(dimension2.length(), 4);

        BOOST_REQUIRE(dataset.has_dimension("dimension3"));
        auto dimension3 = dataset.dimension("dimension3");
        BOOST_CHECK_EQUAL(dimension3.name(), "dimension3");
        BOOST_CHECK_EQUAL(dimension3.length(), NC_UNLIMITED);
    }
}


BOOST_AUTO_TEST_CASE(variable)
{
    std::string const dataset_name = "group_variable.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    // TODO Test all element types

    {
        auto dimension1 = dataset.add_dimension("dimension1", 6);
        auto dimension2 = dataset.add_dimension("dimension2", 4);
        auto variable1 = dataset.add_variable(
            "variable1", NC_INT, std::vector<lue::netcdf::Dimension>{dimension1, dimension2});
    }

    BOOST_REQUIRE(dataset.has_variable("variable1"));
    auto variable1 = dataset.variable("variable1");
    BOOST_CHECK_EQUAL(variable1.type(), NC_INT);

    auto variables = dataset.variables();
    BOOST_CHECK_EQUAL(variables.size(), 1);
    BOOST_CHECK_EQUAL(variables[0].name(), "variable1");
    BOOST_CHECK_EQUAL(variables[0].type(), NC_INT);

    auto dimensions = variable1.dimensions();
    BOOST_CHECK_EQUAL(dimensions.size(), 2);
    BOOST_CHECK_EQUAL(dimensions[0].name(), "dimension1");
    BOOST_CHECK_EQUAL(dimensions[1].name(), "dimension2");
}


BOOST_AUTO_TEST_CASE(attribute_scalar)
{
    std::string const dataset_name = "group_attribute.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    test_attribute(dataset, "attribute_int8", std::int8_t{-8});
    test_attribute(dataset, "attribute_uint8", std::uint8_t{8});
    test_attribute(dataset, "attribute_int32", std::int32_t{-32});
    test_attribute(dataset, "attribute_uint32", std::uint32_t{32});
    test_attribute(dataset, "attribute_int64", std::int64_t{-64});
    test_attribute(dataset, "attribute_uint64", std::uint64_t{64});
    test_attribute(dataset, "attribute_float", float{5.5});
    test_attribute(dataset, "attribute_double", double{6.6});

    // TODO string
}


BOOST_AUTO_TEST_CASE(attribute_array)
{
    std::string const dataset_name = "group_attribute.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    test_attribute(dataset, "attribute_int8", 5, std::int8_t{-8});
    test_attribute(dataset, "attribute_uint8", 5, std::uint8_t{8});
    test_attribute(dataset, "attribute_int32", 5, std::int32_t{-32});
    test_attribute(dataset, "attribute_uint32", 5, std::uint32_t{32});
    test_attribute(dataset, "attribute_int64", 5, std::int64_t{-64});
    test_attribute(dataset, "attribute_uint64", 5, std::uint64_t{64});
    test_attribute(dataset, "attribute_float", 5, float{5.5});
    test_attribute(dataset, "attribute_double", 5, double{6.6});

    // TODO string
}
