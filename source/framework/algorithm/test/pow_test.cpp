#define BOOST_TEST_MODULE lue framework algorithm pow
#include "lue/framework/algorithm/all.hpp"
#include "lue/framework/algorithm/comparison.hpp"
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/pow.hpp"
#include "lue/framework/test/array.hpp"
#include "lue/framework/test/hpx_unit_test.hpp"


namespace detail {

    template<
        typename Element,
        std::size_t rank>
    void test_array()
    {
        using Array = lue::PartitionedArray<Element, rank>;

        auto const array_shape{lue::Test<Array>::shape()};
        auto const partition_shape{lue::Test<Array>::partition_shape()};

        Element const fill_value{2};
        Element const exponent{3};

        Array array{lue::create_partitioned_array(array_shape, partition_shape, fill_value)};

        auto pow = lue::pow(array, exponent);

        BOOST_CHECK(lue::all(pow == std::pow(fill_value, exponent)).get());
    }

}  // namespace detail


#define TEST_CASE(                               \
    rank,                                        \
    Element)                                     \
                                                 \
BOOST_AUTO_TEST_CASE(array_##rank##d_##Element)  \
{                                                \
    detail::test_array<Element, rank>();         \
}

TEST_CASE(1, double)
TEST_CASE(2, double)

#undef TEST_CASE
