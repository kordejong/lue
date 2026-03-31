#include "lue/framework/algorithm/value_policies/add.hpp"
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

            using namespace lue;
            using namespace value_policies;

            Rank const rank{2};
            using Element = float;
            using Array = PartitionedArray<Element, rank>;

            Array const array = read_array<Element, rank>(argument_array_pathname);

            // [example
            // TODO
            Array result = add(array, Element{1});
            // example]

            to_gdal(result, result_array_pathname);

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
