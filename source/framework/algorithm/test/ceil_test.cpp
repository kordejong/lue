#define BOOST_TEST_MODULE lue framework algorithm ceil
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/value_policies/ceil.hpp"
#include "lue/framework/test/hpx_unit_test.hpp"
#include "lue/framework.hpp"


BOOST_AUTO_TEST_CASE(pcraster_manual_example1)
{
    using Value = lue::FloatingPointElement<0>;

    auto const nd{lue::policy::no_data_value<Value>};
    lue::Shape<lue::Count, 2> const array_shape{3, 3};
    lue::Shape<lue::Count, 2> const partition_shape{3, 3};

    auto const expression = lue::test::create_partitioned_array<lue::PartitionedArray<Value, 2>>(
        array_shape,
        partition_shape,
        {{
            0.0,
            1.5,
            3.4,
            nd,
            -3.2,
            -2.5,
            0.01,
            9.3,
            8.9,
        }});

    auto const result_we_want = lue::test::create_partitioned_array<lue::PartitionedArray<Value, 2>>(
        array_shape,
        partition_shape,
        {{
            0.0,
            2.0,
            4.0,
            nd,
            -3.0,
            -2.0,
            1.0,
            10.0,
            9.0,
        }});

    auto const result_we_got = lue::value_policies::ceil(expression);

    lue::test::check_arrays_are_equal(result_we_got, result_we_want);
}
