#define BOOST_TEST_MODULE lue netcdf4 variable
#include "lue/netcdf4/dataset.hpp"
#include "lue/netcdf4/variable.hpp"
#include <boost/test/included/unit_test.hpp>
#include <numeric>


BOOST_AUTO_TEST_CASE(fill_value)
{
    std::string const dataset_name = "variable_fill_value.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
    auto variable = dataset.add_variable("variable", NC_INT);
    BOOST_CHECK(variable.has_fill_value());
    BOOST_CHECK(variable.fill_value<int>() != 5);
    variable.set_fill_value<int>(5);
    BOOST_CHECK_EQUAL(variable.fill_value<int>(), 5);
}


BOOST_AUTO_TEST_CASE(scalar)
{
    std::string const dataset_name = "variable_scalar.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
    auto variable = dataset.add_variable("variable", NC_INT);

    {
        // References
        std::int32_t const value_written{55};
        variable.write(value_written);

        std::int32_t value_read{};
        variable.read(value_read);

        BOOST_CHECK_EQUAL(value_written, value_read);
    }

    {
        // Pointers
        std::int32_t const value_written{66};
        variable.write(&value_written);

        std::int32_t value_read{};
        variable.read(&value_read);

        BOOST_CHECK_EQUAL(value_written, value_read);
    }
}


BOOST_AUTO_TEST_CASE(array_1d)
{
    std::string const dataset_name = "variable_array_1d.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
    auto dimension = dataset.add_dimension("dimension", 6);
    auto variable = dataset.add_variable("variable", NC_INT, std::vector<lue::netcdf::Dimension>{dimension});

    std::vector<std::int32_t> values_written(6);
    std::iota(values_written.begin(), values_written.end(), 0);
    variable.write(values_written.data());

    std::vector<std::int32_t> values_read(6);
    variable.read(values_read.data());

    BOOST_CHECK_EQUAL_COLLECTIONS(
        values_written.begin(), values_written.end(), values_read.begin(), values_read.end());
}


BOOST_AUTO_TEST_CASE(array_2d)
{
    std::string const dataset_name = "variable_array_2d.nc";

    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);
    auto dimension1 = dataset.add_dimension("dimension1", 6);
    auto dimension2 = dataset.add_dimension("dimension2", 4);
    auto variable =
        dataset.add_variable("variable", NC_INT, std::vector<lue::netcdf::Dimension>{dimension1, dimension2});

    std::vector<std::int32_t> values_written(6 * 4);
    std::iota(values_written.begin(), values_written.end(), 0);
    variable.write(values_written.data());

    std::vector<std::int32_t> values_read(6 * 4);
    variable.read(values_read.data());

    BOOST_CHECK_EQUAL_COLLECTIONS(
        values_written.begin(), values_written.end(), values_read.begin(), values_read.end());
}
