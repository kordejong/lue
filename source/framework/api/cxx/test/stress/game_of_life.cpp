#include "stress.hpp"
#include "lue/framework/api/cxx.hpp"
#include "lue/framework/model.hpp"
#include "lue/framework.hpp"
#include <hpx/hpx_main.hpp>


namespace {

    class Model: public lue::Model
    {

        public:

            static lue::Rank const rank{2};

            using Shape = lue::Shape<lue::Count, rank>;

            using BooleanElement = lue::BooleanElement;

            using FloatElement = lue::FloatingPointElement<0>;


            Model(Shape const array_shape, Shape const partition_shape):
                lue::Model{},
                _array_shape{array_shape},
                _partition_shape{partition_shape},
                _kernel{
                    {3, 3},
                    {
                        // clang-format off
                        1, 1, 1,
                        1, 0, 1,
                        1, 1, 1,
                        // clang-format on
                    }}


            {
            }


            void initialize() final
            {
                using namespace lue::api;

                FloatElement const fraction_alive_cells{0.25};
                Field const random_field =
                    uniform(_array_shape, _partition_shape, FloatElement{0}, FloatElement{1});

                _generation = random_field <= fraction_alive_cells;
            }


            auto simulate([[maybe_unused]] lue::Count const time_step) -> hpx::shared_future<void> final
            {
                using namespace lue::api;

                Field const nr_alive_cells = focal_sum(_generation, _kernel);

                // Next state of currently alive cells
                Field const underpopulated = nr_alive_cells < BooleanElement{2};
                Field const overpopulated = nr_alive_cells > BooleanElement{3};

                // Next state of currently dead cells
                Field const reproducing = nr_alive_cells == BooleanElement{3};

                _generation = where(
                    _generation,
                    // True if alive and not under / overpopulated
                    !(underpopulated || overpopulated),
                    // True if dead with three neighbours
                    reproducing);

                return as_state(_generation);
            }


        private:

            Shape _array_shape;

            Shape _partition_shape;

            lue::Kernel<BooleanElement, 2> _kernel;

            lue::api::Field _generation;
    };

}  // Anonymous namespace


auto main(int argc, char* argv[]) -> int
{
    return lue::stress::run_stress_test<Model>(argc, argv);
}
