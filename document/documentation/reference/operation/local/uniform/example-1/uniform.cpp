#include "lue/framework/algorithm/value_policies/uniform.hpp"
#include "lue/document.hpp"
#include "lue/framework.hpp"
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
            using namespace lue::value_policies;

            // [example
            Rank const rank{2};
            using Shape = Shape<Count, rank>;
            using Element = LargestUnsignedIntegralElement;
            using Array = PartitionedArray<Element, rank>;

            Shape const array_shape{6, 4};
            Element const min_value{1};
            Element const max_value{10};
            Array const result = uniform(array_shape, min_value, max_value);

            to_gdal(result, result_array_pathname);
            // example]

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
