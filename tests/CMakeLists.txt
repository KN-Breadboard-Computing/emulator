add_executable(baseTest base_test.c)
target_link_libraries(baseTest EmulatorLib Unity)
add_test(NAME BaseTest COMMAND baseTest)

add_executable(moveTest move_test.c)
target_link_libraries(moveTest EmulatorLib Unity)
add_test(NAME MoveTest COMMAND moveTest)

add_executable(aluTest alu_test.c)
target_link_libraries(aluTest EmulatorLib Unity)
add_test(NAME AluTest COMMAND aluTest)