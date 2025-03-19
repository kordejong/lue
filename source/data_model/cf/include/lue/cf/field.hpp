#pragma once
#include "lue/cf/domain.hpp"
#include "lue/cf/export.hpp"
#include "lue/cf/netcdf/data_variable.hpp"
#include <optional>


namespace lue::cf {

    /*!
        @brief      Scientific data discretized within a domain
        @tparam     .
        @param      .
        @return     .
        @exception  .
    */
    class LUE_CF_EXPORT Field: public netcdf4::DataVariable
    {

            /*!
                @brief      Data array
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class Data
            {

                public:

                private:
            };


            /*!
                @brief      Ancillary metadata which varies within the domain

                Metadata defined over the domain.
            */
            class FieldAncillary
            {

                public:

                private:
            };


            /*!
                @brief      Describes how data represents variation with cells

                Metadata describing how the cell values represent the variation of the physical quantity
                within the cells of the domain
            */
            class CellMethod
            {

                public:

                private:

                    Domain::Axes _domain_axes;
            };


            /*!
                @brief      Aspects of the data that are independent of the domain

                - Some attributes of variables: units, long_name, standard_name, ...
                - Some group attributes, including global attributes or the root group: history, institution,
                    ...
            */
            class Properties
            {

                public:

                private:
            };

        public:

            Field(int group_id, int variable_id);

            [[nodiscard]] auto domain() const -> std::optional<Domain> const&;

        private:

            //! Mandatory
            Data _data;

            //! Zero or one
            std::optional<Domain> _domain;

            //! Zero or more
            std::vector<FieldAncillary> _field_ancillaries;

            //! Zero or more
            std::vector<CellMethod> _cell_methods;

            std::optional<Properties> _properties;
    };

}  // namespace lue::cf
