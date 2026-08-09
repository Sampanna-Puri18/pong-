cmake_minimum_required(VERSION 3.15)
project(PongPlus CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ---------------------------------------------------------------------------
# raylib: use an already-installed copy if CMake can find one, otherwise
# fetch and build it from source automatically. This keeps "clone and build"
# working out of the box on Windows/Linux/macOS without asking the user to
# install raylib by hand first.
# ---------------------------------------------------------------------------
find_package(raylib 4.5 QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.5
        GIT_SHALLOW    TRUE
    )
    # We only need the library, not raylib's own examples/games.
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(raylib)
endif()

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

# Assets are loaded from an absolute path baked in at configure time, so the
# game runs correctly no matter which directory you launch it from.
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")

# Also copy the assets next to the built executable, so the game is
# relocatable/shareable as a self-contained build folder if desired.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
