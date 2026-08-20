#pragma once
#include "lue/document/cxx/export.hpp"
#include "lue/utility/application.hpp"


namespace lue::document {

    class LUE_DOCUMENT_CXX_EXPORT Example: public utility::Application
    {

        public:

            Example(cxxopts::Options&& options, int argc, char const* const* argv);
    };

}  // namespace lue::document
