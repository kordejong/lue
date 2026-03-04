#include "lue/document/example.hpp"


namespace lue::document {

    Example::Example(cxxopts::Options&& options, int argc, char const* const* argv):

        utility::Application{std::move(options), argc, argv}

    {
    }

}  // namespace lue::document
