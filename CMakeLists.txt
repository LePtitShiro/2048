cmake_minimum_required(VERSION 4.0)
project(c_projets)

set(CMAKE_CXX_STANDARD 20)


add_executable(c_projets src/main.cpp
        src/App.cpp
        src/App.hpp
        src/Game.cpp
        src/Game.hpp
        src/Board.cpp
        src/Board.hpp
        src/Renderer.cpp
        src/Renderer.hpp
        src/Input.cpp
        src/Input.hpp
        extern/stb_easy_font.h)

# Try to find GLFW via package manager (vcpkg/Conan). If unavailable, fetch from source.
find_package(glfw3 CONFIG QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "glfw3 not found via package manager; fetching from source")
    include(FetchContent)
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)
endif()

find_package(OpenGL REQUIRED)

target_link_libraries(c_projets PRIVATE glfw)
# On Windows, OpenGL::GL is provided by FindOpenGL
target_link_libraries(c_projets PRIVATE OpenGL::GL)
# Include header-only dependency from extern/
target_include_directories(c_projets PRIVATE extern)