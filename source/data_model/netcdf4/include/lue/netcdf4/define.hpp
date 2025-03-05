#pragma once
#include <vector>


namespace lue::netcdf4 {

    using Index = std::size_t;
    using Indices = std::vector<Index>;

    using Start = std::size_t;
    using Starts = std::vector<Start>;

    using Count = std::size_t;
    using Counts = std::vector<Count>;

    using Stride = std::ptrdiff_t;
    using Strides = std::vector<Stride>;

}  // namespace lue::netcdf4
