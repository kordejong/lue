#pragma once
#include "lue/netcdf/group.hpp"
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    /*!
        @brief      .
        @param      .
        @return     .
        @exception  .

        A Dataset is a Group: the root group. A dataset instance can be used anywhere a Group instance can be
        used.
    */
    class Dataset: public Group
    {

        public:

            static auto create(std::string const& name, int create_mode) -> Dataset;

            static auto open(std::string const& name, int open_mode = NC_NOWRITE) -> Dataset;

            Dataset(int dataset_id);

            Dataset(Dataset const& other) = delete;

            Dataset(Dataset&& other) noexcept;

            ~Dataset() noexcept;

            auto operator=(Dataset const& other) -> Dataset& = delete;

            auto operator=(Dataset&& other) noexcept -> Dataset&;

        private:
    };

}  // namespace lue::netcdf
