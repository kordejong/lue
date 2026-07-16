#include "lue/framework/api/cxx/as_field.hpp"
#include <pybind11/pybind11.h>


namespace lue::api {

    void bind_as_field(pybind11::module& module)
    {
        module.def("as_field", [](Array& array) -> Field { return as_field(std::move(array)); });
        module.def("as_field", [](Literal& literal) -> Field { return as_field(std::move(literal)); });
        module.def("as_field", [](Scalar& scalar) -> Field { return as_field(std::move(scalar)); });
    }

}  // namespace lue::api
