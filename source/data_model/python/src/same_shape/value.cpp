#include "submodule.hpp"
#include "lue/array/same_shape/value.hpp"
#include <pybind11/pybind11.h>


namespace py = pybind11;
using namespace pybind11::literals;


namespace lue {
namespace same_shape {

void init_value(
    py::module& module)
{

    py::class_<Value, Array>(
        module,
        "Value",
        R"(
    Value docstring...
)")

        .def("__len__",
            &Value::nr_arrays)

        .def("expand",
            [](
                Value& value,
                Count const nr_arrays) -> Value&
            {
                value.expand(nr_arrays);
                return value;
            })

        ;

}

}  // namespace same_shape
}  // namespace lue