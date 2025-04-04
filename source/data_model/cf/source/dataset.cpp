#include "lue/cf/dataset.hpp"
#include "lue/version.hpp"
#include <regex>

#include <iostream>


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

        return dataset.release();
    }


    Dataset::Dataset(int const dataset_id):

        netcdf::Dataset{dataset_id}

    {
    }


    Dataset::Dataset(netcdf::Dataset&& dataset):

        Dataset{dataset.release()}

    {
    }


    auto Dataset::version() const -> double
    {
        auto const convention{attribute("Conventions").value()};

        // ... CF-<double> ...

        std::regex const version_regex{R"(CF-\d+[.]\d+)"};
        std::smatch version_match{};

        if (!std::regex_search(convention, version_match, version_regex))
        {
            throw std::runtime_error("Could not find CF version");
        }

        // TODO
        // auto const version_as_string = version_match.str();
        // std::cout << "YEAH: version: " << version_as_string << std::endl;
        // std::cout << "YEAH: version: " << version_match[0] << std::endl;
        // std::cout << "YEAH: version: " << version_match[1] << std::endl;

        return 0.0;
    }

}  // namespace lue::cf
