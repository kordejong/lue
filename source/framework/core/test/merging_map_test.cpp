#define BOOST_TEST_MODULE lue framework core merging_map
#include "lue/framework/core/merging_map.hpp"
#include <boost/test/included/unit_test.hpp>


namespace detail {


}  // namespace detail


BOOST_AUTO_TEST_CASE(merge_clump_ids)
{
    // Test the case where this data structure is used in the implementation of the clump algorithm

    using Key = int;
    using Keys = std::vector<Key>;
    using Value = std::string;
    lue::MergingMap<Key, Value> map{};

    // {1, 2, 3, 4, 5} → "five"
    map.insert(Keys{1, 2, 3, 4, 5}, "five");

    BOOST_CHECK_EQUAL(map.nr_keys(), 5);
    BOOST_CHECK_EQUAL(map.nr_values(), 1);

    for (auto const key : Keys{1, 2, 3, 4, 5})
    {
        BOOST_CHECK_EQUAL(std::get<0>(map[key]), "five");
        BOOST_CHECK_EQUAL(std::get<1>(map[key]), 5);  // ref count
    }

    // Associate another key with the value already in the map, referenced by one of the keys already in the
    // map
    // {1, 2, 3, 4, 5, 6} → "five"
    map.associate(6, 3);

    BOOST_CHECK_EQUAL(std::get<0>(map[6]), "five");
    BOOST_CHECK_EQUAL(std::get<1>(map[6]), 6);  // ref count

    // Disassociate a key from a value
    // {1, 2, 4, 5, 6} → "five"
    BOOST_CHECK(map.contains(3));
    map.disassociate(3);
    BOOST_CHECK(!map.contains(3));
    BOOST_CHECK_EQUAL(std::get<1>(map[6]), 5);

    // Update value
    // {1, 2, 4, 5, 6} → "six"
    BOOST_CHECK_EQUAL(std::get<0>(map[6]), "five");
    map.update_value(6, "six");

    for (auto const key : Keys{1, 2, 4, 5, 6})
    {
        BOOST_CHECK_EQUAL(std::get<0>(map[key]), "six");
        BOOST_CHECK_EQUAL(std::get<1>(map[key]), 5);
    }
}
