#pragma once
#include "lue/cf/dataset.hpp"
#include <nlohmann/json.hpp>


namespace lue::cf {

    void to_json(nlohmann::ordered_json& json, Dataset const& dataset);

}  // namespace lue::cf
