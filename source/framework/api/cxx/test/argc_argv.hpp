#pragma once
// #include <algorithm>
// #include <string_view>


namespace lue::detail {

    // inline void filter_out_hpx_arguments(int& argc, char** argv)
    // {
    //     auto* begin = argv;
    //     auto* end = argv + argc;
    //     auto* end_new = std::remove_if(
    //         begin,
    //         end,
    //         [](char* argument) -> bool { return std::string_view{argument}.starts_with("--hpx:"); });
    //     argc = static_cast<int>(std::distance(begin, end_new));
    // }

}  // namespace lue::detail
