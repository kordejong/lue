#define BOOST_TEST_MODULE lue framework algorithm copy
#include "lue/framework/algorithm/copy.hpp"
#include "lue/framework/algorithm/create_partitioned_array.hpp"
#include "lue/framework/algorithm/default_policies/all.hpp"
#include "lue/framework/algorithm/default_policies/equal_to.hpp"
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

            Element const fill_value{5};

            Array array{lue::create_partitioned_array(array_shape, partition_shape, fill_value)};

            auto copy = lue::copy(array);

            BOOST_CHECK(all(copy == array).future().get());
        }
    }

}  // Anonymous namespace


BOOST_AUTO_TEST_CASE(use_case_01)
{
    lue::Rank const rank{2};

    test_array<lue::SignedIntegralElement<0>, rank>();
    test_array<lue::FloatingPointElement<0>, rank>();
}
