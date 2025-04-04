#pragma once
#include "lue/cf/export.hpp"
#include "lue/cf/netcdf/auxiliary_coordinate_variable.hpp"
#include "lue/cf/netcdf/coordinate_variable.hpp"
#include "lue/cf/netcdf/dimension.hpp"
#include "lue/cf/netcdf/scalar_coordinate_variable.hpp"
#include <optional>
#include <variant>
#include <vector>


namespace lue::cf {

    /*!
        @brief      Discrete locations in multi-dimensional space

        Metadata about measurement location and cell properties for the data
    */
    class LUE_CF_EXPORT Domain
    {

        public:

            /*!
                @brief      Independent axes of the domain

                Number of cells lying along an independent axis of the domain

                Either defined by a scalar coordinate variable, which implies a domain axis of size one, or
                a dimension.

                DimensionCoordinate and AuxiliaryCoordinate are related to Axis via the (implicit) domain
                mapping. TODO What does this mean?
            */
            class Axis
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

                    // TODO Associated with zero or more Coordinate instances

                    // TODO Associated with zero or more DomainAncillary instances
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
            class Topology
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

            using Axes = std::vector<Axis>;

            using Coordinates = std::vector<Coordinate>;


            [[nodiscard]] auto axes() const& -> Axes;

        private:

            //! Zero or more
            Axes _axes;

            //! Zero or more
            Coordinates _coordinates;

            // std::vector<netcdf4::GenericCoordinateVariable> _coordinates;

            // std::vector<DimensionCoordinate> _dimension_coordinates;

            // std::vector<AuxiliaryCoordinate> _auxiliary_coordinates;

            std::vector<CoordinateReference> _coordinate_reference_systems;

            std::vector<DomainAncillary> _domain_ancillaries;

            std::vector<CellMeasure> _cell_measures;

            std::vector<Topology> _domain_topologies;

            std::vector<CellConnectivity> _cell_connectivities;
    };

}  // namespace lue::cf
