#pragma once
#include "lue/utility/application.hpp"


namespace lue::utility {

    class Describe: public Application

    {

        public:

            explicit Describe(std::vector<std::string> const& arguments);

            auto run_implementation() -> int final;

        private:
    };

}  // namespace lue::utility
