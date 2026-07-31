#define BOOST_TEST_MODULE lue hdf5 chunk
#include "lue/hdf5/chunk.hpp"
#include <boost/test/included/unit_test.hpp>
#include <cstdint>


namespace lh5 = lue::hdf5;


template<typename Element>
static auto test_chunk(lh5::Shape const& value_shape, lh5::Shape const& chunk_shape)
{
    BOOST_CHECK_EQUAL(chunk_shape.size(), value_shape.size());

    auto const size = lh5::size_of_chunk<Element>(chunk_shape);

    if (!chunk_shape.empty())
    {
        BOOST_CHECK_GE(size, 1);
        // BOOST_CHECK_GE(size, lh5::lower_chunk_size_limit());
        BOOST_CHECK_LE(size, lh5::upper_chunk_size_limit());
    }
}


static auto test_chunk(lh5::Shape const& value_shape) -> lh5::Shape
{
    using T = std::int32_t;

    auto chunk_shape = lh5::chunk_shape<T>(value_shape);

    test_chunk<T>(value_shape, chunk_shape);

    return chunk_shape;
}


static auto test_chunk(lh5::Shape const& value_shape, std::size_t const nr_dimensions_to_skip) -> lh5::Shape
{
    using T = std::int32_t;

    auto chunk_shape = lh5::chunk_shape<T>(value_shape, nr_dimensions_to_skip);

    test_chunk<T>(value_shape, chunk_shape);

    return chunk_shape;
}


BOOST_AUTO_TEST_CASE(chunk_shape_small_1)
{
    // 0D values
    lh5::Shape const value_shape{};
    test_chunk(value_shape);
}


BOOST_AUTO_TEST_CASE(chunk_shape_small_2)
{
    // 1D values
    lh5::Shape const value_shape({10});
    test_chunk(value_shape);
}


BOOST_AUTO_TEST_CASE(chunk_shape_small_3)
{
    // 2D values
    lh5::Shape const value_shape({2, 3});

    test_chunk(value_shape);

    std::size_t const nr_dimensions_to_skip = 1;
    auto const chunk_shape = test_chunk(value_shape, nr_dimensions_to_skip);

    BOOST_CHECK_EQUAL(chunk_shape[0], 1);
}


BOOST_AUTO_TEST_CASE(chunk_shape_large_1)
{
    // 1D values
    lh5::Shape const value_shape({2000000});
    test_chunk(value_shape);
}


BOOST_AUTO_TEST_CASE(chunk_shape_large_2)
{
    // 2D values
    {
        lh5::Shape const value_shape({2000000, 2000000});
        test_chunk(value_shape);

        std::size_t const nr_dimensions_to_skip = 1;
        auto const chunk_shape = test_chunk(value_shape, nr_dimensions_to_skip);

        BOOST_CHECK_EQUAL(chunk_shape[0], 1);
    }

    {
        lh5::Shape const value_shape({1, 2000000});
        auto chunk_shape = test_chunk(value_shape);

        BOOST_CHECK_EQUAL(chunk_shape[0], 1);

        std::size_t const nr_dimensions_to_skip = 1;
        chunk_shape = test_chunk(value_shape, nr_dimensions_to_skip);

        BOOST_CHECK_EQUAL(chunk_shape[0], 1);
    }

    {
        lh5::Shape const value_shape({2000000, 1});
        auto chunk_shape = test_chunk(value_shape);

        std::size_t const nr_dimensions_to_skip = 1;
        chunk_shape = test_chunk(value_shape, nr_dimensions_to_skip);

        BOOST_CHECK_EQUAL(chunk_shape[0], 1);
    }
}


BOOST_AUTO_TEST_CASE(case_01)
{
    lh5::Shape const value_shape({2, 16000, 14000});
    auto chunk_shape = test_chunk(value_shape);

    std::size_t const nr_dimensions_to_skip = 1;
    chunk_shape = test_chunk(value_shape, nr_dimensions_to_skip);

    BOOST_CHECK_EQUAL(chunk_shape[0], 1);
    BOOST_CHECK_GT(chunk_shape[1], 1);
}
