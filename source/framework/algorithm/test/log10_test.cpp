#define BOOST_TEST_MODULE lue framework algorithm log10
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/default_policies/all.hpp"
#include "lue/framework/algorithm/default_policies/equal_to.hpp"
#include "lue/framework/algorithm/default_policies/log10.hpp"
#include "lue/framework/algorithm/value_policies/log10.hpp"
#include "lue/framework/test/hpx_unit_test.hpp"
#include "lue/framework.hpp"


namespace {

    template<typename Element, std::size_t rank>
    void test_array()
    {
        if constexpr (lue::BuildOptions::default_policies_enabled)
        {
            using namespace lue::default_policies;

            using Array = lue::PartitionedArray<Element, rank>;

            auto const array_shape{lue::Test<Array>::shape()};
            auto const partition_shape{lue::Test<Array>::partition_shape()};

            Array array{lue::create_partitioned_array(array_shape, partition_shape, Element{25})};

            BOOST_CHECK(all(log10(array) == std::log10(Element{25})).future().get());
        }
    }

}  // Anonymous namespace


BOOST_AUTO_TEST_CASE(use_case_01)
{
    lue::Rank const rank{2};

    test_array<lue::FloatingPointElement<0>, rank>();
}


BOOST_AUTO_TEST_CASE(pcraster_manual_example1)
{
    if constexpr (lue::BuildOptions::default_value_policies_enabled)
    {
        using namespace lue::value_policies;

        using Element = lue::FloatingPointElement<0>;

        auto const nd = lue::policy::no_data_value<Element>;
        lue::Shape<lue::Count, 2> const array_shape{3, 3};
        lue::Shape<lue::Count, 2> const partition_shape{3, 3};

        auto const expression = lue::test::create_partitioned_array<lue::PartitionedArray<Element, 2>>(
            array_shape,
            partition_shape,
            {{
                -10.0,
                1.0,
                nd,
                0.0,
                1000.0,
                24.0,
                11.0,
                0.13,
                0.01,
            }});

        auto const result_we_got = log10(expression);
        auto const result_we_want = lue::test::create_partitioned_array<lue::PartitionedArray<Element, 2>>(
            array_shape, partition_shape, {{nd, 0.0, nd, nd, 3.0, 1.38, 1.04, -0.886, -2.0}});
    }
}
