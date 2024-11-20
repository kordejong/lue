#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Dataset
    {

        public:

            static auto create(std::string const& name, int create_mode) -> Dataset;

            static auto open(std::string const& name, int open_mode = NC_NOWRITE) -> Dataset;

            Dataset(int id);

            Dataset(Dataset const& other) = delete;

            Dataset(Dataset&& other) noexcept;

            ~Dataset() noexcept;

            auto operator=(Dataset const& other) -> Dataset& = delete;

            auto operator=(Dataset&& other) noexcept -> Dataset&;

        private:

            int _id;
    };

}  // namespace lue::netcdf
