#define BOOST_TEST_MODULE lue netcdf4 group
#include "lue/netcdf4/dataset.hpp"
#include "lue/netcdf4/group.hpp"
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(sub_group)
{
    std::string const dataset_name = "group_sub_group.nc";

    {
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
        auto group1 = dataset.add_sub_group("sub_group1");
        [[maybe_unused]] auto group2 = group1.add_sub_group("sub_group2");
    }

    {
        auto dataset = lue::netcdf::Dataset::open(dataset_name);

        BOOST_CHECK(dataset.has_sub_group("sub_group1"));
        BOOST_CHECK(!dataset.has_sub_group("sub_group2"));

        auto group1 = dataset.sub_group("sub_group1");

        BOOST_CHECK(!group1.has_sub_group("sub_group1"));
        BOOST_CHECK(group1.has_sub_group("sub_group2"));
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


BOOST_AUTO_TEST_CASE(attribute)
{
    std::string const dataset_name = "group_attribute.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    BOOST_REQUIRE(!dataset.has_attribute("attribute_int32"));

    {
        auto attribute_int32 = dataset.add_attribute("attribute_int32", std::int32_t{55});
        BOOST_CHECK_EQUAL(attribute_int32.type(), NC_INT);
        BOOST_CHECK_EQUAL(attribute_int32.value<std::int32_t>(), 55);
    }

    BOOST_REQUIRE(dataset.has_attribute("attribute_int32"));
    auto attribute_int32 = dataset.attribute("attribute_int32");
    BOOST_CHECK_EQUAL(attribute_int32.type(), NC_INT);
    BOOST_CHECK_EQUAL(attribute_int32.value<std::int32_t>(), 55);
}
