#include "unit_test.h"
#include "argc_argv.hpp"
#include <cstdlib>
#include <cstring>


// void filter_out_hpx_arguments(int* argc, char** argv)
// {
//     lue::detail::filter_out_hpx_arguments(*argc, argv);
// }
//
//
// auto copy_argv(int const argc, char** argv) -> char**
// {
//     char** copy = static_cast<char**>(malloc((argc + 1) * sizeof *copy));
//
//     for (int idx = 0; idx < argc; ++idx)
//     {
//         size_t length = strlen(argv[idx]) + 1;
//         copy[idx] = static_cast<char*>(malloc(length));
//         memcpy(copy[idx], argv[idx], length);
//     }
//     copy[argc] = NULL;
//
//     return copy;
// }
//
//
// void free_argv(int const argc, char** argv)
// {
//     for (int idx = 0; idx < argc; ++idx)
//     {
//         free(argv[idx]);
//     }
//
//     free(static_cast<void*>(argv));
// }
