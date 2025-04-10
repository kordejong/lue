#include "lue/cf/field.hpp"


namespace lue::cf {

    Field::Property::Property(std::string const& name, std::string const& value):

        _name{name},
        _value{value}

    {
    }


    auto Field::Property::name() const -> std::string
    {
        return _name;
    }


    auto Field::Property::value() const -> std::string
    {
        return _value;
    }


    Field::Field(int const group_id, int const variable_id):

        netcdf4::DataVariable{group_id, variable_id}

    {
        Field::Properties properties{};

        for (auto const& attribute : attributes())
        {
            properties.insert({attribute.name(), {attribute.name(), to_string(attribute)}});
        }

        add_properties(std::move(properties));
    }


    auto Field::has_domain() const -> bool
    {
        return _domain.operator bool();
    }


    auto Field::domain() const -> Domain const&
    {
        assert(_domain);

        return *_domain;
    }


    auto Field::field_ancillaries() const -> FieldAncillaries const&
    {
        return _field_ancillaries;
    }


    auto Field::cell_methods() const -> CellMethods const&
    {
        return _cell_methods;
    }


    auto Field::properties() const -> Properties const&
    {
        return _properties;
    }


    void Field::set_property(Property const& property)
    {
        auto const key = property.name();

        _properties.insert({key, property});
    }


    void Field::set_property(Property&& property)
    {
        auto const key = property.name();

        _properties.insert({key, std::move(property)});
    }


    void Field::set_properties(Properties const& properties)
    {
        _properties = properties;
    }


    void Field::set_properties(Properties&& properties)
    {
        _properties = std::move(properties);
    }


    void Field::add_properties(Properties const& properties)
    {
        for (auto const& [name, property] : properties)
        {
            set_property(property);
        }
    }


    void Field::add_properties(Properties&& properties)
    {
        for (auto&& [name, property] : properties)
        {
            set_property(std::move(property));
        }
    }


    auto Field::has_property(std::string const& name) const -> bool
    {
        return _properties.contains(name);
    }


    auto Field::property(std::string const& name) const -> Property
    {
        return _properties.at(name);
    }

}  // namespace lue::cf
