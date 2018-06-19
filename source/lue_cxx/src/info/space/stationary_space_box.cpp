#include "lue/info/space/stationary_space_box.hpp"
#include "lue/core/tag.hpp"


namespace lue {

StationarySpaceBox::StationarySpaceBox(
    hdf5::Group const& parent,
    hdf5::Datatype const& memory_datatype):

    same_shape::Value{parent, coordinates_tag, memory_datatype}

{
}


StationarySpaceBox::StationarySpaceBox(
    same_shape::Value&& value):

    same_shape::Value{std::forward<same_shape::Value>(value)}

{
}


StationarySpaceBox create_stationary_space_box(
    hdf5::Group& parent,
    hdf5::Datatype const& memory_datatype,
    std::size_t const rank)
{
    return create_stationary_space_box(
        parent, file_datatype(memory_datatype), memory_datatype, rank);
}


StationarySpaceBox create_stationary_space_box(
    hdf5::Group& parent,
    hdf5::Datatype const& file_datatype,
    hdf5::Datatype const& memory_datatype,
    std::size_t const rank)
{
    // A box is defined by the coordinates of two opposite points
    // (diagonally). Two of them is enough.
    hdf5::Shape value_shape = {2 * rank};

    auto value = same_shape::create_value(
        parent, coordinates_tag, file_datatype, memory_datatype, value_shape);

    return StationarySpaceBox{std::move(value)};
}

}  // namespace lue
