#include "lue/cf/dataset.hpp"
#include "lue/version.hpp"
#include <regex>


namespace lue::cf {

    auto Dataset::create(std::string const& name, int const create_mode) -> Dataset
    {
        auto dataset = netcdf::Dataset::create(name, create_mode);

        dataset.add_attribute("Conventions", std::string{"CF-1.11"});
        dataset.add_attribute("history", std::string{std::format("LUE-{}", lue::version())});

        return dataset;
    }


    auto Dataset::open(std::string const& name, int const open_mode) -> Dataset
    {
        auto dataset = netcdf::Dataset::open(name, open_mode);

        if ((!dataset.has_attribute("Conventions")) ||
            dataset.attribute("Conventions").value().find("CF-") == std::string::npos)
        {
            throw std::runtime_error(std::format("Dataset {} does not conform to the CF convention", name));
        }

        return netcdf::Dataset{dataset.release()};
    }


    Dataset::Dataset(netcdf::Dataset&& dataset):

        netcdf::Dataset{std::move(dataset)}

    {
    }


    auto Dataset::version() const -> double
    {
        auto const convention{attribute("Conventions").value()};

        // ... CF-<double> ...
        std::regex const version_regex{"CF-([0-9]+[.][0-9]+)"};
        std::smatch version_match{};

        if (!std::regex_search(convention, version_match, version_regex))
        {
            throw std::runtime_error("Could not find CF version");
        }

        return std::stod(version_match[1].str());
    }


    auto Dataset::title() const -> std::string
    {
        return attribute("title").value("");
    }


    auto Dataset::history() const -> std::string
    {
        return attribute("history").value("");
    }


    auto Dataset::institution() const -> std::string
    {
        return attribute("institution").value("");
    }


    auto Dataset::source() const -> std::string
    {
        return attribute("source").value("");
    }


    auto Dataset::comment() const -> std::string
    {
        return attribute("comment").value("");
    }


    auto Dataset::references() const -> std::string
    {
        return attribute("references").value("");
    }

}  // namespace lue::cf
