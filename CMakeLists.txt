cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

option(ENABLE_CUSTOM_COMPILER_FLAGS "Enables custom compiler flags" ON)
if (ENABLE_CUSTOM_COMPILER_FLAGS)
    if (("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"))
        list(APPEND custom_compiler_flags
            -Wall
            -Wextra
            -Wstrict-prototypes
            -Wwrite-strings
            -Wshadow
            -Winit-self
            -Wcast-align
            -Wformat=2
            -Wstrict-overflow=2
            -Wcast-qual
            -Wundef
            -Wswitch-default
            -Wconversion
            -fstack-protector-strong
            -Wdouble-promotion
            -Wparentheses
            -Wformat-overflow
            -Wunused-macros
            -Wmissing-declarations
            -Wswitch-enum
        )
    elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
        # Disable warning c4001 - nonstandard extension 'single line comment' was used
        # Define _CRT_SECURE_NO_WARNINGS to disable deprecation warnings for "insecure" C library functions
        list(APPEND custom_compiler_flags
            /GS
            /Za
            /sdl
            /W4
            /wd4001
            /D_CRT_SECURE_NO_WARNINGS
        )
    endif()
endif()

option(ENABLE_SANITIZERS "Enables AddressSanitizer and UndefinedBehaviorSanitizer." OFF)
if (ENABLE_SANITIZERS)
    list(APPEND custom_compiler_flags
        -fno-omit-frame-pointer
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize=float-cast-overflow
        -fsanitize-address-use-after-scope
        -O1
        -fno-sanitize-recover
        )
endif()

option(ENABLE_SAFE_STACK "Enables the SafeStack instrumentation pass by the Code Pointer Integrity Project" OFF)
if (ENABLE_SAFE_STACK)
    if (ENABLE_SANITIZERS)
        message(FATAL_ERROR "ENABLE_SAFE_STACK cannot be used in combination with ENABLE_SANITIZERS")
    endif()
    list(APPEND custom_compiler_flags
        -fsanitize=safe-stack
        )
endif()

foreach(compiler_flag ${custom_compiler_flags})
    string(REGEX REPLACE "[^a-zA-Z0-9=\-]" "" current_variable ${compiler_flag})
    list(APPEND supported_compiler_flags)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${compiler_flag}")
endforeach()

include(FetchContent)

add_subdirectory(external)

add_subdirectory(src)

add_subdirectory(cli_app)

add_subdirectory(tests)
