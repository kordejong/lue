#include "lue/framework/api/cxx/local_operation.hpp"
#include <pybind11/stl.h>


using namespace pybind11::literals;


namespace lue::api {

    void bind_local_operations(pybind11::module& module)
    {
        module.def("abs", abs);
        module.def("acos", acos);
        module.def("add", add);
        module.def("asin", asin);
        module.def("atan", atan);
        module.def("atan2", atan2);
        module.def("ceil", ceil);
        module.def("close_to", close_to);
        module.def("cos", cos);
        module.def("divide", divide);
        module.def("equal_to", equal_to);
        module.def("exp", exp);
        module.def("floor", floor);
        module.def("greater_than", greater_than);
        module.def("greater_than_equal_to", greater_than_equal_to);
        module.def("less_than", less_than);
        module.def("less_than_equal_to", less_than_equal_to);
        module.def("log", log);
        module.def("log10", log10);
        module.def("logical_and", logical_and);
        module.def("logical_exclusive_or", logical_exclusive_or);
        module.def("logical_inclusive_or", logical_inclusive_or);
        module.def("logical_not", logical_not);
        module.def("valid", valid);
        module.def("modulus", modulus);
        module.def("multiply", multiply);
        module.def("negate", negate);
        module.def("not_equal_to", not_equal_to);
        module.def(
            "open_simplex_noise",
            open_simplex_noise,
            "x_coordinates"_a,
            "y_coordinates"_a,
            pybind11::kw_only(),
            "seed"_a);
        module.def("pow", pow);
        module.def("round", round);
        module.def("sin", sin);
        module.def("sqrt", sqrt);
        module.def("subtract", subtract);
        module.def("tan", tan);
        module.def("trunc", trunc);
        module.def(
            "where",
            [](Field const& condition_field, Field const& true_field) -> Field
            { return where(condition_field, true_field); });
        module.def(
            "where",
            [](Field const& condition_field, Field const& true_field, Field const& false_field) -> Field
            { return where(condition_field, true_field, false_field); });
        module.def(
            "uniform",
            [](Shape<Count, 2> const& array_shape,
               Shape<Count, 2> const& partition_shape,
               Field const& min_value,
               Field const& max_value) -> Field
            { return uniform(array_shape, partition_shape, min_value, max_value); },
            "array_shape"_a,
            "partition_shape"_a,
            pybind11::kw_only(),
            "min_value"_a,
            "max_value"_a);
        module.def(
            "uniform",
            [](Shape<Count, 2> const& array_shape, Field const& min_value, Field const& max_value) -> Field
            { return uniform(array_shape, min_value, max_value); },
            "array_shape"_a,
            pybind11::kw_only(),
            "min_value"_a,
            "max_value"_a);
        module.def(
            "uniform",
            [](Field const& other, Field const& min_value, Field const& max_value) -> Field
            { return uniform(other, min_value, max_value); },
            "other"_a,
            pybind11::kw_only(),
            "min_value"_a,
            "max_value"_a);
        module.def(
            "uniform",
            [](Field const& min_value, Field const& max_value) -> Field
            { return uniform(min_value, max_value); },
            pybind11::kw_only(),
            "min_value"_a,
            "max_value"_a);
    }

}  // namespace lue::api
