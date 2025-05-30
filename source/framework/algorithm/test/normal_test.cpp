#define BOOST_TEST_MODULE lue framework algorithm normal
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/default_policies/all.hpp"
#include "lue/framework/algorithm/default_policies/comparison.hpp"
#include "lue/framework/algorithm/default_policies/logical_and.hpp"
#include "lue/framework/algorithm/default_policies/normal.hpp"
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

            Array array{lue::create_partitioned_array<Element>(array_shape, partition_shape)};

            Element mean{7};
            Element stddev{9};

            // Fill two arrays with values from a normal distribution and check
            // whether
            // - All cells in these arrays are > mean - 10 * stddev
            // - All cells in these arrays are < mean + 10 * stddev
            // (- All cells in these arrays are different)

            array = normal(array, mean, stddev);

            BOOST_CHECK(all(array > mean - (10 * stddev)).future().get());
            BOOST_CHECK(all(array < mean + (10 * stddev)).future().get());
        }
    }


    template<typename Element>
    void test_scalar()
    {
        if constexpr (lue::BuildOptions::default_policies_enabled)
        {
            using namespace lue::default_policies;

            using Scalar = lue::Scalar<Element>;

            Element const mean{7};
            Element const stddev{9};

            // Fill scalar with value from a normal distribution and check whether
            // - value > mean - 10 * stddev
            // - value < mean + 10 * stddev

            Scalar const scalar = normal(mean, stddev);

            BOOST_CHECK((scalar > mean - (10 * stddev)).future().get());
            BOOST_CHECK((scalar < mean + (10 * stddev)).future().get());
        }
    }

}  // Anonymous namespace


BOOST_AUTO_TEST_CASE(use_case_01)
{
    lue::Rank const rank{2};

    test_array<lue::FloatingPointElement<0>, rank>();
    test_scalar<lue::FloatingPointElement<0>>();
}


namespace {

    using Element = lue::FloatingPointElement<0>;
    std::size_t const rank = 2;

    using Array = lue::PartitionedArray<Element, rank>;
    using Shape = lue::ShapeT<Array>;
    // using Offset = lue::OffsetT<Array>;

    // Shape const array_shape{{60, 40}};
    // Shape const partition_shape{{10, 10}};
}  // namespace


BOOST_AUTO_TEST_CASE(use_case_1)
{
    // Create a partitioned array filled with random values and verify that all cells contain
    // correct values

    if constexpr (lue::BuildOptions::default_policies_enabled)
    {
        using namespace lue::default_policies;

        lue::Count const nr_rows = 300;
        lue::Count const nr_cols = 400;
        Shape const array_shape{{nr_rows, nr_cols}};

        lue::Count const nr_rows_partition = 43;
        lue::Count const nr_cols_partition = 43;
        Shape const partition_shape{{nr_rows_partition, nr_cols_partition}};

        Element const mean{33};
        Element const stddev{55};

        Array array = normal(array_shape, partition_shape, mean, stddev);

        BOOST_CHECK(all(array > mean - (10 * stddev)).future().get());
        BOOST_CHECK(all(array < mean + (10 * stddev)).future().get());
    }
}
