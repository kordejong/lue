#define BOOST_TEST_MODULE lue netcdf4 dataset
#include "lue/netcdf4/dataset.hpp"
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(create)
{
    // - Create, with clobber → should succeed
    // - Create, without clobber (same dataset) → should fail
    std::string const dataset_name = "dataset_create.nc";

    auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);


    // TODO Assert exception thrown
    // auto dataset2 = lue::netcdf4::Dataset::create(dataset_name, NC_NOCLOBBER | NC_DISKLESS);
}


BOOST_AUTO_TEST_CASE(open_readable)
{
    std::string const dataset_name = "dataset_open_readable.nc";

    {
        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
    }

    auto const dataset = lue::netcdf4::Dataset::open(dataset_name, NC_NOWRITE);
}


BOOST_AUTO_TEST_CASE(open_writable)
{
    std::string const dataset_name = "dataset_open_writable.nc";

    {
        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
    }

    auto const dataset = lue::netcdf4::Dataset::open(dataset_name, NC_WRITE);
}


BOOST_AUTO_TEST_CASE(name)
{
    {
        std::string const dataset_name = "dataset_name1.nc";
        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
        BOOST_CHECK_EQUAL(dataset.name(), "/");
        BOOST_CHECK_EQUAL(dataset.full_name(), "/");
        BOOST_CHECK_EQUAL(dataset.path(), dataset_name);
    }

    {
        std::string const dataset_name = "./dataset_name2.nc";
        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
        BOOST_CHECK_EQUAL(dataset.name(), "/");
        BOOST_CHECK_EQUAL(dataset.full_name(), "/");
        BOOST_CHECK_EQUAL(dataset.path(), dataset_name);
    }

    {
        std::string const dataset_name = "dætæsêt_ñømê3.nc";
        auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
        BOOST_CHECK_EQUAL(dataset.name(), "/");
        BOOST_CHECK_EQUAL(dataset.full_name(), "/");
        BOOST_CHECK_EQUAL(dataset.path(), dataset_name);
    }
}


BOOST_AUTO_TEST_CASE(conventions)
{
    // Write conventions attribute and read back in. Various cases.
    std::string const dataset_name = "dataset_conventions.nc";

    auto dataset = lue::netcdf4::Dataset::create(dataset_name, NC_CLOBBER);
    std::vector<std::string> conventions_we_want{};
    std::vector<std::string> conventions_we_got{};

    // Default / empty
    conventions_we_got = dataset.conventions();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        conventions_we_got.begin(),
        conventions_we_got.end(),
        conventions_we_want.begin(),
        conventions_we_want.end());

    // Empty value
    dataset.set_conventions({""});
    conventions_we_want = {};
    conventions_we_got = dataset.conventions();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        conventions_we_got.begin(),
        conventions_we_got.end(),
        conventions_we_want.begin(),
        conventions_we_want.end());

    // Single value
    dataset.set_conventions({"MyConvention"});
    conventions_we_want = {"MyConvention"};
    conventions_we_got = dataset.conventions();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        conventions_we_got.begin(),
        conventions_we_got.end(),
        conventions_we_want.begin(),
        conventions_we_want.end());

    // Value with whitespace front / back
    dataset.set_conventions({" 	 MyConvention  	"});  // Note the two tabs
    conventions_we_want = {"MyConvention"};
    conventions_we_got = dataset.conventions();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        conventions_we_got.begin(),
        conventions_we_got.end(),
        conventions_we_want.begin(),
        conventions_we_want.end());

    // Space separated list
    dataset.set_conventions({"MyConvention1", "MyConvention2"});
    conventions_we_want = {"MyConvention1", "MyConvention2"};
    conventions_we_got = dataset.conventions();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        conventions_we_got.begin(),
        conventions_we_got.end(),
        conventions_we_want.begin(),
        conventions_we_want.end());

    // Comma separated list
    dataset.set_conventions({"My Convention1", "My Convention2"});
    conventions_we_want = {"My Convention1", "My Convention2"};
    conventions_we_got = dataset.conventions();
    BOOST_CHECK_EQUAL_COLLECTIONS(
        conventions_we_got.begin(),
        conventions_we_got.end(),
        conventions_we_want.begin(),
        conventions_we_want.end());
}
