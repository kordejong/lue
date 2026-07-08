#include "hpx_runtime.hpp"
#include "lue/framework/core/assert.hpp"
#include <hpx/hpx_start.hpp>
#include <pybind11/pybind11.h>


namespace lue {

    HPXRuntime::HPXRuntime(std::vector<std::string> const& configuration):

        _running{false},
        _command_line{},
        _runtime{nullptr},
        _configuration{configuration}

    {
        hpx::function<int(int, char**)> start_function = hpx::bind_front(&HPXRuntime::hpx_main, this);

        hpx::init_params params{};
        params.cfg = _configuration;
        params.mode = hpx::runtime_mode::console;

        if (!hpx::start(start_function, _command_line.argc(), _command_line.argv(), params))
        {
            // Something went wrong while initializing the runtime. This early we can't generate any output,
            // just bail out.
            std::abort();
        }

        // Wait for the main HPX thread (hpx_main below) to have started running
        {
            std::unique_lock<std::mutex> lock(_startup_mutex);

            // First test whether the thread isn't already running
            if (!_running)
            {
                _startup_condition_variable.wait(lock);
            }

            lue_hpx_assert(_running);
        }
    }


    HPXRuntime::~HPXRuntime()
    {
        // Notify hpx_main to tear down the runtime
        {
            std::lock_guard<hpx::spinlock> lock(_teardown_mutex);

            _runtime = nullptr;                         // Reset pointer
            _teardown_condition_variable.notify_one();  // Signal exit
        }

        // Wait for the runtime to exit
        hpx::stop();
    }


    // Main HPX thread, does nothing but wait for the application to exit
    auto HPXRuntime::hpx_main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) -> int
    {
        // Store a pointer to the runtime here
        _runtime = hpx::get_runtime_ptr();

        // Signal to constructor that the main HPX thread has started running
        {
            std::lock_guard<std::mutex> lock(_startup_mutex);

            _running = true;
            _startup_condition_variable.notify_one();
        }

        // Now, wait for destructor to be called.
        {
            std::unique_lock<hpx::spinlock> lock(_teardown_mutex);

            // First test whether the destructor hasn't been called already
            if (_runtime != nullptr)
            {
                _teardown_condition_variable.wait(lock);
            }
        }

        // Tell the runtime it's OK to exit
        return hpx::finalize();
    }

}  // namespace lue
