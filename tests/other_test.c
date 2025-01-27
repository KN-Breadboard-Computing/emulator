#include "emulator.h"
#include <stdlib.h>
#include <unity.h>

static const char *instruction_set_path = "../instructions.json";

const uint8_t INIT_A_REG_VAL = 57;
const uint8_t INIT_B_REG_VAL = (uint8_t)-37;

const uint8_t NOP_ROM[] = {
    0b11100001, // NOP
    0b11101100  // HALT
};

const uint8_t SKIP_ROM[] = {
    0b11100010, // SKIP
    0b11101100  // HALT
};

const uint8_t SKIP1_ROM[] = {
    0b00010101, // MOVAIMM
    INIT_A_REG_VAL,
    0b00010110, // MOVBIMM
    INIT_B_REG_VAL,
    0b11100011, // SKIP1
    0b10011001, // CLRA
    0b10011010, // CLRB
    0b11101100  // HALT
};

const uint8_t SKIP2_ROM[] = {
    0b00010101, // MOVAIMM
    INIT_A_REG_VAL,
    0b00010110, // MOVBIMM
    INIT_B_REG_VAL,
    0b11100100, // SKIP2
    0b10011001, // CLRA
    0b10011010, // CLRB
    0b11101100  // HALT
};

const uint8_t ISR_ROM[] = {
    0b11100101, // ISR
    0b11101100  // HALT
};

const uint8_t IRET_ROM[] = {
    0b11100110, // IRET
    0b11101100  // HALT
};

const uint8_t INT0_ROM[] = {
    0b11100111, // INT0
    0b11101100  // HALT
};

const uint8_t INT1_ROM[] = {
    0b11101000, // INT1
    0b11101100  // HALT
};

const uint8_t INT2_ROM[] = {
    0b11101001, // INT2
    0b11101100  // HALT
};

const uint8_t INT3_ROM[] = {
    0b11101010, // INT3
    0b11101100  // HALT
};

const uint8_t INT4_ROM[] = {
    0b11101011, // INT4
    0b11101100  // HALT
};

const uint8_t HALT_ROM[] = {
    0b11101100 // HALT
};

typedef struct {
    Emulator *emulator;
    Config *config;
    size_t rom_size;
} BundlePtr;

typedef union {
    uint16_t u16;

    struct {
        uint8_t low;
        uint8_t high;
    };
} u16_u8u8;

void setUp(void) {}

void tearDown(void) {}

void initialize_bundle(BundlePtr *bundle, const uint8_t *rom, size_t rom_size);
void run_emulator(BundlePtr *bundle);
void clear_emulator(BundlePtr *bundle);

void test_nop(void);
void test_skip(void);
void test_skip1(void);
void test_skip2(void);

void test_isr(void);
void test_iret(void);

void test_int0(void);
void test_int1(void);
void test_int2(void);
void test_int3(void);
void test_int4(void);

void test_halt(void);

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

void test_nop(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NOP_ROM, sizeof(NOP_ROM));

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(NOP_ROM) + 0, bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_skip(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SKIP_ROM, sizeof(SKIP_ROM));

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(SKIP_ROM) + 0, bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_skip1(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SKIP1_ROM, sizeof(SKIP1_ROM));

    TEST_ASSERT_EQUAL(0, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SKIP1_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_skip2(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SKIP2_ROM, sizeof(SKIP2_ROM));

    TEST_ASSERT_EQUAL(0, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SKIP2_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_isr(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ISR_ROM, sizeof(ISR_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(ISR_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_iret(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, IRET_ROM, sizeof(IRET_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(IRET_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_int0(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INT0_ROM, sizeof(INT0_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(INT0_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_int1(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INT1_ROM, sizeof(INT1_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(INT1_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_int2(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INT2_ROM, sizeof(INT2_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(INT2_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_int3(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INT3_ROM, sizeof(INT3_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(INT3_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_int4(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INT4_ROM, sizeof(INT4_ROM));

    TEST_IGNORE();

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(INT4_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_halt(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, HALT_ROM, sizeof(HALT_ROM));

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(HALT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nop);
    RUN_TEST(test_skip);
    RUN_TEST(test_skip1);
    RUN_TEST(test_skip2);

    RUN_TEST(test_isr);
    RUN_TEST(test_iret);

    RUN_TEST(test_int0);
    RUN_TEST(test_int1);
    RUN_TEST(test_int2);
    RUN_TEST(test_int3);
    RUN_TEST(test_int4);

    RUN_TEST(test_halt);

    return UNITY_END();
}