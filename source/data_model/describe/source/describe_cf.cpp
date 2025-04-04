#include "describe_cf.hpp"
#include "lue/cf.hpp"
#include <algorithm>


namespace lue::cf {
    namespace {

        // void to_json(nlohmann::ordered_json& json, netcdf::Attribute const& attribute)
        // {
        //     json = nlohmann::ordered_json{
        //         {"name", attribute.name()},
        //         {"length", attribute.length()},
        //         {"type", netcdf::as_string(attribute.type())},
        //     };
        // }


        auto variable_kind_as_string(cf::Variable::Kind const kind)
        {
            std::string result;

            switch (kind)
            {
                case cf::Variable::Kind::coordinate:
                {
                    result = "coordinate";
                    break;
                }
                case cf::Variable::Kind::regular:
                {
                    result = "regular";
                    break;
                }
            }

            return result;
        }


        void to_json(nlohmann::ordered_json& json, netcdf::Dimension const& dimension)
        {
            json = nlohmann::ordered_json{
                {"name", dimension.name()},
                {"length", dimension.length()},
                {"is_time", cf::Dimension::is_time(dimension)},
                {"is_latitude", cf::Dimension::is_latitude(dimension)},
                {"is_longitude", cf::Dimension::is_longitude(dimension)},
                {"is_spatiotemporal", cf::Dimension::is_spatiotemporal(dimension)},
            };
        }


        void to_json(nlohmann::ordered_json& json, netcdf::Variable const& variable)
        {
            nlohmann::ordered_json dimensions_json{};

            for (auto const& dimension : variable.dimensions())
            {
                nlohmann::ordered_json dimension_json{};
                to_json(dimension_json, dimension);
                dimensions_json.push_back(dimension_json);
            }

            json = nlohmann::ordered_json{
                {"standard_name", variable.attribute("standard_name").value("")},
                {"long_name", variable.attribute("long_name").value("")},
                {"units", variable.attribute("units").value()},  // TODO udunits
                {"kind", variable_kind_as_string(cf::Variable::kind(variable))},
                {"dimensions", dimensions_json},
            };
        }


        void to_json(nlohmann::ordered_json& json, netcdf::Group const& group)
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
            // TODO Parse version number
            {"CF version", dataset.version()},
            {"title", dataset.attribute("title").value("")},
            {"history", dataset.attribute("history").value("")},
            {"institution", dataset.attribute("institution").value("")},
            {"source", dataset.attribute("source").value("")},
            {"comment", dataset.attribute("comment").value("")},
            {"references", dataset.attribute("references").value("")},
        };

        // We can't add alternative to_json functions in the netcdf namespace so we call to_json explicitlyly
        nlohmann::ordered_json group_json{};
        to_json(group_json, dynamic_cast<netcdf::Group const&>(dataset));

        json.update(group_json);
    }

}  // namespace lue::cf
