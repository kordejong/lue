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


    auto trim_left(std::string_view view) -> std::string_view
    {
        std::size_t nr_white_space_chars{0};

        for (auto char_it = view.begin(); char_it != view.end() && std::isspace(*char_it) != 0; ++char_it)
        {
            ++nr_white_space_chars;
        }

        if (nr_white_space_chars > 0)
        {
            view.remove_prefix(nr_white_space_chars);
        }

        return view;
    }


    auto trim_right(std::string_view view) -> std::string_view
    {
        std::size_t nr_white_space_chars{0};

        for (auto char_it = view.rbegin(); char_it != view.rend() && std::isspace(*char_it) != 0; ++char_it)
        {
            ++nr_white_space_chars;
        }

        if (nr_white_space_chars > 0)
        {
            view.remove_suffix(nr_white_space_chars);
        }

        return view;
    }


    auto trim(std::string_view view) -> std::string_view
    {
        return trim_right(trim_left(view));
    }

}  // namespace lue
