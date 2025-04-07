#pragma once
#include "lue/cf/domain.hpp"
#include "lue/cf/export.hpp"
#include "lue/cf/netcdf/data_variable.hpp"
#include <memory>
// #include <optional>


namespace lue::cf {

    /*!
        @brief      Scientific data discretized within a domain
        @tparam     .
        @param      .
        @return     .
        @exception  .

        - Construct
        - Corresponds to a netCDF data variable with all of its metadata
        - Data array is the only mandatory component of the Field construct
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

                - Construct
            */
            class Ancillary
            {

                public:

                private:
            };


            /*!
                @brief      Describes how data represents variation with cells

                Metadata describing how the cell values represent the variation of the physical quantity
                within the cells of the domain

                - Construct
            */
            class CellMethod
            {

                public:

                private:

                    // TODO These are part of the Domain as well. Maybe just refer to (a subset of?) these?
                    // Is each CellMethod maybe associated with a single DomainAxis instance?
                    Domain::Axes _domain_axes;
            };


            /*!
                @brief      Aspect of the data that is independent of the domain
                @tparam     .
                @param      .
                @return     .
                @exception  .

                - Some attributes of variables: units, long_name, standard_name, ...
                - Some group attributes, including global attributes of the root group: history,
                institution,
            */
            class Property
            {

                public:

                private:
            };


            // /*!
            //     @brief      Aspects of the data that are independent of the domain

            //     - Some attributes of variables: units, long_name, standard_name, ...
            //     - Some group attributes, including global attributes of the root group: history,
            //     institution,
            //         ...
            // */
            // class Properties
            // {

            //     public:

            //     private:
            // };

        public:

            using FieldAncillaries = std::vector<Ancillary>;

            using CellMethods = std::vector<CellMethod>;

            using Properties = std::vector<Property>;

            Field(int group_id, int variable_id);

            [[nodiscard]] auto has_domain() const -> bool;

            [[nodiscard]] auto domain() const -> Domain const&;

            [[nodiscard]] auto field_ancillaries() const -> FieldAncillaries const&;

            [[nodiscard]] auto cell_methods() const -> CellMethods const&;

            [[nodiscard]] auto properties() const -> Properties const&;

        private:

            //! Mandatory
            Data _data;

            //! Pointer to shared instance of zero or one domains
            std::shared_ptr<Domain> _domain;

            //! Zero or more
            FieldAncillaries _field_ancillaries;

            //! Zero or more
            CellMethods _cell_methods;

            Properties _properties;

            // std::optional<Properties> _properties;
    };

}  // namespace lue::cf
