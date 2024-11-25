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

    // auto variable = dataset.add_variable("variable1", NC_INT, dimensions);
}
