cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Werror -Wshadow")

project(
    Exmulator
    VERSION 0.1
    LANGUAGES C
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    set(CMAKE_C_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)
endif()

include(FetchContent)

add_subdirectory(src)

add_subdirectory(cli_app)

add_subdirectory(tests)
