# Add an executable with the above sources
add_executable(EmulatorCliApp
        main.c
        cli_app_utils.c
        cli_app_utils.h
        debugger.c
        debugger.h
        commands.c
        commands.h)
find_package(Curses REQUIRED)
target_link_libraries(EmulatorCliApp ${CURSES_LIBRARIES})
include_directories(${CURSES_INCLUDE_DIR})
# link the new hello_library target with the hello_binary target
target_link_libraries(EmulatorCliApp EmulatorLib)
