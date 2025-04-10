#pragma once
#include "lue/cf/export.hpp"
#include "lue/cf/netcdf/auxiliary_coordinate_variable.hpp"
#include "lue/cf/netcdf/coordinate_variable.hpp"
#include "lue/cf/netcdf/dimension.hpp"
#include "lue/cf/netcdf/domain_variable.hpp"
#include "lue/cf/netcdf/location_index_set_variable.hpp"
#include "lue/cf/netcdf/mesh_topology_variable.hpp"
#include "lue/cf/netcdf/scalar_coordinate_variable.hpp"
#include <optional>
#include <variant>
#include <vector>


namespace lue::cf {

    /*!
        @brief      Discrete locations in multi-dimensional space

        Metadata about measurement location and cell properties for the data

        - Construct
        - Corresponds to a detCDF domain variable
        - Describes a domain comprising measurement locations and cell properties
        - Domain information is stored either implicitly or explicitly:
            - Implicit: via data variable attributes (such as `coordinates`)
            - Explicit (domain exists without reference to a data array):
                - Domain variable
                - Mesh topology variable
                - Location index set variable

    */
    class LUE_CF_EXPORT Domain /* : public netcdf4::DomainVariable */
    {

        public:

            /*!
                @brief      Independent axes of the domain

                Number of cells lying along an independent axis of the domain

                Specifies the number of cells lying along an independent axis of the domain.

                Either defined by a scalar coordinate variable, which implies a domain axis of size one, or
                a dimension.

                DimensionCoordinate and AuxiliaryCoordinate are related to Axis via the (implicit) domain
                mapping. TODO What does this mean?

                - Construct
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

                - Construct
                - Provides information which locate the cells of the domain and which depend on a subset
                  of the domain axis constructs
            */
            class Coordinate
            {

                private:

                    class Data  // DataArray
                    {
                    };

                public:

                    //! Coordinate values spanning the subset of the domain axis constructs
                    std::optional<Data> _values;

                    // TODO Same type as Field::Properties
                    using Properties = int;

                    //! Properties to describe the coordinates
                    Properties _properties;

                    //! Cell bounds recording the extents of each cell
                    std::optional<Data> _cell_bounds;

                    // TODO Extra arrays needed to interpret cell bounds values
            };


            /*!
                @brief      Cell locations

                Numeric coordinates for a single domain axis that are non-missing and strictly monotonically
                increasing or decreasing

                - Construct
                - Data array of the coordinate values is required
            */
            class DimensionCoordinate: public Coordinate
            {

                public:

                    // TODO Constructor must provide information about the required coordinate values?

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

                - Construct
                - Use this class when:
                    - A single domain axis requires more than one set of coordinate values
                    - Coordinate values are not numeric, strictly monotonic, or contain missing values
                    - When "they" vary along more than one domain axis construct simulateously
            */
            class AuxiliaryCoordinate: public Coordinate
            {

                public:

                private:

                    // Only non-numeric scalar coordinate variable
                    std::variant<netcdf4::AuxiliaryCoordinateVariable, netcdf4::ScalarCoordinateVariable>
                        _coordinates;
            };


            /*!
                @brief      Domain coordinate systems
                @tparam     .
                @param      .
                @return     .
                @exception  .

                - Construct
            */
            class CoordinateReference
            {

                public:

                private:

                    // TODO Associated with zero or more Coordinate instances

                    // TODO Associated with zero or more Ancillary instances
            };


            /*!
                @brief      Cell locations in alternative coordinate systems
                @tparam     .
                @param      .
                @return     .
                @exception  .

                - Construct
            */
            class Ancillary
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

                - Construct
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

                - Construct
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

                - Construct
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

            //! Explicitly stored domain information
            std::optional<std::variant<
                netcdf4::DomainVariable,
                netcdf4::MeshTopologyVariable,
                netcdf4::LocationIndexSetVariable>>
                _domain_variable;

            //! Zero or more
            Axes _axes;

            //! Zero or more
            Coordinates _coordinates;

            // std::vector<netcdf4::GenericCoordinateVariable> _coordinates;

            // std::vector<DimensionCoordinate> _dimension_coordinates;

            // std::vector<AuxiliaryCoordinate> _auxiliary_coordinates;

            std::vector<CoordinateReference> _coordinate_reference_systems;

            std::vector<Ancillary> _ancillaries;

            std::vector<CellMeasure> _cell_measures;

            std::vector<Topology> _domain_topologies;

            std::vector<CellConnectivity> _cell_connectivities;
    };

}  // namespace lue::cf
