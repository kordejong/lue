#include "lue/framework/algorithm/value_policies/convolve.hpp"
#incluce <cxxopts.hpp>
#include <hpx/hpx_main.hpp>


auto main(int argc, char** argv) -> int
{
    cxxopts::Options options{"convolve_example-1"};

    options.add_options()
        ("argument_array", "Array to read", cxxopts::value<std::string>())
        ("argument_kernel", "Kernel to read", cxxopts::value<std::string>())
        ("result_array", "Array to write", cxxopts::value<std::string>())
        ;
    options.parse_posititional({"argument_array", "argument_kernel", "result_array"});
    auto result = options.parse(argc, argv);

    std::string const argument_array_pathname{result["argument_array"]};
    std::string const argument_kernel_pathname{result["argument_kernel"]};
    std::string const result_array_pathname{result["result_array"]};

    lue::Rank const rank{2};

    using Element = float;
    using Array = lue::PartitionedArray<Element, rank>;
    using Kernel = lue::Kernel<Element, rank>;

    Kernel const kernel = lue::from_gdal(argument_kernel_pathname);
    Array const array = lue::from_gdal(argument_array_pathname);
    Array const result = lue::value_policies::convolve(array, kernel);

    lue::to_gdal(result, result_array_pathname);

    return EXIT_SUCCESS;
}
