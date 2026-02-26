#pragma once
#include "lue/data_model.hpp"
#include "lue/utility/metadata.hpp"
#include <nlohmann/json.hpp>


namespace lue::utility {

    auto translate_json_to_lue(nlohmann::json const& json, std::string const& dataset_name)
        -> data_model::Dataset;

    void translate_json_to_lue(nlohmann::json const& json, std::string const& lue_pathname, bool add);

    void translate_json_to_lue(
        std::string const& json_pathname,
        std::string const& lue_pathname,
        bool add,
        Metadata const& metadata);

}  // namespace lue::utility
