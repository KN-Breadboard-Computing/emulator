#include <unity.h>
#include "emulator.h"
void setUp (void) {}
void tearDown (void) {}
void test_emulator_init(void) {
    Emulator emulator;
    init_emulator(&emulator);
    TEST_ASSERT_EQUAL(0, emulator.a_register);
    TEST_ASSERT_EQUAL(0, emulator.b_register);
    TEST_ASSERT_EQUAL(0, emulator.tmp_register);
    TEST_ASSERT_EQUAL(0, emulator.stack_pointer);
    TEST_ASSERT_EQUAL(0, emulator.program_counter);
    TEST_ASSERT_EQUAL(false, emulator.is_halted);
    TEST_ASSERT_EQUAL(0, emulator.clock_cycles_counter);
    TEST_ASSERT_EQUAL(emulator.instruction_counter, 0);
    for (unsigned int i = 0; i < sizeof(emulator.memory); i++) {
        TEST_ASSERT_EQUAL(0, emulator.memory[i]);
    }
    for (unsigned int i = 0; i < sizeof(emulator.stack); i++) {
        TEST_ASSERT_EQUAL(0, emulator.stack[i]);
    }
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_emulator_init);

    return UNITY_END();
}
