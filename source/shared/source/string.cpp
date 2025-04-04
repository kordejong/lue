#include "lue/string.hpp"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


namespace lue {

    auto split_string(std::string const& string, std::string::value_type token) -> std::vector<std::string>
    {
        std::vector<std::string> items{};

        boost::split(items, string, boost::is_any_of(std::string{token}));

        return items;
    }

}  // namespace lue
