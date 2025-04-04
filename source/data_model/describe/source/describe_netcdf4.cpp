#include "describe_netcdf4.hpp"
#include "lue/netcdf4/type.hpp"
#include <algorithm>


namespace lue::netcdf {

    void to_json(nlohmann::ordered_json& json, Attribute const& attribute)
    {
        json = nlohmann::ordered_json{
            {"name", attribute.name()},
            {"type", netcdf::as_string(attribute.type())},
            {"length", attribute.length()},
        };

        nlohmann::ordered_json::value_type value{};

        if (attribute.type() == NC_CHAR || attribute.length() == 1)
        {
            nlohmann::json value_json{};

            switch (attribute.type())
            {
                case NC_CHAR:
                {
                    value = attribute.value();
                    break;
                }
                case NC_BYTE:
                {
                    value = attribute.value<std::int8_t>();
                    break;
                }
                case NC_UBYTE:
                {
                    value = attribute.value<std::uint8_t>();
                    break;
                }
                case NC_INT:
                {
                    value = attribute.value<std::int32_t>();
                    break;
                }
                case NC_UINT:
                {
                    value = attribute.value<std::uint32_t>();
                    break;
                }
                case NC_INT64:
                {
                    value = attribute.value<std::int64_t>();
                    break;
                }
                case NC_UINT64:
                {
                    value = attribute.value<std::uint64_t>();
                    break;
                }
                case NC_FLOAT:
                {
                    value = attribute.value<float>();
                    break;
                }
                case NC_DOUBLE:
                {
                    value = attribute.value<double>();
                    break;
                }
                default:
                {
                    value = "TODO";
                    break;
                }
            }
        }
        else
        {
            value = "TODO";
        }

        json["value"] = value;
    }


    void to_json(nlohmann::ordered_json& json, Dimension const& dimension)
    {
        json = nlohmann::ordered_json{
            {"name", dimension.name()},
            {"length", dimension.length()},
        };
    }


    void to_json(nlohmann::ordered_json& json, Variable const& variable)
    {
        nlohmann::ordered_json attributes_json{};

        std::ranges::transform(
            variable.attributes(),
            std::back_inserter(attributes_json),
            [](auto const& attribute) { return attribute; });

        nlohmann::ordered_json dimensions_json{};

        std::ranges::transform(
            variable.dimensions(),
            std::back_inserter(dimensions_json),
            [](auto const& dimension) { return dimension; });

        json = nlohmann::ordered_json{
            {"name", variable.name()},
            {"attributes", attributes_json},
            {"dimensions", dimensions_json},
        };
    }


    void to_json(nlohmann::ordered_json& json, Group const& group)
    {
        nlohmann::ordered_json attributes_json{};

        std::ranges::transform(
            group.attributes(),
            std::back_inserter(attributes_json),
            [](auto const& attribute) { return attribute; });

        nlohmann::ordered_json variables_json{};

        std::ranges::transform(
            group.variables(),
            std::back_inserter(variables_json),
            [](auto const& variable) { return variable; });

        nlohmann::ordered_json groups_json{};

        std::ranges::transform(
            group.child_groups(), std::back_inserter(groups_json), [](auto const& group) { return group; });

        json = nlohmann::ordered_json{
            {"name", group.name()},
            {"attributes", attributes_json},
            {"variables", variables_json},
            {"child_groups", groups_json},
        };
    }


    void to_json(nlohmann::ordered_json& json, Dataset const& dataset)
    {
        json = nlohmann::ordered_json{
            {"path", dataset.path()},
            {"format", Dataset::format_as_string(dataset.format())},
        };

        nlohmann::ordered_json group_json = dynamic_cast<Group const&>(dataset);

        json.update(group_json);
    }

}  // namespace lue::netcdf
