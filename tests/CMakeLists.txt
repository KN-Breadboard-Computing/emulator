add_executable(baseTest base_test.c)
target_link_libraries(baseTest EmulatorLib Unity)
add_test(NAME BaseTest COMMAND baseTest)

add_executable(moveTest move_test.c)
target_link_libraries(moveTest EmulatorLib Unity)
add_test(NAME MoveTest COMMAND moveTest)

add_executable(aluTest alu_test.c)
target_link_libraries(aluTest EmulatorLib Unity)
add_test(NAME AluTest COMMAND aluTest)

add_executable(jumpTest jump_test.c)
target_link_libraries(jumpTest EmulatorLib Unity)
add_test(NAME JumpTest COMMAND jumpTest)

add_executable(stackTest stack_test.c)
target_link_libraries(stackTest EmulatorLib Unity)
add_test(NAME StackTest COMMAND stackTest)

add_executable(otherTest other_test.c)
target_link_libraries(otherTest EmulatorLib Unity)
add_test(NAME OtherTest COMMAND otherTest)