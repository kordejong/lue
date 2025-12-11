#pragma once
#include "lue/netcdf4/define.hpp"
#include "lue/netcdf4/export.hpp"


namespace lue::netcdf4 {

    /*!
        @brief      Class for defining hyperslabs of arrays

        In general, a hyperslab defines a subset of an array. A subset potentially includes all array
        elements.
    */
    class LUE_NETCDF4_EXPORT Hyperslab
    {

        public:

            Hyperslab(Starts&& starts, Counts&& counts, Strides&& strides);

            Hyperslab(Starts&& starts, Counts&& counts);

            auto starts() const -> Starts;

            auto counts() const -> Counts;

            auto strides() const -> Strides;

            auto nr_dimensions() const -> int;

            auto is_strided() const -> bool;

        private:

            Starts _starts;

            Counts _counts;

            Strides _strides;
    };

}  // namespace lue::netcdf4
