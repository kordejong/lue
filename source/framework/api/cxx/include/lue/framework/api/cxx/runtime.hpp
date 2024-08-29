#pragma once
#include "lue/framework/api/cxx/export.hpp"
#include <hpx/hpx.hpp>


namespace lue::api {

    class LUE_FAPI_EXPORT Runtime
    {

        public:

            Runtime(int argc, char* argv[], std::vector<std::string> configuration);

            Runtime(Runtime const&) = delete;

            Runtime(Runtime&&) = delete;

            auto operator=(Runtime const&) -> Runtime& = delete;

            auto operator=(Runtime&&) -> Runtime& = delete;

        protected:

            ~Runtime();

            virtual void startup() {};

            virtual void shutdown() {};

        private:

            auto hpx_main(int argc, char* argv[]) -> int;

            std::mutex _startup_mutex;

            std::condition_variable _startup_condition_variable;

            bool _running;

            hpx::runtime* _runtime;

            hpx::spinlock _shutdown_mutex;

            hpx::condition_variable_any _shutdown_condition_variable;
    };

}  // namespace lue::api
