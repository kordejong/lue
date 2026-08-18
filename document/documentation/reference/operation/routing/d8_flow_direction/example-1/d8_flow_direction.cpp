#include "lue/framework/api/cxx.hpp"
#include "lue/document.hpp"
#include <hpx/hpx_main.hpp>


class Example: public lue::document::Example
{

    public:

        Example(int const argc, char const* const* argv):

            lue::document::Example{
                [&]() -> cxxopts::Options
                {
                    cxxopts::Options options{std::filesystem::path{argv[0]}.filename()};

                    options.positional_help("<elevation> <flow_direction>");
                    options.add_options()
                        // clang-format off
                        ("elevation_array", "array to read", cxxopts::value<std::string>())
                        ("flow_direction_array", "array to write", cxxopts::value<std::string>())
                        // clang-format on
                        ;
                    options.parse_positional({"elevation_array", "flow_direction_array"});

                    return options;
                }(),
                argc,
                argv}

        {
        }

    private:

        auto run_implementation() -> int override
        {
            if (!argument_parsed("elevation_array") || !argument_parsed("flow_direction_array"))
            {
                throw std::runtime_error("Wrong usage, pass '--help' for more information");
            }

            auto const elevation_array_pathname = argument<std::string>("elevation_array");
            auto const flow_direction_array_pathname = argument<std::string>("flow_direction_array");

            using namespace lue::api;

            // [example
            Field const elevation = from_gdal(elevation_array_pathname);
            Field const flow_direction = d8_flow_direction(elevation);

            auto written = to_gdal(flow_direction, flow_direction_array_pathname);
            // example]

            written.get();
            lue::document::write_value_scale(flow_direction_array_pathname, "flow_direction");

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
