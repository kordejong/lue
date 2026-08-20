#include "lue/framework/api/cxx.hpp"
#include "lue/framework/configure.hpp"
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

                    options.positional_help("<result>");
                    options.add_options()
                        // clang-format off
                        ("result_array", "array to write", cxxopts::value<std::string>())
                        // clang-format on
                        ;
                    options.parse_positional({"result_array"});

                    return options;
                }(),
                argc,
                argv}

        {
        }

    private:

        auto run_implementation() -> int override
        {
            // Command line arguments
            if (!argument_parsed("result_array"))
            {
                throw std::runtime_error("Wrong usage, pass '--help' for more information");
            }

            auto const result_array_pathname = argument<std::string>("result_array");

            using namespace lue;
            using namespace lue::api;

            // [example
            Field const condition_array = as_field(create_array({600, 400}, BooleanElement{1}));
            Field const x_coordinates = cast(cell_index(condition_array, 1), ElementType::Float32);
            Field const y_coordinates = cast(cell_index(condition_array, 0), ElementType::Float32);
            int const seed = 5;
            Field const result = open_simplex_noise(x_coordinates, y_coordinates, seed);

            to_gdal(result, result_array_pathname);
            // example]

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
