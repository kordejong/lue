#include "lue/utility/print_message.hpp"
#include "lue/string.hpp"


namespace lue::utility {

    void print_message(std::ostream& stream, std::string const& message)
    {
        stream << trim(std::string_view{message}) << std::endl;
    }


    void print_info_message(std::ostream& stream, std::string const& message)
    {
        print_message(stream, /* "info: " + */ message);
    }


    void print_info_message(bool const verbose, std::ostream& stream, std::string const& message)
    {
        if (verbose)
        {
            print_info_message(stream, message);
        }
    }


    void print_error_message(std::ostream& stream, std::string const& message)
    {
        print_message(stream, "error: " + message);
    }

}  // namespace lue::utility
