#include "lue/framework/api/cxx.hpp"
#include <hpx/hpx_main.hpp>


auto main() -> int
{
    using namespace lue::api;

    Field field1 = from_gdal("raster1.tif");
    Field field2 = from_gdal("raster2.tif");

    Field result = add(field1, field2);
}
