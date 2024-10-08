#include "lue/py/framework/submodule.hpp"
#include "hpx_runtime.hpp"
#include "lue/gdal.hpp"
#include <pybind11/stl.h>


namespace lue::framework {
    namespace {

        HPXRuntime* runtime{nullptr};


        void start_hpx_runtime(std::vector<std::string> const& configuration)
        {
            // Iff the pointer to the runtime is not pointing to an instance,
            // instantiate one. This will start the HPX runtime.
            if (runtime == nullptr)
            {
                pybind11::gil_scoped_release release;
                runtime = new HPXRuntime{configuration};
            }
        }


        void stop_hpx_runtime()
        {
            // Iff the pointer to the runtime is pointing to an instance, delete
            // it. This will stop the HPX runtime.
            if (runtime != nullptr)
            {
                HPXRuntime* r = runtime;
                runtime = nullptr;
                pybind11::gil_scoped_release release;
                delete r;
            }
        }


        bool on_root_locality()
        {
            return hpx::find_here() == hpx::find_root_locality();
        }

    }  // Anonymous namespace


    void bind_hpx(pybind11::module& module);
    void bind_create_array(pybind11::module& module);
    void bind_wait_partitioned_array(pybind11::module& module);
    void bind_focal_operations(pybind11::module& module);
    void bind_global_operations(pybind11::module& module);
    void bind_local_operations(pybind11::module& module);
    void bind_routing_operations(pybind11::module& module);
    void bind_zonal_operations(pybind11::module& module);
    void bind_partitioned_array(pybind11::module& module);
    void bind_scalar(pybind11::module& module);
    void bind_serial_route(pybind11::module& module);
    void bind_wait_serial_route(pybind11::module& module);

    void bind_array_partition_id(pybind11::module& module);
    void bind_locality_id(pybind11::module& module);
    void bind_normal(pybind11::module& module);
    void bind_timestamp(pybind11::module& module);
    void bind_uniform(pybind11::module& module);

    void bind_read_array(pybind11::module& module);
    void bind_write_array(pybind11::module& module);

    void bind_model(pybind11::module& module);
    void bind_progressor(pybind11::module& module);
    void bind_simulate(pybind11::module& module);

    void bind_from_numpy(pybind11::module& module);
    void bind_to_numpy(pybind11::module& module);

    void bind_from_gdal(pybind11::module& module);
    void bind_to_gdal(pybind11::module& module);

    void init_submodule(pybind11::module& module)
    {
        pybind11::module submodule = module.def_submodule(
            "framework",
            R"(
    :mod:`lue.framework` --- LUE Environmental Modelling Framework
    ==============================================================

    The :mod:`lue.framework` package ...
)");

        gdal::register_gdal_drivers();

        submodule.def("start_hpx_runtime", &start_hpx_runtime);

        submodule.def("stop_hpx_runtime", &stop_hpx_runtime);

        submodule.def("on_root_locality", &on_root_locality);

        bind_hpx(submodule);

        // Wrap high-level data structures
        bind_scalar(submodule);
        bind_partitioned_array(submodule);
        bind_serial_route(submodule);
        bind_wait_serial_route(submodule);

        // Wrap high-level algorithms
        bind_create_array(submodule);
        bind_wait_partitioned_array(submodule);
        bind_focal_operations(submodule);
        bind_global_operations(submodule);
        bind_local_operations(submodule);
        bind_routing_operations(submodule);
        bind_zonal_operations(submodule);
        bind_read_array(submodule);
        bind_write_array(submodule);

        bind_array_partition_id(submodule);
        bind_locality_id(submodule);
        bind_normal(submodule);
        bind_timestamp(submodule);
        bind_uniform(submodule);

        bind_model(submodule);
        bind_progressor(submodule);
        bind_simulate(submodule);

        bind_from_numpy(submodule);
        bind_to_numpy(submodule);
        bind_from_gdal(submodule);
        bind_to_gdal(submodule);

        // Unless the user calls stop_hpx_runtime explicitly, we will do it
        // automatically upon module unload
        auto atexit = pybind11::module_::import("atexit");
        atexit.attr("register")(pybind11::cpp_function([]() { stop_hpx_runtime(); }));
    }

}  // namespace lue::framework
