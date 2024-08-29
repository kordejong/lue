#include "lue/py/framework_x/submodule.hpp"
#include "stream.hpp"
#include "lue/framework/api/cxx/runtime.hpp"
#include "lue/assert.hpp"
#include <pybind11/stl.h>  // vector<string>
#include <memory>


namespace lue::api {

    void bind_array(pybind11::module& module);
    void bind_field(pybind11::module& module);
    void bind_io(pybind11::module& module);
    void bind_local_operations(pybind11::module& module);
    void bind_scalar(pybind11::module& module);


    namespace {

        class CommandLine
        {

            public:

                CommandLine()
                {
                    // The goal here is (only) to set _argc/_argv for the
                    // HPX runtime to use

                    pybind11::gil_scoped_acquire acquire;

                    pybind11::object sys{pybind11::module_::import("sys")};
                    pybind11::list argv_py{sys.attr("argv")};

                    _argc = static_cast<int>(argv_py.size());

                    _argument_strings.resize(_argc);
                    _argument_pointers.resize(_argument_strings.size() + 1);

                    for (int i = 0; i < _argc; ++i)
                    {
                        pybind11::str arg_py{argv_py[i]};
                        _argument_strings[i] = arg_py;
                        _argument_pointers[i] = _argument_strings[i].data();
                    }

                    lue_assert(_argument_pointers.size() == static_cast<std::size_t>(_argc + 1));
                    _argument_pointers[_argc] = nullptr;
                    _argv = _argument_pointers.data();
                }

                [[nodiscard]] auto argc() const -> int
                {
                    return _argc;
                }

                [[nodiscard]] auto argv() const -> char**
                {
                    return _argv;
                }

            private:

                // For each argument a string
                std::vector<std::string> _argument_strings;

                // For each argument a pointer to the array of characters
                std::vector<char*> _argument_pointers;

                // Command line arguments, to be used by HPX runtime startup code
                int _argc{};
                char** _argv{};
        };


        class PyRuntime: public Runtime
        {
            public:

                PyRuntime(int argc, char* argv[], std::vector<std::string> configuration):

                    Runtime{argc, argv, std::move(configuration)}

                {
                }


                PyRuntime(PyRuntime const&) = delete;

                PyRuntime(PyRuntime&&) = delete;

                virtual ~PyRuntime() = default;

                auto operator=(PyRuntime const&) -> PyRuntime& = delete;

                auto operator=(PyRuntime&&) -> PyRuntime& = delete;


                void startup() final
                {
                    // Redirect all console output to Python's stdout
                    std::unique_ptr<ScopedOstreamRedirect> stream;

                    {
                        pybind11::gil_scoped_acquire acquire;
                        stream = std::unique_ptr<ScopedOstreamRedirect>{};
                    }
                }


                void shutdown() final
                {
                    pybind11::gil_scoped_acquire acquire;
                    stream.reset();
                }

            private:

                std::unique_ptr<ScopedOstreamRedirect> stream{};
        };


        std::unique_ptr<PyRuntime> runtime{};


        void start_hpx_runtime(std::vector<std::string> const& configuration)
        {
            // Iff the pointer to the runtime is not pointing to an instance, instantiate one. This will start
            // the HPX runtime on the current locality.

            if (!runtime)
            {
                pybind11::gil_scoped_release release;
                CommandLine command_line{};
                runtime =
                    std::make_unique<PyRuntime>(command_line.argc(), command_line.argv(), configuration);
            }
        }


        void stop_hpx_runtime()
        {
            // Iff the pointer to the runtime is pointing to an instance, delete it. This will stop the HPX
            // runtime on the current locality.
            if (runtime)
            {
                // Stopping the runtime can actually take a long time, in code that doesn't use the Python
                // API: release the GIL
                pybind11::gil_scoped_release release;

                runtime.reset();

                lue_assert(!runtime);
            }
        }


        auto on_root_locality() -> bool
        {
            return hpx::find_here() == hpx::find_root_locality();
        }

    }  // Anonymous namespace


    void init_framework_x(pybind11::module& module)
    {
        pybind11::module submodule = module.def_submodule("framework_x");

        submodule.def("start_hpx_runtime", &start_hpx_runtime);
        submodule.def("stop_hpx_runtime", &stop_hpx_runtime);
        submodule.def("on_root_locality", &on_root_locality);

        bind_array(submodule);
        bind_field(submodule);
        bind_scalar(submodule);

        // bind_io(submodule);
        bind_local_operations(submodule);

        // Unless the user calls stop_hpx_runtime explicitly, we will do it automatically upon module unload
        auto atexit = pybind11::module_::import("atexit");
        atexit.attr("register")(pybind11::cpp_function([]() { stop_hpx_runtime(); }));
    }

}  // namespace lue::api
