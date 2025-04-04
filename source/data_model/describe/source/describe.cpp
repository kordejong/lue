#include "describe.hpp"
#include "describe_cf.hpp"
#include "describe_netcdf4.hpp"
#include <format>


namespace lue::utility {

    Describe::Describe(int const argc, char const* const* argv):

        Application{
            []()
            {
                cxxopts::Options options{"lue_describe", "Describe a dataset"};
                options.add_options()("h,help", "Show usage")("v,version", "Show version")(
                    "dataset", "Input dataset", cxxopts::value<std::string>());
                options.parse_positional({"dataset"});
                options.positional_help("<dataset>");
                options.show_positional_help();
                return options;
            }(),
            argc,
            argv}

    {
    }


    auto Describe::run_implementation() -> int
    {
        if (!argument_parsed("dataset"))
        {
            throw std::runtime_error("missing dataset");
        }

        auto const dataset_name = argument<std::string>("dataset");
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
            print_error_message(std::format("Cannot open dataset {}: {}", dataset_name, exception.what()));
        }

        for (auto const& line : description)
        {
            print_info_message(line);
        }

        return result;
    }

}  // namespace lue::utility
