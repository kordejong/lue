#include "lue/netcdf/dataset.hpp"
#include "lue/netcdf/error.hpp"
#include <format>


namespace lue::netcdf {

    /*!
        @brief      Create dataset
        @param      name Name of dataset
        @param      create_mode NC_CLOBBER, NC_NOCLOBBER, NC_64BIT_OFFSET, NC_64BIT_DATA, NC_NETCDF4,
                    NC_DISKLESS, NC_PERSIST
        @return     Created dataset
        @exception  std::runtime_error In case the dataset cannot be created
    */
    auto Dataset::create(std::string const& name, int create_mode) -> Dataset
    {
        // TODO Force use of NC_NETCDF4?
        int id{};

        if (int status = nc_create(name.c_str(), create_mode, &id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot create {}: {}", name, error_message(status)));
        }

        return id;
    }


    /*!
        @brief      Open dataset
        @param      open_mode NC_NOWRITE, NC_WRITE, NC_SHARE, NC_WRITE | NC_SHARE
        @param      name Name of dataset
        @return     Open dataset
        @exception  std::runtime_error In case the dataset cannot be opened
    */
    auto Dataset::open(std::string const& name, int open_mode) -> Dataset
    {
        int id{};

        if (int status = nc_open(name.c_str(), open_mode, &id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot open {}: {}", name, error_message(status)));
        }

        return id;
    }


    Dataset::Dataset(int id):

        _id{id}

    {
        // TODO Verify id is a valid ID of an open dataset. Don't create instances for invalid IDs.
    }


    Dataset::~Dataset() noexcept
    {
        if (int status = nc_close(_id); status != NC_NOERR)
        {
            // TODO Can't throw here. Log an error?
        }
    }

}  // namespace lue::netcdf
