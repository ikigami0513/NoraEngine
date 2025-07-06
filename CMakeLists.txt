cmake_minimum_required(VERSION 3.10)

project(NoraEngine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FindPython)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)

include_directories(include)
include_directories(packages)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(NoraEngine ${SOURCES})

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Forcing RTTI ON for GCC/Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -frtti)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/glfw-3.4)

target_link_libraries(NoraEngine PRIVATE glfw)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/glad-core-gl-3.3)

target_link_libraries(NoraEngine PRIVATE glad)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/stb-image-1.0.0)

target_link_libraries(NoraEngine PRIVATE stb_image)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/raudio-1.0.0)

target_link_libraries(NoraEngine PRIVATE raudio)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/glm-1.0.1)

target_link_libraries(NoraEngine PRIVATE glm)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/pybind11-2.13.6)

target_link_libraries(NoraEngine PRIVATE pybind11::embed Python3::Python)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/freetype-2.13.3)

target_link_libraries(NoraEngine PRIVATE freetype)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/packages/assimp-6.0.2)

target_link_libraries(NoraEngine PRIVATE assimp)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(assimp PRIVATE -Wno-error)
endif()
