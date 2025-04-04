#include "describe_netcdf4.hpp"
#include "lue/string.hpp"
#include <algorithm>


namespace lue::netcdf {

    void to_json(nlohmann::ordered_json& json, Attribute const& attribute)
    {
        json = nlohmann::ordered_json{
            {"name", attribute.name()},
            {"type", attribute.type()},
            {"length", attribute.length()},
        };

        if (attribute.type() == NC_CHAR || attribute.length() == 1)
        {
            nlohmann::json value_json{};

            switch (attribute.type())
            {
                case NC_CHAR:
                {
                    json["value"] = attribute.value();
                    break;
                }
                case NC_BYTE:
                {
                    json["value"] = attribute.value<std::int8_t>();
                    break;
                }
                case NC_UBYTE:
                {
                    json["value"] = attribute.value<std::uint8_t>();
                    break;
                }
                case NC_INT:
                {
                    json["value"] = attribute.value<std::int32_t>();
                    break;
                }
                case NC_UINT:
                {
                    json["value"] = attribute.value<std::uint32_t>();
                    break;
                }
                case NC_INT64:
                {
                    json["value"] = attribute.value<std::int64_t>();
                    break;
                }
                // TODO
                // case NC_UINT64:
                //     {
                //         json["value"] = attribute.value<std::uint64_t>();
                //         break;
                //     }
                case NC_FLOAT:
                {
                    json["value"] = attribute.value<float>();
                    break;
                }
                case NC_DOUBLE:
                {
                    json["value"] = attribute.value<double>();
                    break;
                }
                default:
                {
                    json["value"] = "TODO";
                    break;
                }
            }
        }
    }


    void to_json(nlohmann::ordered_json& json, Variable const& variable)
    {
        nlohmann::ordered_json attributes_json{};

        std::ranges::transform(
            variable.attributes(),
            std::back_inserter(attributes_json),
            [](auto const& attribute) { return attribute; });

        json = nlohmann::ordered_json{
            {"name", variable.name()},
            {"attributes", attributes_json},
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
            group.groups(), std::back_inserter(groups_json), [](auto const& group) { return group; });

        json = nlohmann::ordered_json{
            {"name", group.name()},
            {"attributes", attributes_json},
            {"variables", variables_json},
            {"groups", groups_json},
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
