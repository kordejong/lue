#include "lue/validate.hpp"
#include <pybind11/stl_bind.h>


namespace py = pybind11;


namespace lue {

void init_validate(
    py::module& module)
{

    py::class_<Issue>(
        module,
        "Issue",
        R"(
    Issue docstring...
)")

        .def_property_readonly(
            "message",
            &Issue::message
        )

        ;

    py::bind_vector<std::vector<Issue>>(
        module,
        "VectorIssue")

        ;


    py::class_<Issues>(
        module,
        "Issues",
        R"(
    Issues docstring...
)")

        .def_property_readonly(
            "errors_found",
            &Issues::errors_found
        )

        .def_property_readonly(
            "warnings_found",
            &Issues::warnings_found
        )

        .def_property_readonly(
            "errors",
            &Issues::errors
        )

        .def_property_readonly(
            "warnings",
            &Issues::warnings
        )

        ;

    module.def(
        "validate",
        [](std::string const& name) {
            Issues issues;
            validate(name, issues);
            return issues;
        },
        R"(
    Validate docstring...
)")
        ;

}

}  // namespace lue
