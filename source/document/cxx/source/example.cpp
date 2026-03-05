#include "lue/document/example.hpp"
#include "lue/gdal.hpp"


namespace lue::document {

    Example::Example(cxxopts::Options&& options, int argc, char const* const* argv):

        utility::Application{std::move(options), argc, argv}

    {
        lue::gdal::register_gdal_drivers();
    }

}  // namespace lue::document
