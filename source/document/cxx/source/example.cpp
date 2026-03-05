#include "lue/document/example.hpp"
#include "lue/gdal.hpp"


namespace lue::document {

    Example::Example(cxxopts::Options&& options, int argc, char const* const* argv):

        utility::Application{
            [](cxxopts::Options&& options) -> cxxopts::Options
            {
                // Stuff common to all example's options
                options.custom_help("");
                options.add_options()
                    // clang-format off
                    ("h,help", "Show usage")
                    ("v,version", "Show version")
                    // clang-format on
                    ;

                return std::move(options);
            }(std::move(options)),
            argc,
            argv}

    {
        lue::gdal::register_gdal_drivers();
    }

}  // namespace lue::document
