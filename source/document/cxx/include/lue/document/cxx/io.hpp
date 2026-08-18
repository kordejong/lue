#pragma once
#include "lue/framework/api/cxx/kernel.hpp"
#include "lue/document/cxx/export.hpp"
#include <string>


namespace lue::document {

    LUE_DOCUMENT_CXX_EXPORT auto read_kernel(std::string const& pathname) -> api::Kernel;

}  // namespace lue::document
