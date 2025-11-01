option(LUE_ENABLE_ASAN "Enable AddressSanitizer" FALSE)

if(MSVC)
    if(LUE_ENABLE_ASAN)
        string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
        string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
        add_compile_options(/fsanitize=address /fsanitize-address-use-after-return)
    endif()
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    option(LUE_ENABLE_LSAN "Enable LeakSanitizer" FALSE)
    option(LUE_ENABLE_TSAN "Enable ThreadSanitizer" FALSE)
    option(LUE_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" FALSE)

    if(NOT APPLE)
        option(LUE_ENABLE_MSAN "Enable MemorySanitizer" FALSE)
    endif()

    if(
        (LUE_ENABLE_ASAN AND (LUE_ENABLE_TSAN OR LUE_ENABLE_MSAN)) OR
        (LUE_ENABLE_LSAN AND (LUE_ENABLE_TSAN OR LUE_ENABLE_MSAN)) OR
        (LUE_ENABLE_TSAN AND LUE_ENABLE_MSAN)
    )
        message(FATAL_ERROR
            "Invalid sanitizer combination:\n"
            "    LUE_ENABLE_ASAN: ${LUE_ENABLE_ASAN}\n"
            "    LUE_ENABLE_LSAN: ${LUE_ENABLE_LSAN}\n"
            "    LUE_ENABLE_TSAN: ${LUE_ENABLE_TSAN}\n"
            "    LUE_ENABLE_MSAN: ${LUE_ENABLE_MSAN}"
        )
    endif()

    add_compile_options(
        -fno-omit-frame-pointer
        $<$<BOOL:${LUE_ENABLE_ASAN}>:-fsanitize=address>
        $<$<BOOL:${LUE_ENABLE_LSAN}>:-fsanitize=leak>
        $<$<BOOL:${LUE_ENABLE_MSAN}>:-fsanitize=memory>
        $<$<BOOL:${LUE_ENABLE_TSAN}>:-fsanitize=thread>
        $<$<BOOL:${LUE_ENABLE_UBSAN}>:-fsanitize=undefined>
    )

    add_link_options(
        $<$<BOOL:${LUE_ENABLE_ASAN}>:-fsanitize=address>
        $<$<BOOL:${LUE_ENABLE_LSAN}>:-fsanitize=leak>
        $<$<BOOL:${LUE_ENABLE_MSAN}>:-fsanitize=memory>
        $<$<BOOL:${LUE_ENABLE_TSAN}>:-fsanitize=thread>
        $<$<BOOL:${LUE_ENABLE_UBSAN}>:-fsanitize=undefined>
    )
endif()
