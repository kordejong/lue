#pragma once
#include <sstream>
#include <string>
#include <vector>


namespace lue {

    template<typename Collection>
    auto join(Collection const& elements, std::string const& separator) -> std::string
    {
        std::ostringstream stream{};

        if (!std::empty(elements))
        {
            auto element_it = std::begin(elements);

            stream << *element_it;

            for (++element_it; element_it != std::end(elements); ++element_it)
            {
                stream << separator << *element_it;
            }
        }

        return stream.str();
    }

    auto split_string(std::string const& string, std::string::value_type token) -> std::vector<std::string>;

}  // namespace lue
