#include "lue/cf/dataset.hpp"
#include "lue/version.hpp"


namespace lue::cf {

    auto Dataset::create(std::string const& name, int const create_mode) -> Dataset
    {
        auto dataset = netcdf::Dataset::create(name, create_mode);

        dataset.add_attribute("Conventions", std::string{"CF-1.11"});
        dataset.add_attribute("history", std::string{std::format("LUE-{}", version())});

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

}  // namespace lue::cf
