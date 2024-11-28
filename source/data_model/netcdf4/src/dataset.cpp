#include "lue/netcdf4/dataset.hpp"
#include "lue/netcdf4/error.hpp"
#include <fmt/format.h>
#include <cassert>


namespace lue::netcdf {

    /*!
        @brief      Create dataset
        @param      name Name of dataset
        @param      create_mode One of NC_CLOBBER, NC_NOCLOBBER, NC_DISKLESS, NC_PERSIST. To force the use
                    of netCDF4 format, create mode NC_NETCDF4 is added unconditionally, so the caller doesn't
                    have to.
        @return     Created dataset, in define mode
        @exception  std::runtime_error In case @a create_mode is not compatible with netCDF-4 format or in
                    case the dataset cannot be created
    */
    auto Dataset::create(std::string const& name, int create_mode) -> Dataset
    {
        create_mode |= NC_NETCDF4;

        // if(create_mode &
        //
        // NC_SHARE
        // NC_64BIT_OFFSET
        // NC_64BIT_DATA
        // NC_CLASSIC_MODEL
        //
        // TODO Throw exception, invalid format
        //
        // NC_MMAP
        //
        // TODO Throw exception, deprecated create_mode

        int dataset_id{};

        if (int status = nc_create(name.c_str(), create_mode, &dataset_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                fmt::format("Cannot create dataset {}: {}", name, error_message(status)));
        }

        return dataset_id;
    }


    /*!
        @brief      Open dataset
        @param      open_mode NC_NOWRITE, NC_WRITE, NC_SHARE, NC_WRITE | NC_SHARE
        @param      name Name of dataset
        @return     Open dataset, in define mode if @a open_mode contains NC_WRITE
        @exception  std::runtime_error In case the dataset cannot be opened
    */
    auto Dataset::open(std::string const& name, int open_mode) -> Dataset
    {
        int dataset_id{};

        if (int status = nc_open(name.c_str(), open_mode, &dataset_id); status != NC_NOERR)
        {
            throw std::runtime_error(fmt::format("Cannot open dataset {}: {}", name, error_message(status)));
        }

        return dataset_id;
    }


    Dataset::Dataset(int const dataset_id):

        Group{dataset_id}

    {
    }


    Dataset::Dataset(Dataset&& other) noexcept:

        Group{std::move(other)}

    {
        assert(id() < 0);
    }


    Dataset::~Dataset() noexcept
    {
        if (Group::id() >= 0)
        {
            if (int status = nc_close(Group::id()); status != NC_NOERR)
            {
                // TODO Can't throw here. Log an error? Abort? Forget about it?
            }
        }
    }


    // /*!
    //     @brief      Leave define mode
    //     @exception  std::runtime_error In case define mode cannot be left
    // */
    // void Dataset::end_define()
    // {
    //     if (int status = nc_enddef(Group::id()); status != NC_NOERR)
    //     {
    //         throw std::runtime_error(fmt::format("Cannot leave define mode: {}", error_message(status)));
    //     }
    // }

}  // namespace lue::netcdf
