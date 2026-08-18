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

                    options.positional_help("<array> <result>");
                    options.add_options()
                        // clang-format off
                        ("argument_array", "array to read", cxxopts::value<std::string>())
                        ("result_array", "array to write", cxxopts::value<std::string>())
                        // clang-format on
                        ;
                    options.parse_positional({"argument_array", "result_array"});

                    return options;
                }(),
                argc,
                argv}

        {
        }

    private:

        auto run_implementation() -> int override
        {
            if (!argument_parsed("argument_array") || !argument_parsed("result_array"))
            {
                throw std::runtime_error("Wrong usage, pass '--help' for more information");
            }

            auto const argument_array_pathname = argument<std::string>("argument_array");
            auto const result_array_pathname = argument<std::string>("result_array");

            using namespace lue::api;

            // [example
            Field const array = from_gdal(argument_array_pathname);

            // TODO: Do something useful here
            Field const& result = array;

            to_gdal(result, result_array_pathname);
            // example]

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
