#include "lue/framework/api/c/array.h"
#include "field.hpp"
#include "lue/framework/api/cxx/create_array.hpp"


Field* create_array(
    Rank const rank, Extent const* array_extents, Field const* fill_value, Extent const* partition_extents)
{
    assert(rank == 2);

    lue::Shape<Extent, 2> array_shape{};

    std::copy(array_extents, array_extents + rank, array_shape.data());

    lue::api::Field result{};

    if (partition_extents != nullptr)
    {
        lue::Shape<Extent, 2> partition_shape{};

        std::copy(partition_extents, partition_extents + rank, array_shape.data());

        result = lue::api::create_array(array_shape, partition_shape, as_cxx_field(fill_value));
    }
    else
    {
        result = lue::api::create_array(array_shape, as_cxx_field(fill_value));
    }

    return new Field{.instance = new lue::api::Field{std::move(result)}};
}
