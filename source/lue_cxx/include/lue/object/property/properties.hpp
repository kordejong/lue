#pragma once
#include "lue/object/property/same_shape/properties.hpp"
#include "lue/object/property/same_shape/constant_shape/properties.hpp"
#include "lue/info/identity/active_id.hpp"
#include "lue/info/identity/active_object_index.hpp"
#include "lue/info/identity/active_set_index.hpp"


namespace lue {

class Properties:
    public hdf5::Group
{

public:

    friend Properties create_properties(hdf5::Group& parent);

    explicit       Properties          (hdf5::Group const& parent);

                   Properties          (Properties const&)=delete;

                   Properties          (Properties&&)=default;

                   ~Properties         ()=default;

    Properties&    operator=           (Properties const&)=delete;

    Properties&    operator=           (Properties&&)=default;

    info::ID const& id                 () const;

    info::ActiveSetIndex const&
                   active_set_index    () const;

    info::ActiveObjectIndex const&
                   active_object_index () const;

    info::ActiveID const&
                   active_id           () const;

    template<
        typename T>
    T const&       collection          () const;

private:

                   Properties          (hdf5::Group&& group);

    info::ID       _id;

    info::ActiveID _active_id;

    info::ActiveSetIndex _active_set_index;

    info::ActiveObjectIndex _active_object_index;

    same_shape::Properties _same_shape_properties;

    same_shape::constant_shape::Properties
        _same_shape_constant_shape_properties;

};


Properties         create_properties   (hdf5::Group& parent);


template<>
inline same_shape::Properties const&
    Properties::collection<same_shape::Properties>() const
{
    return _same_shape_properties;
}


template<>
inline same_shape::constant_shape::Properties const&
    Properties::collection<same_shape::constant_shape::Properties>() const
{
    return _same_shape_constant_shape_properties;
}

}  // namespace lue
