#include "emulator.h"
#include <stdlib.h>
#include <unity.h>

static const char *instruction_set_path = "../instructions.json";

const uint8_t INIT_A_REG_VAL = 57;
const uint8_t INIT_B_REG_VAL = (uint8_t)-37;
const uint8_t INIT_INT_DATA_VAL = 0x99;

const uint16_t INIT_TMP_REG_VAL = 0x4587;
const uint8_t INIT_TH_REG_VAL = 0x45;
const uint8_t INIT_TL_REG_VAL = 0x87;

const uint8_t INIT_FLAG_REG_VAL = 0b10101010;

const uint8_t IMM_VAL = 0x99;

const uint16_t ADDR_VAL = 0x1234;
const uint16_t ADDR_ZP_VAL = 0x0034;
const uint8_t ADDR_HIGH_VAL = 0x12;
const uint8_t ADDR_LOW_VAL = 0x34;

const uint8_t PUSHA_ROM[] = {
    0b11011110, // PUSHA
    0b11111010  // HALT
};

const uint8_t PUSHB_ROM[] = {
    0b11011111, // PUSHB
    0b11111010  // HALT
};

const uint8_t PUSHTH_ROM[] = {
    0b11100000, // PUSHTH
    0b11111010  // HALT
};

const uint8_t PUSHTL_ROM[] = {
    0b11100001, // PUSHTL
    0b11111010  // HALT
};

const uint8_t PUSHF_ROM[] = {
    0b11100010, // PUSHF
    0b11111010  // HALT
};

const uint8_t PUSHINT_ROM[] = {
    0b11100011, // PUSHINT
    0b11111010  // HALT
};

const uint8_t PUSHT_ROM[] = {
    0b11100100, // PUSHT
    0b11111010  // HALT
};

const uint8_t PUSHIMM_ROM[] = {
    0b11100101, // PUSHIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t PUSHABS_ROM[] = {
    0b11100110, // PUSHABS
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t PUSHABSZP_ROM[] = {
    0b11100111, // PUSHABSZP
    ADDR_ZP_VAL,
    0b11111010 // HALT
};

const uint8_t POPA_ROM[] = {
    0b11101000, // POPA
    0b11111010  // HALT
};

const uint8_t POPB_ROM[] = {
    0b11101001, // POPB
    0b11111010  // HALT
};

const uint8_t POPTH_ROM[] = {
    0b11101010, // POPTH
    0b11111010  // HALT
};

const uint8_t POPTL_ROM[] = {
    0b11101011, // POPTL
    0b11111010  // HALT
};

const uint8_t POPT_ROM[] = {
    0b11101100, // POPT
    0b11111010  // HALT
};

const uint8_t POPMEM_ROM[] = {
    0b11101101, // POPMEM
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t POPMEMZP_ROM[] = {
    0b11101110, // POPMEMZP
    ADDR_ZP_VAL,
    0b11111010 // HALT
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

void test_pusha(void);
void test_pushb(void);
void test_pushth(void);
void test_pushtl(void);
void test_pushf(void);
void test_pushint(void);
void test_pusht(void);
void test_pushimm(void);
void test_pushabs(void);
void test_pushabszp(void);

void test_popa(void);
void test_popb(void);
void test_popth(void);
void test_poptl(void);
void test_popt(void);
void test_popmem(void);
void test_popmemzp(void);

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

void test_pusha(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHA_ROM, sizeof(PUSHA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHB_ROM, sizeof(PUSHB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHTH_ROM, sizeof(PUSHTH_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushtl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHTL_ROM, sizeof(PUSHTL_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(tmp.low, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushf(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHF_ROM, sizeof(PUSHF_ROM));

    bundle.emulator->flag_register = 0b10101010;

    TEST_ASSERT_EQUAL(0b10101010, bundle.emulator->flag_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(0b10101010, bundle.emulator->flag_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(0b10101010, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHF_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHINT_ROM, sizeof(PUSHINT_ROM));

    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->stack[0]);

    clear_emulator(&bundle);
}

void test_pusht(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHT_ROM, sizeof(PUSHT_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(2, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(tmp.low, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->stack[1]);

    TEST_ASSERT_EQUAL(sizeof(PUSHT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHIMM_ROM, sizeof(PUSHIMM_ROM));

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushabs(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHABS_ROM, sizeof(PUSHABS_ROM));

    bundle.emulator->memory[ADDR_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHABS_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_pushabszp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, PUSHABSZP_ROM, sizeof(PUSHABSZP_ROM));

    bundle.emulator->memory[ADDR_ZP_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(PUSHABSZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_popa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPA_ROM, sizeof(POPA_ROM));

    bundle.emulator->stack_pointer = 1;
    bundle.emulator->stack[0] = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->stack[0]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_popb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPB_ROM, sizeof(POPB_ROM));

    bundle.emulator->stack_pointer = 1;
    bundle.emulator->stack[0] = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->stack[0]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_popth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPTH_ROM, sizeof(POPTH_ROM));

    bundle.emulator->stack_pointer = 1;
    bundle.emulator->stack[0] = INIT_TH_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->stack[0]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_poptl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPTL_ROM, sizeof(POPTL_ROM));

    bundle.emulator->stack_pointer = 1;
    bundle.emulator->stack[0] = INIT_TL_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->stack[0]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_popt(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPT_ROM, sizeof(POPT_ROM));

    bundle.emulator->stack_pointer = 2;
    bundle.emulator->stack[0] = INIT_TL_REG_VAL;
    bundle.emulator->stack[1] = INIT_TH_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(2, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->stack[1]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_popmem(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPMEM_ROM, sizeof(POPMEM_ROM));

    bundle.emulator->stack_pointer = 1;
    bundle.emulator->stack[0] = IMM_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->stack[0]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);
    ;
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPMEM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_popmemzp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, POPMEMZP_ROM, sizeof(POPMEMZP_ROM));

    bundle.emulator->stack_pointer = 1;
    bundle.emulator->stack[0] = IMM_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->stack[0]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);
    ;
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(POPMEMZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_pusha);
    RUN_TEST(test_pushb);
    RUN_TEST(test_pushth);
    RUN_TEST(test_pushtl);
    RUN_TEST(test_pushf);
    RUN_TEST(test_pushint);
    RUN_TEST(test_pusht);
    RUN_TEST(test_pushimm);
    RUN_TEST(test_pushabs);
    RUN_TEST(test_pushabszp);

    RUN_TEST(test_popa);
    RUN_TEST(test_popb);
    RUN_TEST(test_popth);
    RUN_TEST(test_poptl);
    RUN_TEST(test_popt);
    RUN_TEST(test_popmem);
    RUN_TEST(test_popmemzp);

    return UNITY_END();
}
