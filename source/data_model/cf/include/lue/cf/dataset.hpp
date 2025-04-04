#pragma once
#include "lue/cf/export.hpp"
#include "lue/netcdf4/dataset.hpp"


namespace lue::cf {

    /*!
        @brief      .
        @tparam     .
        @param      .
        @return     .
        @exception  .

        These attributes, if present, may only be used in the root group and shall not be duplicated or
        overridden in child groups:

        - `Conventions`
        - `external_variables`

        These attributes, if present, are global variables:

        - `title`
        - `history`

        They are optional for non-root groups. They are allowed in order to provide additional provenance
        and description of subsidiary data. They do not override attributes from parent groups.

        These attributes, if present, can be overridden by an attribute in a data variable:

        - `institution`
        - `source`
        - `comment`
        - `references`
    */
    class LUE_CF_EXPORT Dataset: public netcdf4::Dataset
    {

        public:

            static auto create(std::string const& name, int create_mode) -> Dataset;

            static auto open(std::string const& name, int open_mode = NC_NOWRITE) -> Dataset;

            Dataset(netcdf4::Dataset&& dataset);

            [[nodiscard]] auto version() const -> double;
    };

}  // namespace lue::cf
