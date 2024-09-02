#include "lue/framework/api/c/runtime.h"
#include "c_runtime.hpp"
#include <cassert>


namespace {

    inline auto as_cxx_runtime_ptr(Runtime const* runtime) -> lue::api::CRuntime const*
    {
        assert(runtime);
        assert(runtime->instance);

        return static_cast<lue::api::CRuntime const*>(runtime->instance);
    }


    inline auto as_cxx_runtime_ptr(Runtime* runtime) -> lue::api::CRuntime*
    {
        assert(runtime);
        assert(runtime->instance);

        return static_cast<lue::api::CRuntime*>(runtime->instance);
    }


    inline auto as_cxx_runtime(Runtime const* runtime) -> lue::api::CRuntime const&
    {
        return *as_cxx_runtime_ptr(runtime);
    }


    inline auto as_cxx_runtime(Runtime* runtime) -> lue::api::CRuntime&
    {
        return *as_cxx_runtime_ptr(runtime);
    }

}  // Anonymous namespace


auto construct_runtime(int const argc, char* argv[], int const nr_items, char* configuration_items[])
    -> Runtime*
{

    std::vector<std::string> configuration_(configuration_items, configuration_items + nr_items);

    return new Runtime{.instance = new lue::api::CRuntime{argc, argv, configuration_}};
}


void destruct_runtime(Runtime* runtime)
{
    if (runtime != nullptr)
    {
        if (runtime->instance != nullptr)
        {
            delete as_cxx_runtime_ptr(runtime);
        }

        delete runtime;
    }
}


bool start_runtime(Runtime* runtime)
{
    return as_cxx_runtime(runtime).start();
}


int stop_runtime(Runtime* runtime)
{
    return as_cxx_runtime(runtime).stop();
}


bool on_root_locality()
{
    return lue::api::Runtime::on_root_locality();
}


void sieve_hpx_arguments(int argc, char* argv[], int* argc_new, char*** argv_new)
{
    std::tie(*argc_new, *argv_new) = lue::api::Runtime::sieve_hpx_arguments(argc, argv);
}
