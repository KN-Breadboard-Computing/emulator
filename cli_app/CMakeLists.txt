# Add an executable with the above sources
add_executable(EmulatorCliApp main.c)
find_package(Curses REQUIRED)
target_link_libraries(EmulatorCliApp ${CURSES_LIBRARIES})
include_directories(${CURSES_INCLUDE_DIR})
# link the new hello_library target with the hello_binary target
target_link_libraries(EmulatorCliApp EmulatorLib)
