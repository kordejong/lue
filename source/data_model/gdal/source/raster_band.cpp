#include "lue/gdal/raster_band.hpp"
#include <stdexcept>


namespace lue::gdal {

    /*!
        @brief      Return the datatype of the elements in the raster band
    */
    auto data_type(GDALRasterBand& band) -> GDALDataType
    {
        return band.GetRasterDataType();
    }


    auto block_shape(GDALRasterBand& band) -> Shape
    {
        int size_x{};
        int size_y{};

        band.GetBlockSize(&size_x, &size_y);

        return {size_y, size_x};
    }


    auto shape(GDALRasterBand& band) -> Shape
    {
        return {band.GetYSize(), band.GetXSize()};
    }


    auto read_block(GDALRasterBand& band, Offset const& block_offset, void* buffer) -> void
    {
        auto const [offset_y, offset_x] = block_offset;
        auto cpl_status = band.ReadBlock(offset_x, offset_y, buffer);

        if (cpl_status != CE_None)
        {
            throw std::runtime_error("Cannot read block from GDAL raster band");
        }
    }


    auto write_block(GDALRasterBand& band, Offset const& block_offset, void* buffer) -> void
    {
        auto const [offset_y, offset_x] = block_offset;
        auto cpl_status = band.WriteBlock(offset_x, offset_y, buffer);

        if (cpl_status != CE_None)
        {
            throw std::runtime_error("Cannot write block to GDAL raster band");
        }
    }


    auto read(
        GDALRasterBand& band,
        Offset const& offset,
        Shape const& raster_shape,
        GDALDataType const data_type,
        void* buffer) -> void
    {
        auto const [offset_y, offset_x] = offset;
        auto const [nr_rows, nr_cols] = raster_shape;

        auto cpl_status = band.RasterIO(
            GF_Read,
            offset_x,
            offset_y,
            nr_cols,
            nr_rows,
            buffer,
            nr_cols,
            nr_rows,
            data_type,
            0,
            0,
            nullptr);

        if (cpl_status != CE_None)
        {
            throw std::runtime_error("Cannot read from GDAL raster band");
        }
    }


    auto read(GDALRasterBand& band, Shape const& raster_shape, GDALDataType const data_type, void* buffer)
        -> void
    {
        read(band, {0, 0}, raster_shape, data_type, buffer);
    }


    auto write(
        GDALRasterBand& band,
        Offset const& offset,
        Shape const& raster_shape,
        GDALDataType const data_type,
        void* buffer) -> void
    {
        auto const [offset_y, offset_x] = offset;
        auto const [nr_rows, nr_cols] = raster_shape;

        auto cpl_status = band.RasterIO(
            GF_Write,
            offset_x,
            offset_y,
            nr_cols,
            nr_rows,
            buffer,
            nr_cols,
            nr_rows,
            data_type,
            0,
            0,
            nullptr);

        if (cpl_status != CE_None)
        {
            throw std::runtime_error("Cannot write to GDAL raster band");
        }
    }


    auto write(GDALRasterBand& band, Shape const& raster_shape, GDALDataType const data_type, void* buffer)
        -> void
    {
        write(band, {0, 0}, raster_shape, data_type, buffer);
    }


    void set_metadata(
        GDALRasterBand& band,
        std::string const& key,
        std::string const& value,
        [[maybe_unused]] std::string const& domain)
    {
        auto cpl_status = band.SetMetadataItem(key.c_str(), value.c_str(), domain.c_str());

        if (cpl_status != CE_None)
        {
            throw std::runtime_error("Cannot set metadata item to GDAL raster band");
        }
    }

    auto metadata(GDALRasterBand& band, std::string const& key, std::string const& domain) -> std::string
    {
        char const* item = band.GetMetadataItem(key.c_str(), domain.c_str());

        if (item == nullptr)
        {
            throw std::runtime_error("Cannot get metadata item from GDAL raster band");
        }

        return item;
    }

}  // namespace lue::gdal
