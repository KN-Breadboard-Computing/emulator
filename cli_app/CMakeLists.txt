# Add an executable with the above sources
add_executable(EmulatorCliApp main.c)

# link the new hello_library target with the hello_binary target
target_link_libraries(EmulatorCliApp EmulatorLib)
