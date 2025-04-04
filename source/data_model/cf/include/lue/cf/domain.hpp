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
        @brief      Discrete locations in multidimensional space

        Metadata about measurement location and cell properties for the data
    */
    class Domain
    {

        public:

            /*!
                @brief      Independent axes of the domain

                Number of cells lying along an independent axis of the domain

                Either defined by a scalar coordinate variable, which implies a domain axis of size one, or
                a dimension.
            */
            class DomainAxis
            {

                public:

                private:

                    std::optional<std::variant<netcdf4::ScalarCoordinateVariable, netcdf4::Dimension>>
                        _nr_cells;
            };


            /*!
                @brief      .
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
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
                @brief      Cell locations

                Numeric coordinates for a single domain axis that are non-missing and strictly monotonically
                increasing or decreasing
            */
            class DimensionCoordinate: public Coordinate
            {

                public:

                private:

                    // ScalarCoordinateVariable has to be numeric
                    std::variant<netcdf4::CoordinateVariable, netcdf4::ScalarCoordinateVariable> _coordinates;
            };


            /*!
                @brief      Cell locations
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class AuxiliaryCoordinate: public Coordinate
            {

                public:

                private:

                    // ScalarCoordinateVariable has to be non-numeric
                    // std::variant<netcdf4::AuxiliaryCoordinateVariable, netcdf4::ScalarCoordinateVariable>
                    //     _coordinates;
                    netcdf4::AuxiliaryCoordinateVariable _coordinates;
            };


            /*!
                @brief      Domain coordinate systems
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class CoordinateReference
            {

                public:

                private:
            };


            /*!
                @brief      Cell locations in alternative coordinate systems
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class DomainAncillary
            {

                public:

                private:
            };


            /*!
                @brief      Cell size or shape
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class CellMeasure
            {

                public:

                private:
            };


            /*!
                @brief      Geospatial topology of domain cells
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class DomainTopology
            {

                public:

                private:
            };


            /*!
                @brief      Connectivity of domain cells
                @tparam     .
                @param      .
                @return     .
                @exception  .
            */
            class CellConnectivity
            {

                public:

                private:
            };


        private:

            //! Zero or more
            std::vector<DomainAxis> _domain_axes;

            //! Zero or more
            std::vector<netcdf4::GenericCoordinateVariable> _coordinates;

            // std::vector<DimensionCoordinate> _dimension_coordinates;

            // std::vector<AuxiliaryCoordinate> _auxiliary_coordinates;

            std::vector<CoordinateReference> _coordinate_reference_systems;

            std::vector<DomainAncillary> _domain_ancillaries;

            std::vector<CellMeasure> _cell_measures;

            std::vector<DomainTopology> _domain_topologies;

            std::vector<CellConnectivity> _cell_connectivities;
    };

}  // namespace lue::cf
