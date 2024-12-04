#pragma once
#include "lue/cf.hpp"
#include <string>
#include <vector>


namespace lue::utility {

    auto describe_cf_dataset(cf::Dataset const& dataset) -> std::vector<std::string>;

}  // namespace lue::utility
