#pragma once
#include <chrono>
#include <string>


namespace lue::utility {

    class Stopwatch
    {

        public:

            using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

            Stopwatch() = default;

            Stopwatch(Stopwatch const&) = default;

            Stopwatch(Stopwatch&&) = default;

            ~Stopwatch() = default;

            auto operator=(Stopwatch const&) -> Stopwatch& = default;

            auto operator=(Stopwatch&&) -> Stopwatch& = default;

            void start();

            void stop();

            auto start() const -> TimePoint const&;

            auto end() const -> TimePoint const&;

            auto elapsed_seconds() const -> double;

        private:

            TimePoint _start;

            TimePoint _end;
    };


    auto to_string(Stopwatch::TimePoint const& time_point) -> std::string;

}  // namespace lue::utility
