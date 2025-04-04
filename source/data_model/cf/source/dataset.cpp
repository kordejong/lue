#include "lue/cf/dataset.hpp"
#include "lue/version.hpp"
#include <regex>


namespace lue::cf {

    auto Dataset::create(std::string const& name, int const create_mode) -> Dataset
    {
        auto dataset = netcdf4::Dataset::create(name, create_mode);

        dataset.set_conventions({"CF-1.11"});
        dataset.add_attribute("history", std::string{std::format("LUE-{}", lue::version())});

        return dataset;
    }


    auto Dataset::open(std::string const& name, int const open_mode) -> Dataset
    {
        auto dataset = netcdf4::Dataset::open(name, open_mode);

        if ((!dataset.has_attribute("Conventions")) ||
            dataset.attribute("Conventions").value().find("CF-") == std::string::npos)
        {
            throw std::runtime_error(std::format("Dataset {} does not conform to the CF convention", name));
        }

        return netcdf4::Dataset{dataset.release()};
    }


    Dataset::Dataset(netcdf4::Dataset&& dataset):

        netcdf4::Dataset{std::move(dataset)}

    {
    }


    /*!
        @brief      Return the version of the CF convention used to write the data set
    */
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

}  // namespace lue::cf
