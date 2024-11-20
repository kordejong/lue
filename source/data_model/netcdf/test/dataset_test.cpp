#define BOOST_TEST_MODULE lue netcdf dataset
#include "lue/netcdf/dataset.hpp"
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(create)
{
    BOOST_CHECK(true);

    // // - Create, with clobber → should succeed
    // // - Create, without clobber (same dataset) → should fail
    // std::string const dataset_name = "my_dataset.nc";
}


BOOST_AUTO_TEST_CASE(open_)
{
    BOOST_CHECK(true);

    // std::string const dataset_name = "my_dataset.nc";

    // auto const dataset = lue::netcdf::Dataset::open(dataset_name);
}
