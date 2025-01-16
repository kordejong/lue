#pragma once
#include "lue/netcdf4/define.hpp"


namespace lue::netcdf {

    /*!
        @brief      Class for defining hyperslabs of arrays

        In general, a hyperslab defines a subset of an array. A subset potentially includes all array
        elements.
    */
    class Hyperslab
    {

        public:

            Hyperslab(Starts&& starts, Counts&& counts, Strides&& strides);

            Hyperslab(Starts&& starts, Counts&& counts);

            [[nodiscard]] auto starts() const -> Starts;

            [[nodiscard]] auto counts() const -> Counts;

            [[nodiscard]] auto strides() const -> Strides;

            [[nodiscard]] auto nr_dimensions() const -> int;

            [[nodiscard]] auto is_strided() const -> bool;

        private:

            Starts _starts;

            Counts _counts;

            Strides _strides;
    };

}  // namespace lue::netcdf
