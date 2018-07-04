#pragma once
#include "lue/array/same_shape/value.hpp"


namespace lue {

/*!
    - One space point per item
    - Each space point has a unique location in space
*/
class StationarySpacePoint:
    public same_shape::Value
{

public:

                   StationarySpacePoint(hdf5::Group& parent,
                                        hdf5::Datatype const& memory_datatype);

                   StationarySpacePoint(StationarySpacePoint const&)=delete;

                   StationarySpacePoint(StationarySpacePoint&&)=default;

                   StationarySpacePoint(same_shape::Value&& value);

                   ~StationarySpacePoint()=default;

    StationarySpacePoint& operator=    (StationarySpacePoint const&)=delete;

    StationarySpacePoint& operator=    (StationarySpacePoint&&)=default;

private:

};


StationarySpacePoint   create_stationary_space_point(
                                        hdf5::Group& parent,
                                        hdf5::Datatype const& memory_datatype,
                                        std::size_t rank);

StationarySpacePoint   create_stationary_space_point(
                                        hdf5::Group& parent,
                                        hdf5::Datatype const& file_datatype,
                                        hdf5::Datatype const& memory_datatype,
                                        std::size_t rank);

}  // namespace lue
