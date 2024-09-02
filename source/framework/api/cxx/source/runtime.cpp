#include "lue/framework/api/cxx/runtime.hpp"
#include <hpx/hpx_start.hpp>
#include <cstring>


namespace lue::api {

    namespace {

        auto is_hpx_argument(char* arg) -> bool
        {
            return std::strncmp(arg, "--hpx:", 6) == 0;
        }

    }  // Anonymous namespace

    std::tuple<int, char**> Runtime::sieve_hpx_arguments(int argc, char* argv[])
    {
        int argc_new{0};

        for (int idx = 0; idx < argc; ++idx)
        {
            if (!is_hpx_argument(argv[idx]))
            {
                ++argc_new;
            }
        }

        char* argv_new[argc_new];

        for (int idx = 0, idx_new = 0; idx < argc; ++idx)
        {
            if (!is_hpx_argument(argv[idx]))
            {
                argv_new[idx_new++] = argv[idx];
            }
        }

        return {argc_new, argv_new};
    }


    auto Runtime::on_root_locality() -> bool
    {
        return hpx::find_here() == hpx::find_root_locality();
    }


    Runtime::Runtime(int const argc, char* argv[], std::vector<std::string> configuration):

        _argc{argc},
        _argv{argv},
        _configuration{std::move(configuration)},
        _runtime{nullptr},
        _main_thread_is_running{false}

    {
        assert(!is_running());
    }


    Runtime::~Runtime()
    {
        if (is_running())
        {
            (void)stop();
        }

        assert(!is_running());
    }


    auto Runtime::start() -> bool
    {
        // This function blocks until actually running
        assert(!is_running());

        // https://hpx-docs.stellar-group.org/latest/html/libs/full/init_runtime/api/hpx_start_impl.html
        // Function that will be scheduled as an HPX thread, once the runtime has started
        std::function<int(int, char**)> start_function = hpx::bind_front(&Runtime::hpx_main, this);

        // https://hpx-docs.stellar-group.org/latest/html/libs/full/init_runtime/api/hpx_init_params.html
        // The runtime mode will be determined based on the command line arguments
        hpx::init_params parameters{};
        parameters.cfg = _configuration;
        parameters.mode = hpx::runtime_mode::default_;

        // start returns immediately
        bool const main_thread_has_started = hpx::start(start_function, _argc, _argv, std::move(parameters));

        if (main_thread_has_started)
        {
            // Wait for the main HPX thread (hpx_main below) to have started running
            std::unique_lock<std::mutex> lock(_startup_mutex);

            while (!is_running())
            {
                _startup_condition_variable.wait(lock);
            }

            // Runtime is now ready to handle tasks
            assert(is_running());
        }

        return main_thread_has_started;
    }


    auto Runtime::stop() -> int
    {
        // This function blocks until actually stopped

        assert(is_running());

        // Notify hpx_main that it is OK to stop the runtime
        {
            std::lock_guard<hpx::spinlock> lock{_shutdown_mutex};
            _runtime = nullptr;  // Reset pointer
        }

        _shutdown_condition_variable.notify_one();  // Signal exit

        // Wait for the runtime to exit
        int const status = hpx::stop();

        _main_thread_is_running = false;

        assert(!is_running());

        return status;
    }


    // Main HPX thread, does nothing but wait for the application to exit
    auto Runtime::hpx_main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) -> int
    {
        assert(!is_running());

        // Store a pointer to the runtime here.
        _runtime = hpx::get_runtime_ptr();

        // Allow specialization to execute startup code
        startup();

        // Signal to start() that thread has started running.
        {
            std::lock_guard<std::mutex> lock(_startup_mutex);
            _main_thread_is_running = true;
        }

        assert(is_running());

        _startup_condition_variable.notify_one();

        // Wait for stop() to be called
        {
            std::unique_lock<hpx::spinlock> lock{_shutdown_mutex};

            if (_runtime != nullptr)
            {
                _shutdown_condition_variable.wait(lock);
            }
        }

        // Allow specialization to execute shutdown code
        shutdown();

        // Tell the runtime it's OK to exit
        return hpx::finalize();
    }


    auto Runtime::is_running() const -> bool
    {
        // return _runtime != nullptr;
        return _main_thread_is_running;
    }

}  // namespace lue::api
