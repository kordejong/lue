#include "describe.hpp"
#include "describe_cf.hpp"
#include "describe_netcdf4.hpp"
#include <fmt/format.h>


namespace lue::utility {

    namespace {

        std::string const usage = fmt::format(
            R"(
Describe a dataset

usage:
    {0} <dataset>
    {0} (-h | --help) | --version

options:
    -h --help   Show this screen
    --version   Show version
)",
            "lue_describe");

    }  // Anonymous namespace


    Describe::Describe(std::vector<std::string> const& arguments):

        Application{usage, arguments}

    {
    }


    auto Describe::run_implementation() -> int
    {
        auto const dataset_name = argument<std::string>("<dataset>");
        int result = EXIT_FAILURE;
        std::vector<std::string> description{};

        try
        {
            // auto dataset = cf::Dataset::open(dataset_name);
            // description = describe_cf_dataset(dataset);

            auto dataset = netcdf::Dataset::open(dataset_name);
            description = describe_netcdf4_dataset(dataset);

            result = EXIT_SUCCESS;
        }
        catch (std::exception const& exception)
        {
            print_error_message(fmt::format("Cannot open dataset {}: {}", dataset_name, exception.what()));
        }

        for (auto const& line : description)
        {
            print_info_message(line);
        }

        return result;
    }

}  // namespace lue::utility
