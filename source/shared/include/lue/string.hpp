#pragma once
#include <string>
#include <vector>


namespace lue {

    auto split_string(std::string const& string, std::string::value_type token) -> std::vector<std::string>;

    auto trim_left(std::string_view view) -> std::string_view;

    auto trim_right(std::string_view view) -> std::string_view;

    auto trim(std::string_view view) -> std::string_view;

}  // namespace lue
