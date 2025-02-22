#include "emulator.h"
#include <stdlib.h>
#include <unity.h>

static const char *instruction_set_path = "../instructions.json";

const uint16_t ADDR_VAL = 0x1234;
const uint8_t ADDR_HIGH_VAL = 0x12;
const uint8_t ADDR_LOW_VAL = 0x34;

const uint8_t OFFSET_VAL = 0x44;

const uint8_t HALT_MNEMONIC = 0b11111010;

const uint8_t JMPIMM_ROM[] = {
    0b10110010, // JMPIMM
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMS_ROM[] = {
    0b10110011, // JMPIMMS
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMNS_ROM[] = {
    0b10110100, // JMPIMMNS
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMTS_ROM[] = {
    0b10110101, // JMPIMMTS
    0b11111010  // HALT
};

const uint8_t JMPIMMTNS_ROM[] = {
    0b10110110, // JMPIMMTNS
    0b11111010  // HALT
};

const uint8_t JMPIMMP_ROM[] = {
    0b10110111, // JMPIMMP
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMNP_ROM[] = {
    0b10111000, // JMPIMMNP
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMTP_ROM[] = {
    0b10111001, // JMPIMMTP
    0b11111010  // HALT
};

const uint8_t JMPIMMTNP_ROM[] = {
    0b10111010, // JMPIMMTNP
    0b11111010  // HALT
};

const uint8_t JMPIMMZ_ROM[] = {
    0b10111011, // JMPIMMZ
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMNZ_ROM[] = {
    0b10111100, // JMPIMMNZ
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMTZ_ROM[] = {
    0b10111101, // JMPIMMTZ
    0b11111010  // HALT
};

const uint8_t JMPIMMTNZ_ROM[] = {
    0b10111110, // JMPIMMTNZ
    0b11111010  // HALT
};

const uint8_t JMPIMMC_ROM[] = {
    0b10111111, // JMPIMMC
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMNC_ROM[] = {
    0b11000000, // JMPIMMNC
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMTC_ROM[] = {
    0b11000001, // JMPIMMTC
    0b11111010  // HALT
};

const uint8_t JMPIMMTNC_ROM[] = {
    0b11000010, // JMPIMMTNC
    0b11111010  // HALT
};

const uint8_t JMPIMMO_ROM[] = {
    0b11000011, // JMPIMMO
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMNO_ROM[] = {
    0b11000100, // JMPIMMNO
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPIMMTO_ROM[] = {
    0b11000101, // JMPIMMTO
    0b11111010  // HALT
};

const uint8_t JMPIMMTNO_ROM[] = {
    0b11000110, // JMPIMMTNO
    0b11111010  // HALT
};

const uint8_t JMPREL_ROM[] = {
    0b11000111, // JMPREL
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELS_ROM[] = {
    0b11001000, // JMPRELS
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELNS_ROM[] = {
    0b11001001, // JMPRELNS
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELTLS_ROM[] = {
    0b11001010, // JMPRELTLS
    0b11111010  // HALT
};

const uint8_t JMPRELTLNS_ROM[] = {
    0b11001011, // JMPRELTLNS
    0b11111010  // HALT
};

const uint8_t JMPRELP_ROM[] = {
    0b11001100, // JMPRELP
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELNP_ROM[] = {
    0b11001101, // JMPRELNP
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELTLP_ROM[] = {
    0b11001110, // JMPRELTLP
    0b11111010  // HALT
};

const uint8_t JMPRELTLNP_ROM[] = {
    0b11001111, // JMPRELTLNP
    0b11111010  // HALT
};

const uint8_t JMPRELZ_ROM[] = {
    0b11010000, // JMPRELZ
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELNZ_ROM[] = {
    0b11010001, // JMPRELNZ
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELTLZ_ROM[] = {
    0b11010010, // JMPRELTLZ
    0b11111010  // HALT
};

const uint8_t JMPRELTLNZ_ROM[] = {
    0b11010011, // JMPRELTLNZ
    0b11111010  // HALT
};

const uint8_t JMPRELC_ROM[] = {
    0b11010100, // JMPRELC
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELNC_ROM[] = {
    0b11010101, // JMPRELNC
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELTLC_ROM[] = {
    0b11010110, // JMPRELTLC
    0b11111010  // HALT
};

const uint8_t JMPRELTLNC_ROM[] = {
    0b11010111, // JMPRELTLNC
    0b11111010  // HALT
};

const uint8_t JMPRELO_ROM[] = {
    0b11011000, // JMPRELO
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELNO_ROM[] = {
    0b11011001, // JMPRELNO
    OFFSET_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRELTLO_ROM[] = {
    0b11011010, // JMPRELTLO
    0b11111010  // HALT
};

const uint8_t JMPRELTLNO_ROM[] = {
    0b11011011, // JMPRELTLNO
    0b11111010  // HALT
};

const uint8_t JMPFUN_ROM[] = {
    0b11011100, // JMPFUN
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t JMPRET_ROM[] = {
    0b11011101, // JMPRET
    0b11111010  // HALT
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

void test_jmpimm(void);

void test_jmpimms(void);
void test_jmpimmns(void);
void test_jmpimmts(void);
void test_jmpimmtns(void);

void test_jmpimmp(void);
void test_jmpimmnp(void);
void test_jmpimmtp(void);
void test_jmpimmtnp(void);

void test_jmpimmz(void);
void test_jmpimmnz(void);
void test_jmpimmtz(void);
void test_jmpimmtnz(void);

void test_jmpimmc(void);
void test_jmpimmnc(void);
void test_jmpimmtc(void);
void test_jmpimmtnc(void);

void test_jmpimmo(void);
void test_jmpimmno(void);
void test_jmpimmto(void);
void test_jmpimmtno(void);

void test_jmprel(void);

void test_jmprels(void);
void test_jmprelns(void);
void test_jmpreltls(void);
void test_jmpreltlns(void);

void test_jmprelp(void);
void test_jmprelnp(void);
void test_jmpreltlp(void);
void test_jmpreltlnp(void);

void test_jmprelz(void);
void test_jmprelnz(void);
void test_jmpreltlz(void);
void test_jmpreltlnz(void);

void test_jmprelc(void);
void test_jmprelnc(void);
void test_jmpreltlc(void);
void test_jmpreltlnc(void);

void test_jmprelo(void);
void test_jmprelno(void);
void test_jmpreltlo(void);
void test_jmpreltlno(void);

void test_jmpfun(void);
void test_jmpret(void);

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

void test_jmpimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMM_ROM, sizeof(JMPIMM_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_jmpimms(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMS_ROM, sizeof(JMPIMMS_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMS_ROM, sizeof(JMPIMMS_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMS_ROM), bundle.emulator->program_counter);
}

void test_jmpimmns(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMNS_ROM, sizeof(JMPIMMNS_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMNS_ROM, sizeof(JMPIMMNS_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMNS_ROM), bundle.emulator->program_counter);
}

void test_jmpimmts(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTS_ROM, sizeof(JMPIMMTS_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTS_ROM, sizeof(JMPIMMTS_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTS_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtns(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTNS_ROM, sizeof(JMPIMMTNS_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTNS_ROM, sizeof(JMPIMMTNS_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTNS_ROM), bundle.emulator->program_counter);
}

void test_jmpimmp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMP_ROM, sizeof(JMPIMMP_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMP_ROM, sizeof(JMPIMMP_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMP_ROM), bundle.emulator->program_counter);
}

void test_jmpimmnp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMNP_ROM, sizeof(JMPIMMNP_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMNP_ROM, sizeof(JMPIMMNP_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMNP_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTP_ROM, sizeof(JMPIMMTP_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTP_ROM, sizeof(JMPIMMTP_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTP_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtnp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTNP_ROM, sizeof(JMPIMMTNP_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTNP_ROM, sizeof(JMPIMMTNP_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTNP_ROM), bundle.emulator->program_counter);
}

void test_jmpimmz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMZ_ROM, sizeof(JMPIMMZ_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMZ_ROM, sizeof(JMPIMMZ_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMZ_ROM), bundle.emulator->program_counter);
}

void test_jmpimmnz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMNZ_ROM, sizeof(JMPIMMNZ_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMNZ_ROM, sizeof(JMPIMMNZ_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMNZ_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTZ_ROM, sizeof(JMPIMMTZ_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTZ_ROM, sizeof(JMPIMMTZ_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTZ_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtnz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTNZ_ROM, sizeof(JMPIMMTNZ_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTNZ_ROM, sizeof(JMPIMMTNZ_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTNZ_ROM), bundle.emulator->program_counter);
}

void test_jmpimmc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMC_ROM, sizeof(JMPIMMC_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMC_ROM, sizeof(JMPIMMC_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMC_ROM), bundle.emulator->program_counter);
}

void test_jmpimmnc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMNC_ROM, sizeof(JMPIMMNC_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMNC_ROM, sizeof(JMPIMMNC_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMNC_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTC_ROM, sizeof(JMPIMMTC_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTC_ROM, sizeof(JMPIMMTC_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTC_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtnc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTNC_ROM, sizeof(JMPIMMTNC_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTNC_ROM, sizeof(JMPIMMTNC_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTNC_ROM), bundle.emulator->program_counter);
}

void test_jmpimmo(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMO_ROM, sizeof(JMPIMMO_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMO_ROM, sizeof(JMPIMMO_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMO_ROM), bundle.emulator->program_counter);
}

void test_jmpimmno(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMNO_ROM, sizeof(JMPIMMNO_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMNO_ROM, sizeof(JMPIMMNO_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMNO_ROM), bundle.emulator->program_counter);
}

void test_jmpimmto(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTO_ROM, sizeof(JMPIMMTO_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTO_ROM, sizeof(JMPIMMTO_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTO_ROM), bundle.emulator->program_counter);
}

void test_jmpimmtno(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPIMMTNO_ROM, sizeof(JMPIMMTNO_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPIMMTNO_ROM, sizeof(JMPIMMTNO_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPIMMTNO_ROM), bundle.emulator->program_counter);
}

void test_jmprel(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPREL_ROM, sizeof(JMPREL_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_jmprels(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELS_ROM, sizeof(JMPRELS_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELS_ROM, sizeof(JMPRELS_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELS_ROM), bundle.emulator->program_counter);
}

void test_jmprelns(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELNS_ROM, sizeof(JMPRELNS_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELNS_ROM, sizeof(JMPRELNS_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELNS_ROM), bundle.emulator->program_counter);
}

void test_jmpreltls(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLS_ROM, sizeof(JMPRELTLS_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLS_ROM, sizeof(JMPRELTLS_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLS_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlns(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLNS_ROM, sizeof(JMPRELTLNS_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLNS_ROM, sizeof(JMPRELTLNS_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_sign = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLNS_ROM), bundle.emulator->program_counter);
}

void test_jmprelp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELP_ROM, sizeof(JMPRELP_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELP_ROM, sizeof(JMPRELP_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELP_ROM), bundle.emulator->program_counter);
}

void test_jmprelnp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELNP_ROM, sizeof(JMPRELNP_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELNP_ROM, sizeof(JMPRELNP_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELNP_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLP_ROM, sizeof(JMPRELTLP_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLP_ROM, sizeof(JMPRELTLP_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLP_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlnp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLNP_ROM, sizeof(JMPRELTLNP_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLNP_ROM, sizeof(JMPRELTLNP_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_parity = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLNP_ROM), bundle.emulator->program_counter);
}

void test_jmprelz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELZ_ROM, sizeof(JMPRELZ_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELZ_ROM, sizeof(JMPRELZ_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELZ_ROM), bundle.emulator->program_counter);
}

void test_jmprelnz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELNZ_ROM, sizeof(JMPRELNZ_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELNZ_ROM, sizeof(JMPRELNZ_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELNZ_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLZ_ROM, sizeof(JMPRELTLZ_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLZ_ROM, sizeof(JMPRELTLZ_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLZ_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlnz(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLNZ_ROM, sizeof(JMPRELTLNZ_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLNZ_ROM, sizeof(JMPRELTLNZ_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_zero = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLNZ_ROM), bundle.emulator->program_counter);
}

void test_jmprelc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELC_ROM, sizeof(JMPRELC_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELC_ROM, sizeof(JMPRELC_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELC_ROM), bundle.emulator->program_counter);
}

void test_jmprelnc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELNC_ROM, sizeof(JMPRELNC_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELNC_ROM, sizeof(JMPRELNC_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELNC_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLC_ROM, sizeof(JMPRELTLC_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLC_ROM, sizeof(JMPRELTLC_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLZ_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlnc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLNC_ROM, sizeof(JMPRELTLNC_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLNC_ROM, sizeof(JMPRELTLNC_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_carry = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLNC_ROM), bundle.emulator->program_counter);
}

void test_jmprelo(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELO_ROM, sizeof(JMPRELO_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELO_ROM, sizeof(JMPRELO_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELO_ROM), bundle.emulator->program_counter);
}

void test_jmprelno(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELNO_ROM, sizeof(JMPRELNO_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELNO_ROM, sizeof(JMPRELNO_ROM));

    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELNO_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlo(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLO_ROM, sizeof(JMPRELTLO_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLO_ROM, sizeof(JMPRELTLO_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLO_ROM), bundle.emulator->program_counter);
}

void test_jmpreltlno(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRELTLNO_ROM, sizeof(JMPRELTLNO_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 0;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(OFFSET_VAL + 2, bundle.emulator->program_counter);

    clear_emulator(&bundle);

    initialize_bundle(&bundle, JMPRELTLNO_ROM, sizeof(JMPRELTLNO_ROM));

    bundle.emulator->tmp_register_16 = OFFSET_VAL;
    bundle.emulator->memory[OFFSET_VAL] = HALT_MNEMONIC;
    bundle.emulator->flag_overflow = 1;

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(sizeof(JMPRELTLNO_ROM), bundle.emulator->program_counter);
}

void test_jmpfun(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPFUN_ROM, sizeof(JMPFUN_ROM));

    bundle.emulator->memory[ADDR_VAL] = HALT_MNEMONIC;

    TEST_ASSERT_EQUAL(0, bundle.emulator->program_counter);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);
    TEST_ASSERT_EQUAL(2, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(3, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack[1]);

    clear_emulator(&bundle);
}

void test_jmpret(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, JMPRET_ROM, sizeof(JMPRET_ROM));

    bundle.emulator->stack_pointer = 2;
    bundle.emulator->stack[0] = ADDR_LOW_VAL;
    bundle.emulator->stack[1] = ADDR_HIGH_VAL;
    bundle.emulator->memory[ADDR_VAL + 1] = HALT_MNEMONIC;

    TEST_ASSERT_EQUAL(2, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(ADDR_HIGH_VAL, bundle.emulator->stack[1]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->program_counter);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    clear_emulator(&bundle);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_jmpimm);

    RUN_TEST(test_jmpimms);
    RUN_TEST(test_jmpimmns);
    RUN_TEST(test_jmpimmts);
    RUN_TEST(test_jmpimmtns);

    RUN_TEST(test_jmpimmp);
    RUN_TEST(test_jmpimmnp);
    RUN_TEST(test_jmpimmtp);
    RUN_TEST(test_jmpimmtnp);

    RUN_TEST(test_jmpimmz);
    RUN_TEST(test_jmpimmnz);
    RUN_TEST(test_jmpimmtz);
    RUN_TEST(test_jmpimmtnz);

    RUN_TEST(test_jmpimmc);
    RUN_TEST(test_jmpimmnc);
    RUN_TEST(test_jmpimmtc);
    RUN_TEST(test_jmpimmtnc);

    RUN_TEST(test_jmpimmo);
    RUN_TEST(test_jmpimmno);
    RUN_TEST(test_jmpimmto);
    RUN_TEST(test_jmpimmtno);

    RUN_TEST(test_jmprel);

    RUN_TEST(test_jmprels);
    RUN_TEST(test_jmprelns);
    RUN_TEST(test_jmpreltls);
    RUN_TEST(test_jmpreltlns);

    RUN_TEST(test_jmprelp);
    RUN_TEST(test_jmprelnp);
    RUN_TEST(test_jmpreltlp);
    RUN_TEST(test_jmpreltlnp);

    RUN_TEST(test_jmprelz);
    RUN_TEST(test_jmprelnz);
    RUN_TEST(test_jmpreltlz);
    RUN_TEST(test_jmpreltlnz);

    RUN_TEST(test_jmprelc);
    RUN_TEST(test_jmprelnc);
    RUN_TEST(test_jmpreltlc);
    RUN_TEST(test_jmpreltlnc);

    RUN_TEST(test_jmprelo);
    RUN_TEST(test_jmprelno);
    RUN_TEST(test_jmpreltlo);
    RUN_TEST(test_jmpreltlno);

    RUN_TEST(test_jmpfun);
    RUN_TEST(test_jmpret);

    return UNITY_END();
}