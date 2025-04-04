#include "describe_cf.hpp"
#include "lue/cf.hpp"
#include <algorithm>


namespace lue::cf {
    namespace {

        // // void to_json(nlohmann::ordered_json& json, netcdf::Attribute const& attribute)
        // // {
        // //     json = nlohmann::ordered_json{
        // //         {"name", attribute.name()},
        // //         {"length", attribute.length()},
        // //         {"type", netcdf::as_string(attribute.type())},
        // //     };
        // // }


        auto dimension_kind_as_string(Dimension::Kind const kind)
        {
            std::string result;

            switch (kind)
            {
                case Dimension::Kind::latitude:
                {
                    result = "latitude";
                    break;
                }
                case Dimension::Kind::longitude:
                {
                    result = "longitude";
                    break;
                }
                case Dimension::Kind::time:
                {
                    result = "time";
                    break;
                }
                case Dimension::Kind::unknown:
                {
                    result = "unknown";
                    break;
                }
            }

            return result;
        }


        auto variable_kind_as_string(Variable::Kind const kind)
        {
            std::string result;

            switch (kind)
            {
                case Variable::Kind::coordinate:
                {
                    result = "coordinate";
                    break;
                }
                case Variable::Kind::regular:
                {
                    result = "regular";
                    break;
                }
            }

            return result;
        }


        void to_json(nlohmann::ordered_json& json, Dimension const& dimension)
        {
            json = nlohmann::ordered_json{
                {"name", dimension.name()},
                {"length", dimension.length()},
                {"kind", dimension_kind_as_string(dimension.kind())},
                {"is_spatiotemporal", dimension.is_spatiotemporal()},
            };
        }


        void to_json(nlohmann::ordered_json& json, Variable const& variable)
        {
            nlohmann::ordered_json dimensions_json{};

            for (auto const& dimension : variable.dimensions())
            {
                nlohmann::ordered_json dimension_json{};
                to_json(dimension_json, dimension);
                dimensions_json.push_back(dimension_json);
            }

            json = nlohmann::ordered_json{
                {"standard_name", variable.standard_name()},
                {"long_name", variable.long_name()},
                {"units", variable.units()},
                {"kind", variable_kind_as_string(variable.kind())},
                {"dimensions", dimensions_json},
            };
        }


        void to_json(nlohmann::ordered_json& json, Group const& group)
        {
            // nlohmann::ordered_json attributes_json{};

            // for(auto const& attribute: group.attributes())
            // {
            //     nlohmann::ordered_json attribute_json{};
            //     to_json(attribute_json, attribute);
            //     attributes_json.push_back(attribute_json);
            // }

            nlohmann::ordered_json variables_json{};

            for (auto const& variable : group.variables())
            {
                nlohmann::ordered_json variable_json{};
                to_json(variable_json, variable);
                variables_json.push_back(variable_json);
            }

            nlohmann::ordered_json groups_json{};

            for (auto const& group : group.child_groups())
            {
                nlohmann::ordered_json group_json{};
                to_json(group_json, group);
                groups_json.push_back(group_json);
            }

            json = nlohmann::ordered_json{
                {"name", group.name()},
                // {"attributes", attributes_json},
                {"variables", variables_json},
                {"child_groups", groups_json},
            };
        }

    }  // Anonymous namespace


    void to_json(nlohmann::ordered_json& json, Dataset const& dataset)
    {
        // High-level stuff, related to CF
        json = nlohmann::ordered_json{
            {"CF version", dataset.version()},
            {"title", dataset.title()},
            {"history", dataset.history()},
            {"institution", dataset.institution()},
            {"source", dataset.source()},
            {"comment", dataset.comment()},
            {"references", dataset.references()},
        };

        nlohmann::ordered_json group_json{};
        to_json(group_json, dynamic_cast<netcdf::Group const&>(dataset));

        json.update(group_json);
    }

}  // namespace lue::cf
