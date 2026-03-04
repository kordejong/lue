#include "lue/framework/algorithm/value_policies/convolve.hpp"
#include "lue/document.hpp"
#include <hpx/hpx_main.hpp>

// TODO: refactor
#include "lue/framework/algorithm/kernel.hpp"
#include "lue/framework/io/gdal.hpp"
#include "lue/gdal.hpp"


class Example: public lue::document::Example
{

    public:

        Example(int const argc, char const* const* argv):

            lue::document::Example{
                [&]() -> cxxopts::Options
                {
                    cxxopts::Options options{std::filesystem::path{argv[0]}.filename()};

                    options.positional_help("<array> <kernel> <result>");
                    options.custom_help("");
                    options.add_options()
                        // clang-format off
                        ("h,help", "Show usage")
                        ("v,version", "Show version")
                        ("argument_array", "array to read", cxxopts::value<std::string>())
                        ("argument_kernel", "kernel to read", cxxopts::value<std::string>())
                        ("result_array", "array to write", cxxopts::value<std::string>())
                        // clang-format on
                        ;
                    options.parse_positional({"argument_array", "argument_kernel", "result_array"});

                    return options;
                }(),
                argc,
                argv}

        {
        }

    private:

        auto run_implementation() -> int override
        {
            // Obtain command line arguments
            if (!argument_parsed("argument_array") || !argument_parsed("argument_kernel") ||
                !argument_parsed("result_array"))
            {
                throw std::runtime_error("Wrong usage, pass '--help' for more information");
            }

            auto const argument_array_pathname = argument<std::string>("argument_array");
            auto const argument_kernel_pathname = argument<std::string>("argument_kernel");
            auto const result_array_pathname = argument<std::string>("result_array");

            // Setup types
            lue::Rank const rank{2};

            using Weight = float;
            using Kernel = lue::Kernel<Weight, rank>;
            using Element = float;
            using Array = lue::PartitionedArray<Element, rank>;
            using Shape = lue::ShapeT<Array>;


            Kernel kernel{{3, 3}};

            lue::gdal::register_gdal_drivers();
            auto raster =
                lue::gdal::Raster{lue::gdal::open_dataset(argument_kernel_pathname, GDALAccess::GA_ReadOnly)};
            raster.band(1).read(kernel.data());

            Array const array = lue::from_gdal<Element>(argument_array_pathname, Shape{5, 5});
            Array const result = lue::value_policies::convolve(array, kernel);

            lue::to_gdal(result, result_array_pathname);

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
