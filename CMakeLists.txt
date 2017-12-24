cmake_minimum_required(VERSION 3.3)
project(graphics)

find_package(OpenGL REQUIRED)

set(GLFW_INCLUDE_PATH "" CACHE PATH "The directoruy that contains GLFW/glfw.h")
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib")

set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h")
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib")

set(GLM_INCLUDE_PATH "" )

set(HELLO_HEADER_FILES )
set(HELLO_SOURCE_FILES hello.cc)

include_directories(
    ${GLFW_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
)

add_executable(hello 
    ${HELLO_HEADER_FILES}
    ${HELLO_SOURCE_FILES}
)
target_link_libraries(hello
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
)
set(REDTRIANGLE_HEADER_FILES )
set(REDTRIANGLE_SOURCE_FILES redtriangle.cc)

add_executable(redtriangle
    ${REDTRIANGLE_HEADER_FILES}
    ${REDTRIANGLE_SOURCE_FILES}
)
target_link_libraries(redtriangle
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
)