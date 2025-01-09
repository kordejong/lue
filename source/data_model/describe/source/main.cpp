#include "describe.hpp"


auto main(int argc, char* argv[]) -> int
{
    return lue::utility::Describe({argv + 1, argv + argc}).run();
    // TODO return lue::utility::Describe(argc, argv).run();
}
