#include "lue/document/cxx/io.hpp"
#include "lue/gdal.hpp"


namespace lue::document {
    namespace {

        template<Rank rank>
        auto from_gdal_shape(gdal::Shape const& gdal_shape) -> Shape<Count, rank>
        {
            lue_hpx_assert(rank == 2);

            Shape<Count, rank> shape{};

            std::copy(gdal_shape.begin(), gdal_shape.end(), shape.begin());

            return shape;
        }

    }  // Anonymous namespace


    auto read_kernel(std::string const& pathname) -> api::Kernel
    {
        // NOTE: Read the kernel weights from a dataset. In real-life, these weights will likely be
        //       literal values stored in the code.

        gdal::Raster raster = gdal::Raster{gdal::open_dataset(pathname, GDALAccess::GA_ReadOnly)};
        auto const shape = raster.shape();
        Rank const rank{2};
        api::Kernel result{};

        switch (raster.data_type())
        {
            case GDT_Float32:
            {
                Kernel<float, 2> kernel{from_gdal_shape<rank>(shape)};
                raster.band(1).read(kernel.data());
                result = kernel;
                break;
            }
            case GDT_Float64:
            {
                Kernel<double, 2> kernel{from_gdal_shape<rank>(shape)};
                raster.band(1).read(kernel.data());
                result = kernel;
                break;
            }
            default:
            {
                // TODO: generalize
                lue_hpx_assert(false);
            }
        }

        return result;
    }


    void write_value_scale(std::string const& pathname, std::string const& value_scale)
    {
        gdal::Raster raster = gdal::Raster{gdal::open_dataset(pathname, GDALAccess::GA_Update)};

        raster.band(1).set_metadata("value_scale", value_scale, "lue");
    }

}  // namespace lue::document
