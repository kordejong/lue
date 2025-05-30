#include "lue/view/application.hpp"
#include "lue/hdf5.hpp"
#include "lue/imgui.hpp"
#include <format>


// - Visualize property values
//     - time domain
//         - time boxes
//         - time cells
//         - time points
//     - space domain
//         - space points
//             - points map
//         - space boxes
//             - boxes map
//     - id
//     - properties
//         - time series graph
//         - raster map
//         - scalars table
// - Interface
//     - Navigator, showing an overview (layout, hierarchy) of a dataset
//     - View, visualizing a piece of information
//         - time domain
//         - space domain
//         - property
// - Make it possible to visualize multiple pieces of information at the
//   same time. If they are part of the same property-set, set up
//   a link/cursor/... E.g. selecting a space point should show the
//   time series graph of a selected property for that point.
//   Also set up links between stuff in the same phenomenon. Set up links
//   between as much stuff as possible.


namespace lue::view {

    Application::Application(int const argc, char const* const* argv):

        utility::Application{
            []()
            {
                cxxopts::Options options{"lue_view", "View LUE datasets"};
                options.add_options()("h,help", "Show usage")("v,version", "Show version")(
                    "dataset", "Input dataset(s)", cxxopts::value<std::vector<std::string>>());
                options.parse_positional({"dataset"});
                options.positional_help("<dataset>");
                options.show_positional_help();
                return options;
            }(),
            argc,
            argv}

    {
        // Turn off error stack traversal. The default functions prints
        // lots of messages we usually don't care about.
        H5Eset_auto(H5E_DEFAULT, nullptr, nullptr);
    }

}  // namespace lue::view
