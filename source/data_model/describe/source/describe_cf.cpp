#include "describe_cf.hpp"
#include "lue/string.hpp"
#include <format>


namespace lue::utility {

    namespace {


        void add_separation(std::vector<std::string>& description)
        {
            description.emplace_back("----------"
                                     "----------"
                                     "----------"
                                     "----------");
        }


        void describe_attribute(
            // cf::Dataset const& dataset,
            auto const& dataset,
            std::string const& name,
            std::vector<std::string>& description,
            std::optional<std::string> const& default_value = {})
        {
            std::string value = dataset.attribute(name).value(default_value);

            description.push_back(std::format("{}: {}", name, value));
        }


        void attribute_value_list(
            cf::Dataset const& dataset,
            std::string const& name,
            std::vector<std::string>& description,
            std::optional<std::string> const& default_value = {})
        {
            std::string value = dataset.attribute(name).value(default_value);

            description.push_back(std::format("{}:", name));

            for (auto const& item : split_string(value, '\n'))
            {
                description.push_back(std::format("- {}", item));
            }
        }


        void describe_variable(netcdf::Variable const& variable, std::vector<std::string>& description)
        {
            description.push_back(std::format("Variable: {}", variable.name()));

            describe_attribute(variable, "units", description, "");
            describe_attribute(variable, "long_name", description, "");
            describe_attribute(variable, "standard_name", description, "");

            // describe_attribute(variable, "bounds", description, "");
        }


        void describe_group(netcdf::Group const& group, std::vector<std::string>& description)
        {
            description.push_back(std::format("Group: {}", group.name()));

            // describe_attribute(variable, "units", description, "");
            // describe_attribute(variable, "long_name", description, "");
            // describe_attribute(variable, "standard_name", description, "");
            // // describe_attribute(variable, "bounds", description, "");
        }

    }  // Anonymous namespace


    auto describe_cf_dataset(cf::Dataset const& dataset) -> std::vector<std::string>
    {
        std::vector<std::string> description{};

        description.push_back(std::format("Dataset: {}", dataset.path()));
        description.emplace_back("File format: netCDF-4");

        describe_attribute(dataset, "Conventions", description);
        describe_attribute(dataset, "title", description, "");
        attribute_value_list(dataset, "history", description, "");
        describe_attribute(dataset, "institution", description, "");
        describe_attribute(dataset, "source", description, "");
        describe_attribute(dataset, "comment", description, "");
        describe_attribute(dataset, "references", description, "");

        for (auto const& variable : dataset.variables())
        {
            add_separation(description);
            describe_variable(variable, description);
        }


        // TODO? Dataset is also a group
        for (auto const& group : dataset.groups())
        {
            add_separation(description);
            describe_group(group, description);
        }

        return description;
    }

}  // namespace lue::utility
