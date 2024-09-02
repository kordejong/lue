#define BOOST_TEST_MODULE lue framework api cxx io
#include "test_suite.hpp"
#include "lue/framework/api/cxx/create_array.hpp"
#include "lue/framework/api/cxx/io.hpp"
#include "lue/gdal.hpp"


namespace {

    struct Fixture
    {

            Fixture()
            {
                lue::gdal::register_gdal_drivers();
            }
    };

}  // Anonymous namespace


BOOST_FIXTURE_TEST_CASE(round_trip, Fixture)
{
    using Element = std::int32_t;
    using Array = lue::PartitionedArray<Element, 2>;
    using Shape = lue::ShapeT<Array>;

    Shape const array_shape{1, 1};
    Shape const partition_shape{1, 1};

    lue::api::Field const fill_value = Element{};

    lue::api::Field field = lue::api::create_array(array_shape, partition_shape, fill_value);

    BOOST_CHECK(std::holds_alternative<Array>(field.variant()));

    std::string const name{"api_cxx_io_round_trip.tif"};

    lue::api::to_gdal(field, name).wait();

    lue::api::Field result = lue::api::from_gdal(name);

    BOOST_CHECK(std::holds_alternative<Array>(result.variant()));
}
