#pragma once
#include "lue/framework/algorithm/kernel.hpp"
#include "lue/framework/io/gdal.hpp"
#include "lue/document/export.hpp"
#include "lue/gdal.hpp"
#include "lue/utility/application.hpp"


namespace lue::document {

    class LUE_DOCUMENT_EXPORT Example: public utility::Application
    {

        public:

            Example(cxxopts::Options&& options, int argc, char const* const* argv);

        protected:

            template<Arithmetic Weight, Rank rank>
            auto read_kernel(std::string const& pathname) -> Kernel<Weight, rank>
            {
                // NOTE: Read the kernel weights from a dataset. In real-life, these weights will likely be
                //       literal values stored in the code.
                gdal::Raster raster = gdal::Raster{gdal::open_dataset(pathname, GDALAccess::GA_ReadOnly)};
                auto const shape = raster.shape();

                Kernel<Weight, rank> kernel{from_gdal_shape<2>(shape)};

                raster.band(1).read(kernel.data());

                return kernel;
            }


            template<Arithmetic Element, Rank rank>
            auto read_array(std::string const& pathname) -> PartitionedArray<Element, rank>
            {
                // NOTE: Determine the shape of the array to base partition shape on. In real-life, the
                //       shape will be passed in by the user.
                gdal::Raster raster = gdal::Raster{gdal::open_dataset(pathname, GDALAccess::GA_ReadOnly)};
                auto const partition_shape = raster.shape();

                return lue::from_gdal<Element>(pathname, from_gdal_shape<2>(partition_shape));
            }

        private:

            template<Rank rank>
            auto from_gdal_shape(gdal::Shape const& gdal_shape) -> Shape<Count, rank>
            {
                Shape<Count, rank> shape{};

                std::copy(gdal_shape.begin(), gdal_shape.end(), shape.begin());

                return shape;
            }
    };

}  // namespace lue::document
