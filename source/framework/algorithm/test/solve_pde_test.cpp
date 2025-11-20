#define BOOST_TEST_MODULE lue framework algorithm solve_pde
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/value_policies/solve_pde.hpp"
#include "lue/framework/test/hpx_unit_test.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(use_case_01)
{
    // TODO: Add popular examples

    // using Element = lue::FloatingPointElement<0>;
    // std::size_t const rank = 2;
    //
    // using ElementArray = lue::PartitionedArray<Element, rank>;
    // using Shape = lue::ShapeT<ElementArray>;
    //
    // Shape const array_shape{{5, 5}};
    // Shape const partition_shape{{5, 5}};
    //
    // Element const x{lue::policy::no_data_value<Element>};
    //
    // auto const array = lue::test::create_partitioned_array<ElementArray>(
    //     array_shape,
    //     partition_shape,
    //     {
    //         // NOLINTBEGIN
    //         // clang-format off
    //         {
    //             0, -1, 1, -30,  0,
    //             2,  x, 1,   2, -3,
    //             3,  2, 3,   4,  2,
    //             0,  0, 2,  40,  2,
    //             1, -2, 4,   7,  1,
    //         },
    //         // clang-format on
    //         // NOLINTEND
    //     });
    // auto const kernel = lue::box_kernel<lue::BooleanElement, 2>(1, 1);
    // auto const result_we_got = lue::value_policies::solve_pde(array, kernel);
    // auto const result_we_want = lue::test::create_partitioned_array<ElementArray>(
    //     array_shape,
    //     partition_shape,
    //     {
    //         // NOLINTBEGIN
    //         // clang-format off
    //         {
    //              0.3333, 0.6,   -5.4,  -4.83,   -7.75,
    //              1.2,    1.375, -2.25, -2.2222, -4.17,
    //              1.4,    1.625,  6.75,  5.89,    7.83,
    //              0.667,  1.4444, 6.67,  7.22,    9.33,
    //             -0.25,   0.833,  8.5,   9.33,    12.5,
    //         },
    //         // clang-format on
    //         // NOLINTEND
    //     });
    //
    // lue::test::check_arrays_are_close(result_we_got, result_we_want, Element{1e-3});
}


BOOST_AUTO_TEST_CASE(all_no_data)
{
    using Element = lue::FloatingPointElement<0>;
    std::size_t const rank = 2;

    using ElementArray = lue::PartitionedArray<Element, rank>;
    using Shape = lue::ShapeT<ElementArray>;

    Shape const array_shape{{3, 3}};
    Shape const partition_shape{{3, 3}};

    Element const x{lue::policy::no_data_value<Element>};
    auto const result_we_want = lue::test::create_partitioned_array<ElementArray>(
        array_shape,
        partition_shape,
        {
            // NOLINTBEGIN
            // clang-format off
            {
                x, x, x,
                x, x, x,
                x, x, x,
            },
            // clang-format on
            // NOLINTEND
        });

    {
        lue::Count const nr_ticks{10};
        auto const c_t{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const c_x{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const c_y{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const c_xx{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const c_yy{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const c_xy{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const rhs{lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
        auto const dirichlet_boundary_condition{
            lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
        auto const neumann_boundary_condition{
            lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};

        auto const result_we_got = lue::value_policies::solve_pde(
            nr_ticks,
            c_t,
            c_x,
            c_y,
            c_xx,
            c_yy,
            c_xy,
            rhs,
            dirichlet_boundary_condition,
            neumann_boundary_condition);

        // TODO: lue::test::check_arrays_are_equal(result_we_got, result_we_want);
    }

    // {
    //     auto const dirichlet_boundary_condition{
    //         lue::create_partitioned_array<Element>(array_shape, partition_shape, x)};
    //     auto const array1{lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
    //     auto const array2{lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
    //     auto const array3{lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
    //     auto const array4{lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
    //     auto const array5{lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
    //     auto const array6{lue::create_partitioned_array<Element>(array_shape, partition_shape, 0)};
    //
    //     auto const result_we_got = lue::value_policies::solve_pde(
    //         dirichlet_boundary_condition, array1, array2, array3, array4, array5, array6);
    //
    //     // TODO: lue::test::check_arrays_are_equal(result_we_got, result_we_want);
    // }
}
