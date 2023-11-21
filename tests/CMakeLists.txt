add_executable(test1 test1.c)
target_link_libraries(test1 EmulatorLib Unity)
add_test(NAME Test1 COMMAND test1)
