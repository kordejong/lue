#define BOOST_TEST_MODULE lue netcdf4 variable
#include "lue/netcdf4/dataset.hpp"
#include "lue/netcdf4/variable.hpp"
#include <boost/test/included/unit_test.hpp>
#include <numeric>


namespace {

    template<lue::Arithmetic T>
    void test_scalar(lue::netcdf::Group& group, std::string const& name, T const value)
    {
        auto variable = group.add_variable(name, lue::netcdf::TypeTraits<T>::type_id);

        {
            // References
            T const value_written{value};
            variable.write(value_written);

            T value_read{};
            variable.read(value_read);

            BOOST_CHECK_EQUAL(value_written, value_read);
        }

        {
            // Pointers
            T const value_written{value};
            variable.write(&value_written);

            T value_read{};
            variable.read(&value_read);

            BOOST_CHECK_EQUAL(value_written, value_read);
        }
    }


    template<lue::Arithmetic T>
    void test_array_1d(lue::netcdf::Group& group, std::string const& name)
    {
        std::size_t const nr_elements{6};
        auto dimension = group.add_dimension(std::format("{}_dimension", name), nr_elements);
        auto variable = group.add_variable(
            name, lue::netcdf::TypeTraits<T>::type_id, std::vector<lue::netcdf::Dimension>{dimension});

        std::vector<T> values_written(nr_elements);
        std::iota(values_written.begin(), values_written.end(), 0);
        variable.write(values_written.data());

        std::vector<T> values_read(nr_elements);
        variable.read(values_read.data());

        BOOST_CHECK_EQUAL_COLLECTIONS(
            values_read.begin(), values_read.end(), values_written.begin(), values_written.end());
    }


    template<lue::Arithmetic T>
    void test_array_2d(lue::netcdf::Group& group, std::string const& name)
    {
        std::size_t const nr_elements1{6};
        std::size_t const nr_elements2{4};
        std::size_t const nr_elements{nr_elements1 * nr_elements2};
        auto dimension1 = group.add_dimension(std::format("{}_dimension1", name), nr_elements1);
        auto dimension2 = group.add_dimension(std::format("{}_dimension2", name), nr_elements2);
        auto variable = group.add_variable(
            name,
            lue::netcdf::TypeTraits<T>::type_id,
            std::vector<lue::netcdf::Dimension>{dimension1, dimension2});

        std::vector<T> values_written(nr_elements);
        std::iota(values_written.begin(), values_written.end(), 0);
        variable.write(values_written.data());

        std::vector<T> values_read(nr_elements);
        variable.read(values_read.data());

        BOOST_CHECK_EQUAL_COLLECTIONS(
            values_read.begin(), values_read.end(), values_written.begin(), values_written.end());
    }


