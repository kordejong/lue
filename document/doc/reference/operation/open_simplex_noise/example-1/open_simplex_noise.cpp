#include "lue/framework/algorithm/value_policies/open_simplex_noise.hpp"
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/value_policies/cast.hpp"
#include "lue/framework/algorithm/value_policies/cell_index.hpp"
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

            using namespace lue;
            using namespace lue::value_policies;

            auto const result_array_pathname = argument<std::string>("result_array");

            // [example
            Rank const rank{2};
            using Shape = Shape<Count, rank>;
            using FloatElement = FloatingPointElement<0>;
            using BooleanArray = PartitionedArray<BooleanElement, rank>;
            using FloatArray = PartitionedArray<FloatElement, rank>;

            Shape const array_shape{600, 400};
            BooleanArray const condition_array = create_partitioned_array<BooleanElement>(array_shape, 1);
            FloatArray const x_coordinates = cast<FloatElement>(cell_index<IndexElement>(condition_array, 1));
            FloatArray const y_coordinates = cast<FloatElement>(cell_index<IndexElement>(condition_array, 0));
            int const seed = 5;
            FloatArray const result = open_simplex_noise(x_coordinates, y_coordinates, seed);

            to_gdal(result, result_array_pathname);
            // example]

            return EXIT_SUCCESS;
        }
};


auto main(int argc, char** argv) -> int
{
    return Example{argc, argv}.run();
}
