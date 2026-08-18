#include "lue/document/c/example.h"
#include "lue/gdal.hpp"


void lue_initialize_example(void)
{
    lue::gdal::register_gdal_drivers();
}
