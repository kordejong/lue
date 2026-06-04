#include "lue/framework/api/cxx/miscellaneous/create_array.hpp"
#include "lue/framework/api/cxx/operator.hpp"
#include <pybind11/stl.h>


using namespace pybind11::literals;


namespace lue::api {

    void bind_array(pybind11::module& module)
    {
        pybind11::class_<Array>(module, "Array")

            // bool(a), not a, if a, while a, ...
            .def(
                "__bool__",
                []([[maybe_unused]] Array const& Array) -> void
                {
                    // ValueError
                    throw std::invalid_argument("The truth value of an Array is ambiguous");
                })

            // a + b, a += b
            // .def(pybind11::self + pybind11::self)
            .def("__add__", add, pybind11::is_operator())
            .def("__radd__", add, pybind11::is_operator())


            // TODO
            // https://pybind11.readthedocs.io/en/stable/advanced/classes.html#operator-overloading

            ;

        module.def(
            "create_array",
            [](Shape<Count, 2> const& array_shape,
               Scalar const& fill_value,
               std::optional<Shape<Count, 2>> const& partition_shape) -> Array
            {
                return partition_shape ? create_array(array_shape, *partition_shape, fill_value)
                                       : create_array(array_shape, fill_value);
            },
            "array_shape"_a,
            "fill_value"_a,
            pybind11::kw_only(),
            "partition_shape"_a = std::optional<Shape<Count, 2>>{},
            pybind11::return_value_policy::move);
    }

}  // namespace lue::api
