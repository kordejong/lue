#include <pybind11/pybind11.h>


namespace lue::framework {

    void bind_focal_sum(pybind11::module& module);


    void bind_focal_operations(
        pybind11::module& module)
    {
        bind_focal_sum(module);
    }

}  // namespace lue::framework