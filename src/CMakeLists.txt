set(ASSIMPEXAMPLE_SOURCE_FILES assimpexample.cc)

add_executable(assimpexample
    ${ASSIMPEXAMPLE_INCLUDE_FILES}
    ${ASSIMPEXAMPLE_SOURCE_FILES}
)
target_link_libraries(assimpexample
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    ${ASSIMP_LIBRARY}
)

add_custom_command(TARGET assimpexample POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${RESOURCES_PATH} $<TARGET_FILE_DIR:assimpexample>/resources)

add_custom_command(TARGET assimpexample POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${GLFW_DLL_PATH} $<TARGET_FILE_DIR:assimpexample>)

add_custom_command(TARGET assimpexample POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${GLEW_DLL_PATH} $<TARGET_FILE_DIR:assimpexample>)

add_custom_command(TARGET assimpexample POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       ${ASSIMP_DLL_PATH} $<TARGET_FILE_DIR:assimpexample>)