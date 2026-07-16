#include "lue/framework/api/cxx/type_info.hpp"
#include <pybind11/native_enum.h>
#include <pybind11/pybind11.h>


namespace lue::api {

    void bind_type_info(pybind11::module& module)
    {
        pybind11::native_enum<DataModel>(module, "DataModel", "enum.Enum")
            .value("Array", DataModel::Array)
            .value("Scalar", DataModel::Scalar)
            .value("Literal", DataModel::Literal)
            .finalize();
    }

}  // namespace lue::api
