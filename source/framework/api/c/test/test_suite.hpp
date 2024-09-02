#pragma once
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include "lue/framework/api/c/runtime.h"
#include <boost/test/included/unit_test.hpp>


auto main(int const argc, char* argv[]) -> int
{
    // Use C-API for managing the runtime

    char* configuration[2] = {
        "hpx.run_hpx_main!=1",
        "hpx.commandline.allow_unknown!=1",
    };

    Runtime* runtime = construct_runtime(argc, argv, 2, configuration);

    bool const runtime_started = start_runtime(runtime);
    assert(runtime_started);

    int test_status{boost::exit_success};

    if (on_root_locality())
    {
        int argc_new;
        char** argv_new;
        sieve_hpx_arguments(argc, argv, &argc_new, &argv_new);
        test_status = boost::unit_test::unit_test_main(init_unit_test, argc_new, argv_new);
    }

    int const hpx_status = stop_runtime(runtime);

    destruct_runtime(runtime);

    return test_status != boost::exit_success ? test_status : hpx_status;
}
