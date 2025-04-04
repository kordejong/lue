#pragma once
#include "lue/netcdf4/group.hpp"
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    /*!
        @brief      Class for representing the root group

        A dataset instance can be used anywhere a Group instance can be used.
    */
    class LUE_NETCDF4_EXPORT Dataset: public Group
    {

        public:

            static auto create(std::string const& name, int create_mode) -> Dataset;

            static auto open(std::string const& name, int open_mode = NC_NOWRITE) -> Dataset;

            static auto format_as_string(int format) -> std::string;

            Dataset(int dataset_id);

            Dataset(Dataset const& other) = delete;

            Dataset(Dataset&& other) noexcept;

            ~Dataset() noexcept;

            auto operator=(Dataset const& other) -> Dataset& = delete;

            auto operator=(Dataset&& other) noexcept -> Dataset& = delete;  // TODO

            auto release() -> int;

            [[nodiscard]] auto path() const -> std::string;

            [[nodiscard]] auto format() const -> int;

        private:
    };

}  // namespace lue::netcdf
