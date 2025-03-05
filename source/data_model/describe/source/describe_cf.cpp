#include "describe_cf.hpp"
#include "lue/cf.hpp"
#include "lue/netcdf4/type.hpp"
#include <algorithm>


namespace lue::cf {
    // namespace {

    //     // // void to_json(nlohmann::ordered_json& json, netcdf4::Attribute const& attribute)
    //     // // {
    //     // //     json = nlohmann::ordered_json{
    //     // //         {"name", attribute.name()},
    //     // //         {"length", attribute.length()},
    //     // //         {"type", netcdf4::as_string(attribute.type())},
    //     // //     };
    //     // // }


    //     auto dimension_kind_as_string(Dimension::Kind const kind)
    //     {
    //         std::string result;

    //         switch (kind)
    //         {
    //             case Dimension::Kind::latitude:
    //             {
    //                 result = "latitude";
    //                 break;
    //             }
    //             case Dimension::Kind::longitude:
    //             {
    //                 result = "longitude";
    //                 break;
    //             }
    //             case Dimension::Kind::time:
    //             {
    //                 result = "time";
    //                 break;
    //             }
    //             case Dimension::Kind::unknown:
    //             {
    //                 result = "unknown";
    //                 break;
    //             }
    //         }

    //         return result;
    //     }


    //     auto variable_kind_as_string(Variable::Kind const kind)
    //     {
    //         std::string result;

    //         switch (kind)
    //         {
    //             case Variable::Kind::coordinate:
    //             {
    //                 result = "coordinate";
    //                 break;
    //             }
    //             case Variable::Kind::regular:
    //             {
    //                 result = "regular";
    //                 break;
    //             }
    //         }

    //         return result;
    //     }


    //     void to_json(nlohmann::ordered_json& json, Dimension const& dimension)
    //     {
    //         json = nlohmann::ordered_json{
    //             {"name", dimension.name()},
    //             {"length", dimension.length()},
    //             {"kind", dimension_kind_as_string(dimension.kind())},
    //             {"is_spatiotemporal", dimension.is_spatiotemporal()},
    //         };
    //     }


    //     void add_attribute(netcdf4::Attribute const& attribute, nlohmann::ordered_json& json)
    //     {
    //         nlohmann::ordered_json::value_type value{};

    //         if (attribute.type() == NC_CHAR || attribute.length() == 1)
    //         {
    //             switch (attribute.type())
    //             {
    //                 case NC_CHAR:
    //                 {
    //                     value = attribute.value();
    //                     break;
    //                 }
    //                 case NC_BYTE:
    //                 {
    //                     value = attribute.value<std::int8_t>();
    //                     break;
    //                 }
    //                 case NC_UBYTE:
    //                 {
    //                     value = attribute.value<std::uint8_t>();
    //                     break;
    //                 }
    //                 case NC_INT:
    //                 {
    //                     value = attribute.value<std::int32_t>();
    //                     break;
    //                 }
    //                 case NC_UINT:
    //                 {
    //                     value = attribute.value<std::uint32_t>();
    //                     break;
    //                 }
    //                 case NC_INT64:
    //                 {
    //                     value = attribute.value<std::int64_t>();
    //                     break;
    //                 }
    //                 case NC_UINT64:
    //                 {
    //                     value = attribute.value<std::uint64_t>();
    //                     break;
    //                 }
    //                 case NC_FLOAT:
    //                 {
    //                     value = attribute.value<float>();
    //                     break;
    //                 }
    //                 case NC_DOUBLE:
    //                 {
    //                     value = attribute.value<double>();
    //                     break;
    //                 }
    //                 default:
    //                 {
    //                     value = "TODO";
    //                     break;
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             value = "TODO";
    //         }

    //         json.push_back({attribute.name(), value});
    //     }


    //     void to_json(nlohmann::ordered_json& json, Variable const& variable)
    //     {
    //         nlohmann::ordered_json dimensions_json{};

    //         for (auto const& dimension : variable.dimensions())
    //         {
    //             nlohmann::ordered_json dimension_json{};
    //             to_json(dimension_json, dimension);
    //             dimensions_json.push_back(dimension_json);
    //         }

    //         auto attributes_json = nlohmann::ordered_json{
    //             {"standard_name", variable.standard_name()},
    //             {"long_name", variable.long_name()},
    //             {"units", variable.units()},
    //             {"kind", variable_kind_as_string(variable.kind())},
    //         };

    //         switch (variable.kind())
    //         {
    //             case Variable::Kind::regular:
    //             {
    //                 if (variable.has_attribute("missing_value"))
    //                 {
    //                     add_attribute(variable.attribute("missing_value"), attributes_json);
    //                 }

    //                 break;
    //             }
    //             case Variable::Kind::coordinate:
    //             {
    //                 if (variable.has_attribute("axis"))
    //                 {
    //                     attributes_json.update(nlohmann::json{{"axis",
    //                     variable.attribute("axis").value()}});
    //                 }

    //                 if (variable.has_attribute("positive"))
    //                 {
    //                     attributes_json.update(
    //                         nlohmann::json{{"positive", variable.attribute("positive").value()}});
    //                 }

    //                 break;
    //             }
    //         }

    //         json = nlohmann::ordered_json{
    //             {"name", variable.name()},
    //             {"type", netcdf4::as_string(variable.type())},
    //             {"attributes", attributes_json},
    //             {"dimensions", dimensions_json},
    //         };
    //     }


    //     void to_json(nlohmann::ordered_json& json, Group const& group)
    //     {
    //         // nlohmann::ordered_json attributes_json{};

    //         // for(auto const& attribute: group.attributes())
    //         // {
    //         //     nlohmann::ordered_json attribute_json{};
    //         //     to_json(attribute_json, attribute);
    //         //     attributes_json.push_back(attribute_json);
    //         // }

    //         nlohmann::ordered_json variables_json{};

    //         for (auto const& variable : group.variables())
    //         {
    //             nlohmann::ordered_json variable_json{};
    //             to_json(variable_json, variable);
    //             variables_json.push_back(variable_json);
    //         }

    //         nlohmann::ordered_json groups_json{};

    //         for (auto const& group : group.child_groups())
    //         {
    //             nlohmann::ordered_json group_json{};
    //             to_json(group_json, group);
    //             groups_json.push_back(group_json);
    //         }

    //         json = nlohmann::ordered_json{
    //             {"name", group.name()},
    //             // {"attributes", attributes_json},
    //             {"variables", variables_json},
    //             {"child_groups", groups_json},
    //         };
    //     }

    // }  // Anonymous namespace


    // void to_json(nlohmann::ordered_json& json, Dataset const& dataset)
    // {
    //     // High-level stuff, related to CF
    //     json = nlohmann::ordered_json{
    //         {"CF version", dataset.version()},
    //         {"title", dataset.title()},
    //         {"history", dataset.history()},
    //         {"institution", dataset.institution()},
    //         {"source", dataset.source()},
    //         {"comment", dataset.comment()},
    //         {"references", dataset.references()},
    //     };

    //     nlohmann::ordered_json group_json{};
    //     to_json(group_json, dynamic_cast<netcdf4::Group const&>(dataset));

    //     json.update(group_json);
    // }

}  // namespace lue::cf
