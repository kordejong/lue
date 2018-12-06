#pragma once
#include "lue/framework/benchmark/environment.hpp"
#include "lue/framework/benchmark/format.hpp"
#include "lue/framework/core/configuration_entry.hpp"
#include <hpx/hpx_finalize.hpp>
#include <hpx/hpx_init.hpp>
#include <fstream>
#include <iostream>


namespace lue {
namespace benchmark {
namespace detail {

/*!
    @brief      Return an object representing the environment in which the
                benchmark is run
*/
Environment create_environment()
{
    std::uint64_t const count =
        required_configuration_entry<std::uint64_t>("benchmark.count");
    std::uint64_t const nr_localities =
        required_configuration_entry<std::uint64_t>("hpx", "localities");
    std::uint64_t const nr_threads =
        required_configuration_entry<std::uint64_t>("hpx", "os_threads");
    std::uint64_t const work_size =
        required_configuration_entry<std::uint64_t>("benchmark.work_size");

    return Environment{count, nr_localities, nr_threads, work_size};
}

} // Namespace detail


template<
    typename Benchmark>
inline int run_hpx_benchmark(
    Benchmark& benchmark,
    std::ostream& stream)
{
    int status = benchmark.run();

    if(status != EXIT_SUCCESS) {
        std::cerr << "Running benchmark failed" << std::endl;
        return EXIT_FAILURE;
    }

    status = hpx::finalize();

    if(status != 0) {
        std::cerr << "Stopping HPX failed" << std::endl;
        return EXIT_FAILURE;
    }

    stream << format_as_json(benchmark) << "\n";

    return EXIT_SUCCESS;
}


#define LUE_CONFIGURE_HPX_BENCHMARK( \
    configuration)                                          \
int hpx_main(                                                                  \
    int argc,                                                                  \
    char* argv[])                                                              \
{                                                                              \
    std::string const pathname =                                               \
        lue::optional_configuration_entry<std::string>(                        \
            "benchmark.output", "");                                           \
                                                                               \
    auto const environment{                                                    \
        lue::benchmark::detail::create_environment()};                         \
                                                                               \
    auto benchmark = setup_benchmark(argc, argv, environment);                 \
                                                                               \
    int status = EXIT_FAILURE;                                                 \
                                                                               \
    if(!pathname.empty()) {                                                    \
        std::fstream stream;                                                   \
        stream.open(pathname, std::ios::out);                                  \
        status = run_hpx_benchmark(benchmark, stream);                         \
    }                                                                          \
    else {                                                                     \
        status = run_hpx_benchmark(benchmark, std::cout);                      \
    }                                                                          \
                                                                               \
    return status;                                                             \
}                                                                              \
                                                                               \
                                                                               \
int main(                                                                      \
    int argc,                                                                  \
    char* argv[])                                                              \
{                                                                              \
    std::vector<std::string> cfg = {                                           \
        configuration                                                          \
    };                                                                         \
                                                                               \
    return hpx::init(argc, argv, cfg);                                         \
}

}  // namespace benchmark
}  // namespace lue