    template<lue::Arithmetic T>
    void test_hyperslab_2d(lue::netcdf::Group& group, std::string const& name)
    {
        std::size_t const nr_elements1{6};
        std::size_t const nr_elements2{4};
        auto dimension1 = group.add_dimension(std::format("{}_dimension1", name), nr_elements1);
        auto dimension2 = group.add_dimension(std::format("{}_dimension2", name), nr_elements2);
        auto variable = group.add_variable(
            name,
            lue::netcdf::TypeTraits<T>::type_id,
            std::vector<lue::netcdf::Dimension>{dimension1, dimension2});

        {
            // First row
            std::vector<T> row_written(nr_elements2);
            std::iota(row_written.begin(), row_written.end(), 0);
            variable.write(lue::netcdf::Hyperslab{{0, 0}, {1, nr_elements2}}, row_written.data());

            std::vector<T> row_read(nr_elements2);
            variable.read(lue::netcdf::Hyperslab{{0, 0}, {1, nr_elements2}}, row_read.data());

            BOOST_CHECK_EQUAL_COLLECTIONS(
                row_read.begin(), row_read.end(), row_written.begin(), row_written.end());
        }

        {
            // Last two rows
            std::vector<T> rows_written(2 * nr_elements2);
            std::iota(rows_written.begin(), rows_written.end(), 0);
            variable.write(lue::netcdf::Hyperslab{{nr_elements2, 0}, {2, nr_elements2}}, rows_written.data());

            std::vector<T> rows_read(2 * nr_elements2);
            variable.read(lue::netcdf::Hyperslab{{nr_elements2, 0}, {2, nr_elements2}}, rows_read.data());

            BOOST_CHECK_EQUAL_COLLECTIONS(
                rows_read.begin(), rows_read.end(), rows_written.begin(), rows_written.end());
        }

        {
            // Second column
            std::vector<T> column_written{1, 5, 9, 13, 17, 21};
            variable.write(
                lue::netcdf::Hyperslab{{0, 1}, {nr_elements1, 1}, {1, nr_elements2}}, column_written.data());

            std::vector<T> column_read(nr_elements1);
            variable.read(
                lue::netcdf::Hyperslab{{0, 1}, {nr_elements1, 1}, {1, nr_elements2}}, column_read.data());

            BOOST_CHECK_EQUAL_COLLECTIONS(
                column_read.begin(), column_read.end(), column_written.begin(), column_written.end());
        }
    }

}  // Anonymous namespace


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

    test_scalar(dataset, "variable_int8", std::int8_t{-8});
    test_scalar(dataset, "variable_uint8", std::uint8_t{8});
    test_scalar(dataset, "variable_int32", std::int32_t{-32});
    test_scalar(dataset, "variable_uint32", std::uint32_t{32});
    test_scalar(dataset, "variable_int64", std::int64_t{-64});
    test_scalar(dataset, "variable_uint64", std::uint64_t{64});
    test_scalar(dataset, "variable_float", float{5.5});
    test_scalar(dataset, "variable_double", double{6.6});

    // TODO string
}


BOOST_AUTO_TEST_CASE(array_1d)
{
    std::string const dataset_name = "variable_array_1d.nc";
    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    test_array_1d<std::int8_t>(dataset, "variable_int8");
    test_array_1d<std::uint8_t>(dataset, "variable_uint8");
    test_array_1d<std::int32_t>(dataset, "variable_int32");
    test_array_1d<std::uint32_t>(dataset, "variable_uint32");
    test_array_1d<std::int64_t>(dataset, "variable_int64");
    test_array_1d<std::uint64_t>(dataset, "variable_uint64");
    test_array_1d<float>(dataset, "variable_float");
    test_array_1d<double>(dataset, "variable_double");

    // TODO string
}


BOOST_AUTO_TEST_CASE(array_2d)
{
    std::string const dataset_name = "variable_array_2d.nc";
    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    test_array_2d<std::int8_t>(dataset, "variable_int8");
    test_array_2d<std::uint8_t>(dataset, "variable_uint8");
    test_array_2d<std::int32_t>(dataset, "variable_int32");
    test_array_2d<std::uint32_t>(dataset, "variable_uint32");
    test_array_2d<std::int64_t>(dataset, "variable_int64");
    test_array_2d<std::uint64_t>(dataset, "variable_uint64");
    test_array_2d<float>(dataset, "variable_float");
    test_array_2d<double>(dataset, "variable_double");

    // TODO string
}


BOOST_AUTO_TEST_CASE(hyperslab_2d)
{
    std::string const dataset_name = "variable_hyperslab_2d.nc";
    auto dataset = lue::netcdf::Dataset::create(dataset_name, NC_CLOBBER);

    test_hyperslab_2d<std::int8_t>(dataset, "variable_int8");
    test_hyperslab_2d<std::uint8_t>(dataset, "variable_uint8");
    test_hyperslab_2d<std::int32_t>(dataset, "variable_int32");
    test_hyperslab_2d<std::uint32_t>(dataset, "variable_uint32");
    test_hyperslab_2d<std::int64_t>(dataset, "variable_int64");
    test_hyperslab_2d<std::uint64_t>(dataset, "variable_uint64");
    test_hyperslab_2d<float>(dataset, "variable_float");
    test_hyperslab_2d<double>(dataset, "variable_double");

    // TODO string
}
