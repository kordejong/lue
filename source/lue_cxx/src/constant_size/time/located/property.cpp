#include "lue/constant_size/time/located/property.hpp"
#include "lue/tag.hpp"


namespace lue {
namespace constant_size {
namespace time {
namespace located {

Property::Property(
    hdf5::Identifier const& location)

    : constant_size::Property(location)

{
}


Property::Property(
    constant_size::Property&& property)

    : constant_size::Property(std::forward<constant_size::Property>(property))

{
}


void Property::discretize_time(
    lue::Property const& property)
{
    create_soft_link(property.id(), time_discretization_tag);
}


bool Property::time_is_discretized () const
{
    return contains_soft_link(time_discretization_tag);
}


lue::Property Property::time_discretization () const
{
    return lue::Property(id(), time_discretization_tag);
}


Property create_property(
    hdf5::Group& group,
    std::string const& name,
    Property::Configuration const& configuration)
{
    auto property = constant_size::create_property(
        group, name, configuration);

    return Property(std::move(property));
}


}  // namespace located
}  // namespace time
}  // namespace constant_size
}  // namespace lue