#pragma once
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include "lue/framework/api/cxx/runtime.hpp"
#include <boost/test/included/unit_test.hpp>


// This custom test suite exists because we need to be able to parse and edit the command line argument vector
// before Boost UTF gets it. Otherwise the default main will trip over the HPX-specific command line
// arguments. This happens when using a fixture to start / stop the runtime.


auto main(int const argc, char* argv[]) -> int
{
    // Use C++-API for managing the runtime

    std::vector<std::string> configuration{
        "hpx.run_hpx_main!=1",
        "hpx.commandline.allow_unknown!=1",
    };

    lue::api::Runtime runtime{argc, argv, configuration};

    bool const runtime_started = runtime.start();
    assert(runtime_started);

    int test_status{boost::exit_success};

    if (lue::api::Runtime::on_root_locality())
    {
        auto [argc_new, argv_new] = lue::api::Runtime::sieve_hpx_arguments(argc, argv);
        test_status = boost::unit_test::unit_test_main(init_unit_test, argc_new, argv_new);
    }

    int const hpx_status = runtime.stop();

    return test_status != boost::exit_success ? test_status : hpx_status;
}
