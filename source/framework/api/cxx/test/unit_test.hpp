#pragma once
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <hpx/hpx_init.hpp>
#include <boost/test/included/unit_test.hpp>


namespace lue::detail {

    auto run_unit_tests(int argc, char** argv) -> int
    {
        // init_unit_test is undefined here, but once BOOST_TEST_MODULE is set (in a unit test module),
        // it will be defined
        int test_status = boost::unit_test::unit_test_main(init_unit_test, argc, argv);
        int hpx_status = hpx::finalize();

        return test_status != boost::exit_success ? test_status : hpx_status;
    }

}  // namespace lue::detail


auto main(int argc, char** argv) -> int
{
    // Make sure that processes in tests that depend on GDAL drivers have access to the GDAL drivers
    // lue::gdal::register_gdal_drivers();

    return hpx::init(&lue::detail::run_unit_tests, argc, argv);
}
