#include "describe_cf.hpp"
#include "lue/netcdf4/type.hpp"
#include <boost/algorithm/string/join.hpp>
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


    // void to_json(nlohmann::ordered_json& json, netcdf4::Group const& group)
    // {
    //     json = nlohmann::ordered_json{};

    //     // TODO if(!group.is_root())
    //     {
    //         // Optional for non-root groups
    //         nlohmann::ordered_json attributes_json{
    //             {"title", group.attribute("title").value("")},
    //             {"history", group.attribute("history").value("")},
    //         };

    //         json["attributes"] = attributes_json;
    //     }

    //     // for(auto const& attribute: group.attributes())
    //     // {
    //     //     nlohmann::ordered_json attribute_json{};
    //     //     to_json(attribute_json, attribute);
    //     //     attributes_json.push_back(attribute_json);
    //     // }

    //     //         nlohmann::ordered_json variables_json{};

    //     //         for (auto const& variable : group.variables())
    //     //         {
    //     //             nlohmann::ordered_json variable_json{};
    //     //             to_json(variable_json, variable);
    //     //             variables_json.push_back(variable_json);
    //     //         }

    //     //         nlohmann::ordered_json groups_json{};

    //     //         for (auto const& group : group.child_groups())
    //     //         {
    //     //             nlohmann::ordered_json group_json{};
    //     //             to_json(group_json, group);
    //     //             groups_json.push_back(group_json);
    //     //         }

    //     //         json = nlohmann::ordered_json{
    //     //             {"name", group.name()},
    //     //             // {"attributes", attributes_json},
    //     //             {"variables", variables_json},
    //     //             {"child_groups", groups_json},
    //     //         };
    // }


    void to_json([[maybe_unused]] nlohmann::ordered_json& json, [[maybe_unused]] Domain::Axis const& axis)
    {
        // TODO
    }


    void to_json(nlohmann::ordered_json& json, Domain const& domain)
    {
        nlohmann::ordered_json axes_json{};

        std::ranges::transform(
            domain.axes(), std::back_inserter(axes_json), [](auto const& axis) { return axis; });

        json = nlohmann::ordered_json{
            {"axes", axes_json},
        };
    }


    // void to_json(nlohmann::ordered_json& json, Field::Property const& property)
    // {
    //     json = nlohmann::ordered_json{
    //         {"name", property.name()},
    //         {"value", property.value()},
    //     };
    // }


    void to_json(nlohmann::ordered_json& json, Field const& field)
    {
        json = nlohmann::ordered_json{
            {"name", field.name()},
        };

        if (field.has_attribute("standard_name"))
        {
            json["standard_name"] = field.attribute("standard_name").value();
        }

        if (field.has_attribute("long_name"))
        {
            json["long_name"] = field.attribute("long_name").value();
        }

        if (field.has_attribute("units"))
        {
            json["units"] = field.attribute("units").value();
        }

        if (field.has_domain())
        {
            json["domain"] = field.domain();
        }

        {
            nlohmann::ordered_json properties_json{};

            for (auto const& [name, property] : field.properties())
            {
                properties_json[name] = property.value();
            }

            json["properties"] = properties_json;
        }
    }

    // }  // Anonymous namespace


    void to_json(nlohmann::ordered_json& json, Dataset const& dataset)
    {
        // High-level stuff, related to CF
        // TODO external_variables


        // * --------------------------------------
        // * Field: temperature (ncvar%temperature)
        // * --------------------------------------
        // * CDI = 'Climate Data Interface version 1.9.6 (http://mpimet.mpg.de/cdi)'
        // * CDO = 'Climate Data Operators version 1.9.6 (http://mpimet.mpg.de/cdo)'
        // * Conventions = 'CF-1.4'
        // _FillValue = np.float32(1e+20)
        // * comment = 'This file is part of the input files for the PCR-GLOBWB model runs
        // *            underlying the publication "PCR-GLOBWB 2: a 5 arcmin global
        // *            hydrological and water resources model" (Sutanudjaja et al., 2018,
        // *            https://doi.org/10.5194/gmd-11-2429-2018).'
        // * description = 'None'
        // * history = 'Mon Nov 11 11:25:28 2019: cdo -L -f nc4 -copy /quanta1/home/hydrowld
        // *            /data/hydroworld/pcrglobwb2_input_release/version_2019_10_beta_1_ext
        // *            ended/pcrglobwb2_input/global_30min/meteo/forcing/daily_temperature_
        // *            cru_era-interim_2000_to_2010.nc /scratch/depfg/sutan101/data/pcrglob
        // *            wb2_input_release/version_2019_11_beta_without_compression/pcrglobwb
        // *            2_input/global_30min/meteo/forcing/daily_temperature_cru_era-
        // *            interim_2000_to_2010.nc\nSun Nov 05 16:23:48 2017: cdo -f nc4 -z
        // *            szip -selyear,2000/2010 daily_temperature_cruts321_era-40_era-
        // *            interim_1958-2010_cruts324_era-interim_2011_to_2015.nc
        // *            daily_temperature_cruts321_era-interim_2000_to_2010.nc4\nMon Jul 17
        // *            01:01:01 2017: cdo mergetime
        // *            /projects/0/dfguu/data/hydroworld/forcing/CRU-
        // *            TS3.21/merged_1958_to_2010/CRU-TS3.21_ERA-40_ERA-
        // *            Interim_daily_temperature_1958_to_2010.nc
        // *            /projects/0/dfguu/data/hydroworld/forcing/ERA-Interim-
        // *            GPCPCorrected/cruts324_erai_daily_temperature_2011_to_2015.nc
        // *            daily_temperature_cruts321_era-40_era-
        // *            interim_1958-2010_cruts324_era-interim_2011_to_2015.nc\nNone'
        // * institution = 'Utrecht University, Department of Physical Geography'
        // * long_name = 'temperature'
        // * missing_value = np.float32(1e+20)
        // * references = 'None'
        // * source = 'None'
        // * standard_name = 'temperature'
        // * title = 'Forcing dataset derived from CRU TS3.21 and ERA40'
        // * units = 'degrees Celsius'
        //
        // Data(time(4018), latitude(360), longitude(720)) = [[[-10.801055908203125, ...,
        // -25.279266357421875]]] degrees Celsius
        //
        // Domain Axis: latitude(360)
        // Domain Axis: longitude(720)
        // Domain Axis: time(4018)
        //
        // Dimension coordinate: time
        //     axis = 'T'
        //     calendar = 'standard'
        //     long_name = 'Days since 1901-01-01'
        //     standard_name = 'time'
        //     units = 'days since 1901-01-01 00:00:00'
        //     Data(time(4018)) = [2000-01-01 00:00:00, ..., 2010-12-31 00:00:00] standard
        //
        // Dimension coordinate: latitude
        //     axis = 'Y'
        //     long_name = 'latitude'
        //     standard_name = 'latitude'
        //     units = 'degrees_north'
        //     Data(latitude(360)) = [89.75, ..., -89.75] degrees_north
        //
        // Dimension coordinate: longitude
        //     axis = 'X'
        //     long_name = 'longitude'
        //     standard_name = 'longitude'
        //     units = 'degrees_east'
        //     Data(longitude(720)) = [-179.75, ..., 179.75] degrees_east

        nlohmann::ordered_json fields_json{};

        std::ranges::transform(
            dataset.fields(), std::back_inserter(fields_json), [](auto const& field) { return field; });

        json = nlohmann::ordered_json{
            // {"CF version", dataset.version()},
            // {"conventions", boost::algorithm::join(dataset.conventions(), ", ")},
            // {"title", dataset.attribute("title").value("")},
            // {"history", dataset.attribute("history").value("")},
            // {"institution", dataset.attribute("institution").value("")},
            // {"source", dataset.attribute("source").value("")},
            // {"comment", dataset.attribute("comment").value("")},
            // {"references", dataset.attribute("references").value("")},
            {"fields", fields_json},
        };


        // nlohmann::ordered_json group_json{};
        // to_json(group_json, dynamic_cast<netcdf4::Group const&>(dataset));

        // if (!group_json.empty())
        // {
        //     json.update(group_json);
        // }
    }

}  // namespace lue::cf
