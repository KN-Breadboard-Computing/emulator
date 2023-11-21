# Add an executable with the above sources
add_executable(EmulatorCliApp main.c)

# link the new hello_library target with the hello_binary target
target_link_libraries(EmulatorCliApp PRIVATE EmulatorLib)

# Make sure the compiler can find the headers for our library
# when building the executable
target_include_directories(EmulatorCliApp PRIVATE ../src)
