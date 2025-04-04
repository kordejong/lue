#pragma once
#include "lue/utility/application.hpp"


namespace lue::utility {

    class Describe: public Application

    {

        public:

            Describe(int argc, char const* const* argv);

            // TODO explicit Describe(int argc, char const* argv[]);

            auto run_implementation() -> int final;

        private:
    };

}  // namespace lue::utility
