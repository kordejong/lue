#define BOOST_TEST_MODULE lue netcdf4 dataset
#include "lue/netcdf4/dataset.hpp"
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(create)
{
    // - Create, with clobber → should succeed
    // - Create, without clobber (same dataset) → should fail
    std::string const dataset_name = "dataset_create.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);


    // TODO Assert exception thrown
    // auto dataset2 = lue::netcdf::Dataset::create(dataset_name, NC_NOCLOBBER | NC_DISKLESS);
}


BOOST_AUTO_TEST_CASE(open_readable)
{
    std::string const dataset_name = "dataset_open_readable.nc";

    {
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
    }

    auto const dataset = lue::netcdf::Dataset::open(dataset_name, NC_NOWRITE);
}


BOOST_AUTO_TEST_CASE(open_writable)
{
    std::string const dataset_name = "dataset_open_writable.nc";

    {
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
    }

    auto const dataset = lue::netcdf::Dataset::open(dataset_name, NC_WRITE);
}


BOOST_AUTO_TEST_CASE(name)
{
    {
        std::string const dataset_name = "dataset_name1.nc";
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
        BOOST_CHECK_EQUAL(dataset.name(), "/");
        BOOST_CHECK_EQUAL(dataset.full_name(), "/");
        BOOST_CHECK_EQUAL(dataset.path(), dataset_name);
    }

    {
        std::string const dataset_name = "./dataset_name2.nc";
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
        BOOST_CHECK_EQUAL(dataset.name(), "/");
        BOOST_CHECK_EQUAL(dataset.full_name(), "/");
        BOOST_CHECK_EQUAL(dataset.path(), dataset_name);
    }

    {
        std::string const dataset_name = "dætæsêt_ñømê3.nc";
        auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
        BOOST_CHECK_EQUAL(dataset.name(), "/");
        BOOST_CHECK_EQUAL(dataset.full_name(), "/");
        BOOST_CHECK_EQUAL(dataset.path(), dataset_name);
    }
}
