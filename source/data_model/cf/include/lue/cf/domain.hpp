#pragma once
#include "lue/cf/netcdf/auxiliary_coordinate_variable.hpp"
#include "lue/cf/netcdf/coordinate_variable.hpp"
#include "lue/cf/netcdf/dimension.hpp"
#include "lue/cf/netcdf/scalar_coordinate_variable.hpp"
#include <optional>
#include <variant>
#include <vector>


namespace lue::cf {

    /*!
        @brief      Metadata about measurement location and cell properties for the data
    */
    class Domain
    {

        public:

            /*!
                @brief      Number of cells lying along an independent axis of the domain

                Either defined by a scalar coordinate variable, which implies a domain axis of size one, or
                a dimension.
            */
            class DomainAxis
            {

                public:

                private:

                    std::optional<std::variant<netcdf::ScalarCoordinateVariable, netcdf::Dimension>>
                        _nr_cells;
            };


            class Coordinate
            {

                private:

                    class Data
                    {
                    };

                public:

                    std::optional<Data> _data;
            };


            /*!
                @brief      Numeric coordinates for a single domain axis that are non-missing and strictly
                            monotonically increasing or decreasing
            */
            class DimensionCoordinate: public Coordinate
            {

                public:

                private:

                    // ScalarCoordinateVariable has to be numeric
                    std::variant<netcdf::CoordinateVariable, netcdf::ScalarCoordinateVariable> _coordinates;
            };


            class AuxiliaryCoordinate: public Coordinate
            {

                public:

                private:

                    // ScalarCoordinateVariable has to be non-numeric
                    // std::variant<netcdf::AuxiliaryCoordinateVariable, netcdf::ScalarCoordinateVariable>
                    //     _coordinates;
                    netcdf::AuxiliaryCoordinateVariable _coordinates;
            };


            class CoordinateReference
            {

                public:

                private:
            };

            class DomainAncillary
            {

                public:

                private:
            };

            class CellMeasure
            {

                public:

                private:
            };

            class DomainTopology
            {

                public:

                private:
            };

            class CellConnectivity
            {

                public:

                private:
            };


        private:

            std::vector<DomainAxis> _domain_axes;

            std::vector<DimensionCoordinate> _dimension_coordinates;

            std::vector<AuxiliaryCoordinate> _auxiliary_coordinates;

            std::vector<CoordinateReference> _coordinate_reference_systems;

            std::vector<DomainAncillary> _domain_ancillaries;

            std::vector<CellMeasure> _cell_measures;

            std::vector<DomainTopology> _domain_topologies;

            std::vector<CellConnectivity> _cell_connectivities;
    };

}  // namespace lue::cf
