#pragma once
#include "lue/constant_size/value.hpp"
#include "lue/item/constant_size/constant_shape/same_shape/variable.hpp"


namespace lue {
namespace constant_size {
namespace time {
namespace located {
namespace constant_shape {
namespace same_shape {

class Value:
    public constant_size::constant_shape::same_shape::Variable,
    public constant_size::Value
{

public:

                   Value               (hdf5::Group const& parent,
                                        std::string const& name,
                                        hdf5::Datatype const& memory_datatype);

                   Value               (constant_size::constant_shape::same_shape::Variable&&
                                            collection);

                   Value               (Value const&)=delete;

                   Value               (Value&&)=default;

                   ~Value              ()=default;

    Value&         operator=           (Value const&)=delete;

    Value&         operator=           (Value&&)=default;

    hsize_t        nr_items            () const final;

private:

};


Value              create_value        (hdf5::Group& parent,
                                        std::string const& name,
                                        hdf5::Datatype const& file_datatype,
                                        hdf5::Datatype const& memory_datatype);

Value              create_value        (hdf5::Group& parent,
                                        std::string const& name,
                                        hdf5::Datatype const& file_datatype,
                                        hdf5::Datatype const& memory_datatype,
                                        hdf5::Shape const& value_shape);

}  // namespace same_shape
}  // namespace constant_shape
}  // namespace located
}  // namespace time
}  // namespace constant_size
}  // namespace lue