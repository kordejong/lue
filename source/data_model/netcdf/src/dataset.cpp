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
        int dataset_id{};

        if (int status = nc_create(name.c_str(), create_mode, &dataset_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot create dataset {}: {}", name, error_message(status)));
        }

        return dataset_id;
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
        int dataset_id{};

        if (int status = nc_open(name.c_str(), open_mode, &dataset_id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot open dataset {}: {}", name, error_message(status)));
        }

        return dataset_id;
    }


    Dataset::Dataset(int const dataset_id):

        Group{dataset_id}

    {
    }


    Dataset::~Dataset() noexcept
    {
        if (int status = nc_close(Group::id()); status != NC_NOERR)
        {
            // TODO Can't throw here. Log an error? Abort? Forget about it?
        }
    }

}  // namespace lue::netcdf
