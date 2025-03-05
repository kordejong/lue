#include "lue/netcdf4/hyperslab.hpp"
#include <algorithm>
#include <cassert>


namespace lue::netcdf4 {

    /*!
        @brief      Construct an instance given @a starts, @a counts, and @a strides

        Strides must be larger than zero. If the strides for all dimensions are one then the hyperslab is
        non-strided.
    */
    Hyperslab::Hyperslab(Starts&& starts, Counts&& counts, Strides&& strides):

        _starts{std::move(starts)},
        _counts{std::move(counts)},
        _strides{std::move(strides)}

    {
        assert(_counts.size() == _starts.size());
        assert(_strides.size() == _starts.size());

        // A stride of zero makes no sense
        assert(std::all_of(_strides.begin(), _strides.end(), [](auto const stride) { return stride > 0; }));
    }


    /*!
        @brief      Construct a non-strided instance
    */
    Hyperslab::Hyperslab(Starts&& starts, Counts&& counts):

        Hyperslab{std::move(starts), std::move(counts), Strides(starts.size(), 1)}

    {
    }


    /*!
        @brief      Return the starts
    */
    auto Hyperslab::starts() const -> Starts
    {
        return _starts;
    }


    /*!
        @brief      Return the counts
    */
    auto Hyperslab::counts() const -> Counts
    {
        return _counts;
    }


    /*!
        @brief      Return the strides
    */
    auto Hyperslab::strides() const -> Strides
    {
        return _strides;
    }


    /*!
        @brief      Return the number of dimensions
    */
    auto Hyperslab::nr_dimensions() const -> int
    {
        return static_cast<int>(_starts.size());
    }


    /*!
        @brief      Return whether the hyperslab defines a strided subset, where elements are skipped
    */
    auto Hyperslab::is_strided() const -> bool
    {
        return std::all_of(_strides.begin(), _strides.end(), [](auto const lhs) { return lhs == 1; });
    }

}  // namespace lue::netcdf4
