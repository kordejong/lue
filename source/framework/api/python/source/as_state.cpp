#include "lue/framework/api/cxx/as_state.hpp"
#include <pybind11/pybind11.h>


namespace lue::api {

    void bind_as_state(pybind11::module& module)
    {
        module.def("as_state", as_state);
    }

}  // namespace lue::api
