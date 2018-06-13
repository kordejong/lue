#pragma once
#include "lue/core/array.hpp"
#include "lue/array/value_group.hpp"
#include "lue/core/define.hpp"


namespace lue {
namespace different_shape {

/*!
    @brief      Class for storing different shape x constant value
                object arrays

    The implementation uses an HDF5 dataset per object array.
*/
class Value:
    public ValueGroup
{

public:

                   Value               (hdf5::Group const& parent,
                                        std::string const& name);

                   Value               (hdf5::Group const& parent,
                                        std::string const& name,
                                        hdf5::Datatype const& memory_datatype);

                   Value               (ValueGroup&& group);

                   Value               (Value const&)=delete;

                   Value               (Value&&)=default;

                   ~Value              ()=default;

    Value&         operator=           (Value const&)=delete;

    Value&         operator=           (Value&&)=default;

    Count          nr_objects          () const;

    void           reserve             (Count nr_objects,
                                        ID const* ids,
                                        hdf5::Shape const* shapes);

    void           reserve             (ID id,
                                        hdf5::Shape const& shape);

    Array          operator[]          (ID id);

private:

    // hdf5::Hyperslab hyperslab          (IndexRange const& range) const;

    // hdf5::Hyperslab hyperslab          (Index idx) const;

    Count          _nr_objects;

};


Value              create_value        (hdf5::Group& parent,
                                        std::string const& name,
                                        hdf5::Datatype const& memory_datatype,
                                        Rank rank);

Value              create_value        (hdf5::Group& parent,
                                        std::string const& name,
                                        hdf5::Datatype const& file_datatype,
                                        hdf5::Datatype const& memory_datatype,
                                        Rank rank);

}  // namespace different_shape
}  // namespace lue
