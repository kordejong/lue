#include "lue/glfw/library.hpp"
#include <fmt/format.h>
#include <GLFW/glfw3.h>
#include <stdexcept>


namespace lue::glfw {

    static void glfw_error_callback(int const error, char const* description)
    {
        throw std::runtime_error(fmt::format("GLFW error {}: {})", error, description));
    }


    /*!
        @brief      Return the version of the runtime library
    */
    std::tuple<int, int, int> Library::version()
    {
        int major, minor, revision;

        glfwGetVersion(&major, &minor, &revision);

        return {major, minor, revision};
    }


    /*!
        @brief      Return the version of the compile-time library
    */
    std::string Library::version_string()
    {
        static std::string const string{glfwGetVersionString()};

        return string;
    }


    void Library::hint(int hint, int value)
    {
        glfwInitHint(hint, value);
    }


    Library::Library()
    {
        glfwSetErrorCallback(glfw_error_callback);
        glfwInit();
    }


    Library::~Library()
    {
        glfwTerminate();
    }

}  // namespace lue::glfw
