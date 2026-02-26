#include "lue/utility/stopwatch.hpp"
#include <cassert>
#include <ctime>


namespace lue::utility {

    void Stopwatch::start()
    {
        _start = std::chrono::system_clock::now();
        _end = _start;

        assert(_start == _end);
    }


    void Stopwatch::stop()
    {
        _end = std::chrono::system_clock::now();

        assert(_start <= _end);
    }


    auto Stopwatch::start() const -> Stopwatch::TimePoint const&
    {
        return _start;
    }


    auto Stopwatch::end() const -> Stopwatch::TimePoint const&
    {
        return _end;
    }


    auto Stopwatch::elapsed_seconds() const -> double
    {
        std::chrono::duration<double> elapsed_seconds = _end - _start;

        return elapsed_seconds.count();
    }


    auto to_string(Stopwatch::TimePoint const& time_point) -> std::string
    {
        std::time_t const time = std::chrono::system_clock::to_time_t(time_point);
        std::string string = std::ctime(&time);

        // Remove trailing '\n', which ctime adds to the end...
        if (!string.empty())
        {
            assert(string.back() == '\n');
            string.pop_back();
        }

        return string;
    }

}  // namespace lue::utility
