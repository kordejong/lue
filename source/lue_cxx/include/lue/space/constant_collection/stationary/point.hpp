#pragma once
#include "lue/item/constant_collection/constant_shape/same_shape/constant.hpp"


namespace lue {
namespace constant_collection {
namespace stationary {

class Point:
    public constant_shape::same_shape::Constant
{

public:

                   Point               (hdf5::Group const& parent,
                                        hdf5::Datatype const& memory_datatype);

                   Point               (Point const&)=delete;

                   Point               (Point&&)=default;

                   ~Point              ()=default;

    Point&         operator=           (Point const&)=delete;

    Point&         operator=           (Point&&)=default;

private:

};


Point              create_point        (hdf5::Group& parent,
                                        hdf5::Datatype const& memory_datatype,
                                        std::size_t rank);

Point              create_point        (hdf5::Group& parent,
                                        hdf5::Datatype const& file_datatype,
                                        hdf5::Datatype const& memory_datatype,
                                        std::size_t rank);

}  // namespace stationary
}  // namespace constant_collection
}  // namespace lue
