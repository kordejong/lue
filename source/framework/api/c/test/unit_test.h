#pragma once
// #include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


// extern char** copy_argv(int argc, char** argv);
//
// extern void free_argv(int argc, char** argv);
//
// extern void filter_out_hpx_arguments(int* argc, char** argv);


// extern inline void print_failed_assertion_message(
//     char const* filename, int const line_nr, char const* message)
// {
//     fprintf(stderr, "%s:%d: failed assertion: %s\n", filename, line_nr, message);
// }
//
//
// #define assert_true(current_status, expression) \
//     if ((current_status) == EXIT_SUCCESS) \
//     { \
//         if (!(expression)) \
//         { \
//             char message[1024]; \
//             sprintf(message, "assert_true(%s)", #expression); \
//             print_failed_assertion_message(__FILE__, __LINE__, message); \
//             (current_status) = EXIT_FAILURE; \
//         } \
//     }


#ifdef __cplusplus
}
#endif
