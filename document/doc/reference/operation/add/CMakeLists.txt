set(target_name lue.doc.framework.api.operation.add)

add_executable(${target_name}
    add.cpp
)

target_link_libraries(${target_name}
    PRIVATE
        lue::framework_api_cxx
)

# TODO Create raster1.tif and raster2.tif

# add_test(NAME ${target_name}-test
#     COMMAND
#         ${Python_EXECUTABLE} ${HPXRUN}
#             "--runwrapper" ${LUE_QA_TEST_HPX_RUNWRAPPER}
#             "--parcelport" ${LUE_QA_TEST_HPX_PARCELPORT}
#             "--localities" ${LUE_QA_TEST_NR_LOCALITIES_PER_TEST}
#             "--thread" ${LUE_QA_TEST_NR_THREADS_PER_LOCALITY} --
#                 "$<TARGET_FILE:${target_name}>"
# )
