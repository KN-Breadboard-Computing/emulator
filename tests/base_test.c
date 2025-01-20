#include "emulator.h"
#include <stdlib.h>
#include <unity.h>

static const char *instruction_set_path = "../computer/config/instructions.json";

const uint8_t IMM_VAL1 = 0b10101010;
const uint8_t IMM_VAL2 = 0b01010101;

const uint8_t SAMPLE_PROGRAM_ROM[] = {
    0b00010101, // MOVAIMM
    IMM_VAL1,
    0b00010110, // MOVBIMM
    IMM_VAL2,
    0b11101100 // HALT
};

typedef struct {
    Emulator *emulator;
    Config *config;
    size_t rom_size;
} BundlePtr;

void setUp(void) {}

void tearDown(void) {}

void initialize_bundle(BundlePtr *bundle, const uint8_t *rom, size_t rom_size);
void run_emulator(BundlePtr *bundle);
void clear_emulator(BundlePtr *bundle);

void test_emulator_init(void);
void test_sample_program(void);

void initialize_bundle(BundlePtr *bundle, const uint8_t *rom, size_t rom_size) {
    Emulator *emulator = (Emulator *)malloc(sizeof(Emulator));
    init_emulator(emulator);

    Config *config = (Config *)malloc(sizeof(Config));
    int config_load_result_code = load_config(config, instruction_set_path);
    TEST_ASSERT_EQUAL(0, config_load_result_code);

    for (size_t i = 0; i < rom_size; ++i) {
        emulator->memory[i] = rom[i];
    }

    bundle->emulator = emulator;
    bundle->config = config;
    bundle->rom_size = rom_size;
}

void run_emulator(BundlePtr *bundle) {
    while (bundle->emulator->is_halted == 0 && bundle->emulator->program_counter < bundle->rom_size) {
        if (run_next_emulator_instruction(bundle->emulator, bundle->config) != 0) {
            break;
        }
    }
}

void clear_emulator(BundlePtr *bundle) {
    free(bundle->emulator);
    free(bundle->config);
}

void test_emulator_init(void) {
    Emulator emulator;
    init_emulator(&emulator);

    TEST_ASSERT_EQUAL(0, emulator.a_register);
    TEST_ASSERT_EQUAL(0, emulator.b_register);
    TEST_ASSERT_EQUAL(0, emulator.tmp_register_16);
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

void test_sample_program(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SAMPLE_PROGRAM_ROM, sizeof(SAMPLE_PROGRAM_ROM));

    TEST_ASSERT_EQUAL(0, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL1, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL2, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SAMPLE_PROGRAM_ROM), bundle.emulator->program_counter);
    TEST_ASSERT_EQUAL(3, bundle.emulator->instruction_counter);
    TEST_ASSERT_EQUAL(10, bundle.emulator->clock_cycles_counter);

    clear_emulator(&bundle);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_emulator_init);
    RUN_TEST(test_sample_program);

    return UNITY_END();
}
