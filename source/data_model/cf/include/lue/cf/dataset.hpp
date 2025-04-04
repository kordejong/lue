#pragma once
#include "lue/cf/export.hpp"
#include "lue/netcdf4/dataset.hpp"


namespace lue::cf {

    class LUE_CF_EXPORT Dataset: public netcdf::Dataset
    {

        public:

            static auto create(std::string const& name, int create_mode) -> Dataset;

            static auto open(std::string const& name, int open_mode = NC_NOWRITE) -> Dataset;

            Dataset(netcdf::Dataset&& dataset);

            [[nodiscard]] auto version() const -> double;

            [[nodiscard]] auto title() const -> std::string;

            [[nodiscard]] auto history() const -> std::string;

            [[nodiscard]] auto institution() const -> std::string;

            [[nodiscard]] auto source() const -> std::string;

            [[nodiscard]] auto comment() const -> std::string;

            [[nodiscard]] auto references() const -> std::string;
    };

}  // namespace lue::cf
