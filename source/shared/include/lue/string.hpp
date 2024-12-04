#pragma once
#include <string>
#include <vector>


namespace lue {

    auto split_string(std::string const& string, std::string::value_type token) -> std::vector<std::string>;

}
