#include "describe_netcdf4.hpp"
#include "lue/string.hpp"
#include <format>


namespace lue::utility {

    auto describe_netcdf4_dataset(netcdf::Dataset const& dataset) -> std::vector<std::string>
    {
        std::vector<std::string> description{};

        description.push_back(std::format("Dataset: {}", dataset.path()));
        description.push_back(
            std::format("File format: {}", netcdf::Dataset::format_as_string(dataset.format())));

        // TODO Refactor with describe_cf_dataset

        return description;
    }

}  // namespace lue::utility
