#include "lue/framework/api/c/io.h"
#include "field.hpp"
#include "lue/framework/api/cxx/io.hpp"


auto lue_from_gdal(char const* name) -> LUE_Field*
{
    lue::api::Field result = lue::api::from_gdal(name);

    return new LUE_Field{.instance = new lue::api::Field{std::move(result)}};
}


void lue_to_gdal(LUE_Field* field, char const* name, char const* clone_name)
{
    hpx::future<void> result = clone_name == nullptr
                                   ? lue::api::to_gdal(lue_as_cxx_field(field), name)
                                   : lue::api::to_gdal(lue_as_cxx_field(field), name, clone_name);
    ;

    // TODO What to return? The caller may need the future<void>...
    /* return new Future{.instance = new hpx::future<void>{std::move(result)}}; */
}
