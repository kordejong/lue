#pragma once
#include "detail/type_name.hpp"
#include "lue/framework/api/cxx/field.hpp"
#include "lue/string.hpp"
#include <concepts>
#include <format>
#include <initializer_list>
#include <string>


namespace lue::api::detail {

    template<std::convertible_to<std::string_view>... ArgumentTypeNames>
    void throw_unsupported_overload_exception(
        std::string_view const& name, ArgumentTypeNames const&... type_names)
    {
        throw std::runtime_error{std::format(
            "Unsupported C++ overload called: {}({})",
            name,
            join(std::initializer_list<std::string_view>{type_names...}, ", "))};
    }


    template<typename... Arguments>
    void unsupported_overload(std::string const& name, Arguments const&... arguments)
    {
        throw_unsupported_overload_exception(name, type_name(arguments)...);
    }

}  // namespace lue::api::detail
