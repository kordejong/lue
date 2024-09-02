#include "c_runtime.hpp"


namespace lue::api {

    CRuntime::CRuntime(int argc, char* argv[], std::vector<std::string> configuration):

        Runtime{argc, argv, std::move(configuration)}

    {
    }


    void CRuntime::startup()
    {
    }


    void CRuntime::shutdown()
    {
    }

}  // namespace lue::api
