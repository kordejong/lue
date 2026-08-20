#include "lue/document/c/io.h"
#include "lue/document/cxx/io.hpp"


auto lue_read_kernel(char const* pathname) -> LUE_Kernel*
{
    lue::api::Kernel result = lue::document::read_kernel(pathname);

    return new LUE_Kernel{.instance = new lue::api::Kernel{std::move(result)}};
}
