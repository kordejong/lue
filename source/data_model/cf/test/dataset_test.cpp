#define BOOST_TEST_MODULE lue cf dataset
#include "lue/cf/dataset.hpp"
#include "lue/version.hpp"
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(create)
{
    // - Create, with clobber → should succeed
    // - Create, without clobber (same dataset) → should fail
    std::string const dataset_name = "dataset_create.nc";

    auto dataset = lue::cf::Dataset::create(dataset_name, NC_CLOBBER);

    BOOST_REQUIRE(dataset.has_attribute("Conventions"));
    BOOST_CHECK_EQUAL(dataset.attribute("Conventions").value(), "CF-1.11");
    BOOST_CHECK_EQUAL(dataset.version(), 1.11);

    BOOST_REQUIRE(dataset.has_attribute("history"));
    BOOST_CHECK_EQUAL(dataset.attribute("history").value(), std::format("LUE-{}", lue::version()));

    // // TODO Assert exception thrown
    // // auto dataset2 = lue::netcdf4::Dataset::create(dataset_name, NC_NOCLOBBER | NC_DISKLESS);
}


BOOST_AUTO_TEST_CASE(open_readable)
{
    // std::string const dataset_name = "dataset_open_readable.nc";

    // {
    //     auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
    // }

    // auto const dataset = lue::netcdf4::Dataset::open(dataset_name, NC_NOWRITE);
}


BOOST_AUTO_TEST_CASE(open_writable)
{
    // std::string const dataset_name = "dataset_open_writable.nc";

    // {
    //     auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
    // }

    // auto const dataset = lue::netcdf4::Dataset::open(dataset_name, NC_WRITE);
}
