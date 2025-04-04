#pragma once
#include "lue/cf/domain.hpp"
#include <optional>


namespace lue::cf {

    class Field
    {

            class Data
            {

                public:

                private:
            };


            /*!
                @brief      Metadata defined over the domain
            */
            class FieldAncillary
            {

                public:

                private:
            };


            /*!
                @brief      Metadata describing how the cell values represent the variation of the physical
                            quantity within the cells of the domain
            */
            class CellMethod
            {

                public:

                private:

                    std::vector<Domain::DomainAxis> _domain_axes;
            };


            /*!
                @brief      Aspects of the data that are independent of the domain
            */
            class Properties
            {

                public:

                private:
            };

        public:

        private:

            Data _data;

            std::optional<Domain> _domain;

            std::vector<FieldAncillary> _field_ancillaries;

            std::vector<CellMethod> _cell_methods;

            std::optional<Properties> _properties;
    };

}  // namespace lue::cf
