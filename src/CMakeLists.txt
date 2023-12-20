# Create a library called YourLib which includes the source files.
# The extension is already found. Any number of sources could be listed here.
add_library(EmulatorLib emulator.c config.c)


target_link_libraries(EmulatorLib cJSON)

# Make sure the compiler can find include files for our library
# when other libraries or executables link to YourLib
target_include_directories(EmulatorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
    TARGET EmulatorLib POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
)
