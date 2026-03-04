#pragma once
#include "lue/document/export.hpp"
#include "lue/utility/application.hpp"


namespace lue::document {

    class LUE_DOCUMENT_EXPORT Example: public utility::Application
    {

        public:

            Example(cxxopts::Options&& options, int argc, char const* const* argv);

        private:
    };

}  // namespace lue::document
