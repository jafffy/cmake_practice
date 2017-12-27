cmake_minimum_required(VERSION 3.3)
project(graphics)

find_package(OpenGL REQUIRED)

set(GLFW_INCLUDE_PATH "" CACHE PATH "The directoruy that contains GLFW/glfw.h")
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib")

set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h")
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib")

set(GLM_INCLUDE_PATH "" CACHE PATH "")

set(ASSIMP_INCLUDE_PATH "" CACHE PATH "The directory that contains assimp/*.hpp and assimp/*.h")
set(ASSIMP_LIBRARY "" CACHE FILEPATH "libassimp.a or assimp.lib")

if (WIN32)
    set(GLFW_DLL_PATH "" CACHE FILEPATH "glfw.dll" )
    set(GLEW_DLL_PATH "" CACHE FILEPATH "glew.dll" )
    set(ASSIMP_DLL_PATH "" CACHE FILEPATH "assimp.dll" )
endif(WIN32)

set(BOOST_INCLUDE_PATH "" CACHE PATH "Boost")

set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources)
set(GRAPHICS_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

include_directories(
    ${GLFW_INCLUDE_PATH}
    ${GLEW_INCLUDE_PATH}
    ${GLM_INCLUDE_PATH}
    ${ASSIMP_INCLUDE_PATH}
    ${BOOST_INCLUDE_PATH}

    ${GRAPHICS_INCLUDE_PATH}
)

add_subdirectory(src)