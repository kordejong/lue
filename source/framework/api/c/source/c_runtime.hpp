#pragma once
#include "lue/framework/api/cxx/runtime.hpp"


namespace lue::api {

    class CRuntime: public Runtime
    {

        public:

            CRuntime(int argc, char* argv[], std::vector<std::string> configuration);

            CRuntime(CRuntime const&) = delete;

            CRuntime(CRuntime&&) = delete;

            virtual ~CRuntime() = default;

            auto operator=(CRuntime const&) -> CRuntime& = delete;

            auto operator=(CRuntime&&) -> CRuntime& = delete;

            void startup() final;

            void shutdown() final;

        private:
    };

}  // namespace lue::api
