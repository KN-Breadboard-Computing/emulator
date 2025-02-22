#include "emulator.h"
#include <stdlib.h>
#include <unity.h>

static const char *instruction_set_path = "../instructions.json";

const uint8_t INIT_A_REG_VAL = 57;
const uint8_t INIT_B_REG_VAL = (uint8_t)-37;

const uint16_t INIT_TMP_REG_VAL = 0x4587;
const uint8_t INIT_TH_REG_VAL = 0x45;
const uint8_t INIT_TL_REG_VAL = 0x87;

const uint8_t IMM_VAL = 0x99;

const uint16_t ADDR_VAL = 0x1234;
const uint16_t ADDR_ZP_VAL = 0x0034;
const uint8_t ADDR_HIGH_VAL = 0x12;
const uint8_t ADDR_LOW_VAL = 0x34;

const uint8_t NEGAA_ROM[] = {
    0b01000111, // NEGAA
    0b11111010  // HALT
};

const uint8_t NEGAB_ROM[] = {
    0b01001000, // NEGAB
    0b11111010  // HALT
};

const uint8_t NEGBA_ROM[] = {
    0b01001001, // NEGBA
    0b11111010  // HALT
};

const uint8_t NEGBB_ROM[] = {
    0b01001010, // NEGBB
    0b11111010  // HALT
};

const uint8_t NEGMEMA_ROM[] = {
    0b01001011, // NEGMEMA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t NEGMEMB_ROM[] = {
    0b01001100, // NEGMEMB
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t NEGMEZPA_ROM[] = {
    0b01001101, // NEGMEZPA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t NEGMEZPB_ROM[] = {
    0b01001110, // NEGMEZPB
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t NEGSTCA_ROM[] = {
    0b01001111, // NEGSTCA
    0b11111010  // HALT
};

const uint8_t NEGSTCB_ROM[] = {
    0b01010000, // NEGSTCB
    0b11111010  // HALT
};

const uint8_t ADDA_ROM[] = {
    0b01010001, // ADDA
    0b11111010  // HALT
};

const uint8_t ADDB_ROM[] = {
    0b01010010, // ADDB
    0b11111010  // HALT
};

const uint8_t ADDMEM_ROM[] = {
    0b01010011, // ADDMEMA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t ADDMEZP_ROM[] = {
    0b01010100, // ADDMEZPA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t ADDSTC_ROM[] = {
    0b01010101, // ADDSTCA
    0b11111010  // HALT
};

const uint8_t SUBAAB_ROM[] = {
    0b01010110, // SUBAAB
    0b11111010  // HALT
};

const uint8_t SUBABA_ROM[] = {
    0b01010111, // SUBABA
    0b11111010  // HALT
};

const uint8_t SUBBAB_ROM[] = {
    0b01011000, // SUBBAB
    0b11111010  // HALT
};

const uint8_t SUBBBA_ROM[] = {
    0b01011001, // SUBBBA
    0b11111010  // HALT
};

const uint8_t SUBMEMAB_ROM[] = {
    0b01011010, // SUBMEMAB
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t SUBMEMBA_ROM[] = {
    0b01011011, // SUBMEMBA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t SUBMEZPAB_ROM[] = {
    0b01011100, // SUBMEZPAB
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t SUBMEZPBA_ROM[] = {
    0b01011101, // SUBMEZPBA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t SUBSTCAB_ROM[] = {
    0b01011110, // SUBSTCAB
    0b11111010  // HALT
};

const uint8_t SUBSTCBA_ROM[] = {
    0b01011111, // SUBSTCBA
    0b11111010  // HALT
};

const uint8_t DIV2AA_ROM[] = {
    0b01100000, // DIV2AA
    0b11111010  // HALT
};

const uint8_t DIV2AB_ROM[] = {
    0b01100001, // DIV2AB
    0b11111010  // HALT
};

const uint8_t DIV2BA_ROM[] = {
    0b01100010, // DIV2BA
    0b11111010  // HALT
};

const uint8_t DIV2BB_ROM[] = {
    0b01100011, // DIV2BB
    0b11111010  // HALT
};

const uint8_t DIV2MEMA_ROM[] = {
    0b01100100, // DIV2MEMA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t DIV2MEMB_ROM[] = {
    0b01100101, // DIV2MEMB
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t DIV2MEZPA_ROM[] = {
    0b01100110, // DIV2MEZPA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t DIV2MEZPB_ROM[] = {
    0b01100111, // DIV2MEZPB
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t DIV2STCA_ROM[] = {
    0b01101000, // DIV2STCA
    0b11111010  // HALT
};

const uint8_t DIV2STCB_ROM[] = {
    0b01101001, // DIV2STCB
    0b11111010  // HALT
};

const uint8_t INVAA_ROM[] = {
    0b01101010, // INVAA
    0b11111010  // HALT
};

const uint8_t INVAB_ROM[] = {
    0b01101011, // INVAB
    0b11111010  // HALT
};

const uint8_t INVBA_ROM[] = {
    0b01101100, // INVBA
    0b11111010  // HALT
};

const uint8_t INVBB_ROM[] = {
    0b01101101, // INVBB
    0b11111010  // HALT
};

const uint8_t INVMEMA_ROM[] = {
    0b01101110, // INVMEMA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t INVMEMB_ROM[] = {
    0b01101111, // INVMEMB
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t INVMEZPA_ROM[] = {
    0b01110000, // INVMEZPA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t INVMEZPB_ROM[] = {
    0b01110001, // INVMEZPB
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t INVSTCA_ROM[] = {
    0b01110010, // INVSTCA
    0b11111010  // HALT
};

const uint8_t INVSTCB_ROM[] = {
    0b01110011, // INVSTCB
    0b11111010  // HALT
};

const uint8_t ORAA_ROM[] = {
    0b01110100, // ORAA
    0b11111010  // HALT
};

const uint8_t ORAB_ROM[] = {
    0b01110101, // ORAB
    0b11111010  // HALT
};

const uint8_t ORMEM_ROM[] = {
    0b01110110, // ORMEM
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t ORMEZP_ROM[] = {
    0b01110111, // ORMEZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t ORSTC_ROM[] = {
    0b01111000, // ORSTC
    0b11111010  // HALT
};

const uint8_t ANDA_ROM[] = {
    0b01111001, // ANDA
    0b11111010  // HALT
};

const uint8_t ANDB_ROM[] = {
    0b01111010, // ANDB
    0b11111010  // HALT
};

const uint8_t ANDMEM_ROM[] = {
    0b01111011, // ANDMEM
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t ANDMEZP_ROM[] = {
    0b01111100, // ANDMEZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t ANDSTC_ROM[] = {
    0b01111101, // ANDSTC
    0b11111010  // HALT
};

const uint8_t XORA_ROM[] = {
    0b01111110, // XORA
    0b11111010  // HALT
};

const uint8_t XORB_ROM[] = {
    0b01111111, // XORB
    0b11111010  // HALT
};

const uint8_t XORMEM_ROM[] = {
    0b10000000, // XORMEM
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t XORMEZP_ROM[] = {
    0b10000001, // XORMEZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t XORSTC_ROM[] = {
    0b10000010, // XORSTC
    0b11111010  // HALT
};

const uint8_t SHRAA_ROM[] = {
    0b10000011, // SHRAA
    0b11111010  // HALT
};

const uint8_t SHRAB_ROM[] = {
    0b10000100, // SHRAB
    0b11111010  // HALT
};

const uint8_t SHRBA_ROM[] = {
    0b10000101, // SHRBA
    0b11111010  // HALT
};

const uint8_t SHRBB_ROM[] = {
    0b10000110, // SHRBB
    0b11111010  // HALT
};

const uint8_t SHRMEMA_ROM[] = {
    0b10000111, // SHRMEMA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t SHRMEMB_ROM[] = {
    0b10001000, // SHRMEMB
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t SHRMEZPA_ROM[] = {
    0b10001001, // SHRMEZPA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t SHRMEZPB_ROM[] = {
    0b10001010, // SHRMEZPB
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t SHRSTCA_ROM[] = {
    0b10001011, // SHRSTCA
    0b11111010  // HALT
};

const uint8_t SHRSTCB_ROM[] = {
    0b10001100, // SHRSTCB
    0b11111010  // HALT
};

const uint8_t SHLAA_ROM[] = {
    0b10001101, // SHLAA
    0b11111010  // HALT
};

const uint8_t SHLAB_ROM[] = {
    0b10001110, // SHLAB
    0b11111010  // HALT
};

const uint8_t SHLBA_ROM[] = {
    0b10001111, // SHLBA
    0b11111010  // HALT
};

const uint8_t SHLBB_ROM[] = {
    0b10010000, // SHLBB
    0b11111010  // HALT
};

const uint8_t SHLMEMA_ROM[] = {
    0b10010001, // SHLMEMA
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t SHLMEMB_ROM[] = {
    0b10010010, // SHLMEMB
    ADDR_LOW_VAL, ADDR_HIGH_VAL,
    0b11111010 // HALT
};

const uint8_t SHLMEZPA_ROM[] = {
    0b10010011, // SHLMEZPA
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t SHLMEZPB_ROM[] = {
    0b10010100, // SHLMEZPB
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t SHLSTCA_ROM[] = {
    0b10010101, // SHLSTCA
    0b11111010  // HALT
};

const uint8_t SHLSTCB_ROM[] = {
    0b10010110, // SHLSTCB
    0b11111010  // HALT
};

const uint8_t CMPAB_ROM[] = {
    0b10010111, // CMPAB
    0b11111010  // HALT
};

const uint8_t CMPBA_ROM[] = {
    0b10011000, // CMPBA
    0b11111010  // HALT
};

const uint8_t CMPTHTL_ROM[] = {
    0b10011001, // CMPMTHTL
    0b11111010  // HALT
};

const uint8_t CMPTLTH_ROM[] = {
    0b10011010, // CMPTLTH
    0b11111010  // HALT
};

const uint8_t CMPTHA_ROM[] = {
    0b10011011, // CMPTHA
    0b11111010  // HALT
};

const uint8_t CMPATH_ROM[] = {
    0b10011100, // CMPATH
    0b11111010  // HALT
};

const uint8_t CMPTHB_ROM[] = {
    0b10011101, // CMPTHB
    0b11111010  // HALT
};

const uint8_t CMPBTH_ROM[] = {
    0b10011110, // CMPBTH
    0b11111010  // HALT
};

const uint8_t CMPTLA_ROM[] = {
    0b10011111, // CMPTLA
    0b11111010  // HALT
};

const uint8_t CMPATL_ROM[] = {
    0b10100000, // CMPATL
    0b11111010  // HALT
};

const uint8_t CMPTLB_ROM[] = {
    0b10100001, // CMPTLB
    0b11111010  // HALT
};

const uint8_t CMPBTL_ROM[] = {
    0b10100010, // CMPBTL
    0b11111010  // HALT
};

const uint8_t CMPINTA_ROM[] = {
    0b10100011, // CMPINTA
    0b11111010  // HALT
};

const uint8_t CMPAINT_ROM[] = {
    0b10100100, // CMPAINT
    0b11111010  // HALT
};

const uint8_t CMPINTB_ROM[] = {
    0b10100101, // CMPINTB
    0b11111010  // HALT
};

const uint8_t CMPBINT_ROM[] = {
    0b10100110, // CMPBINT
    0b11111010  // HALT
};

const uint8_t CLRA_ROM[] = {
    0b10100111, // CLRA
    0b11111010  // HALT
};

const uint8_t CLRB_ROM[] = {
    0b10101000, // CLRB
    0b11111010  // HALT
};

const uint8_t CLRTH_ROM[] = {
    0b10101001, // CLRTH
    0b11111010  // HALT
};

const uint8_t CLRTL_ROM[] = {
    0b10101010, // CLRTL
    0b11111010  // HALT
};

const uint8_t CLRT_ROM[] = {
    0b10101011, // CLRT
    0b11111010  // HALT
};

const uint8_t INCA_ROM[] = {
    0b10101100, // INCA
    0b11111010  // HALT
};

const uint8_t INCB_ROM[] = {
    0b10101101, // INCB
    0b11111010  // HALT
};

const uint8_t INCT_ROM[] = {
    0b10101110, // INCT
    0b11111010  // HALT
};

const uint8_t DECA_ROM[] = {
    0b10101111, // DECA
    0b11111010  // HALT
};

const uint8_t DECB_ROM[] = {
    0b10110000, // DECB
    0b11111010  // HALT
};

const uint8_t DECT_ROM[] = {
    0b10110001, // DECT
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

void test_negaa(void);
void test_negab(void);
void test_negba(void);
void test_negbb(void);
void test_negmema(void);
void test_negmemb(void);
void test_negmezpa(void);
void test_negmezpb(void);
void test_negstca(void);
void test_negstcb(void);

void test_adda(void);
void test_addb(void);
void test_addmem(void);
void test_addmezp(void);
void test_addstc(void);

void test_subaab(void);
void test_subaba(void);
void test_subbab(void);
void test_subbba(void);
void test_submemab(void);
void test_submemba(void);
void test_submezpab(void);
void test_submezpba(void);
void test_substcab(void);
void test_substcba(void);

void test_div2aa(void);
void test_div2ab(void);
void test_div2ba(void);
void test_div2bb(void);
void test_div2mema(void);
void test_div2memb(void);
void test_div2mezpa(void);
void test_div2mezpb(void);
void test_div2stca(void);
void test_div2stcb(void);

void test_invaa(void);
void test_invab(void);
void test_invba(void);
void test_invbb(void);
void test_invmema(void);
void test_invmemb(void);
void test_invmezpa(void);
void test_invmezpb(void);
void test_invstca(void);
void test_invstcb(void);

void test_oraa(void);
void test_orab(void);
void test_ormem(void);
void test_ormezp(void);
void test_orstc(void);

void test_anda(void);
void test_andb(void);
void test_andmem(void);
void test_andmezp(void);
void test_andstc(void);

void test_xora(void);
void test_xorb(void);
void test_xormem(void);
void test_xormezp(void);
void test_xorstc(void);

void test_shraa(void);
void test_shrab(void);
void test_shrba(void);
void test_shrbb(void);
void test_shrmema(void);
void test_shrmemb(void);
void test_shrmezpa(void);
void test_shrmezpb(void);
void test_shrstca(void);
void test_shrstcb(void);

void test_shlaa(void);
void test_shlab(void);
void test_shlba(void);
void test_shlbb(void);
void test_shlmema(void);
void test_shlmemb(void);
void test_shlmezpa(void);
void test_shlmezpb(void);
void test_shlstca(void);
void test_shlstcb(void);

uint8_t cmp_flags(uint8_t reg1, uint8_t reg2);

void test_cmpab(void);
void test_cmpba(void);
void test_cmpthtl(void);
void test_cmptlth(void);
void test_cmptha(void);
void test_cmpath(void);
void test_cmpthb(void);
void test_cmpbth(void);
void test_cmptla(void);
void test_cmpatl(void);
void test_cmptlb(void);
void test_cmpbtl(void);
void test_cmpinta(void);
void test_cmpaint(void);
void test_cmpintb(void);
void test_cmpbint(void);

void test_clra(void);
void test_clrb(void);
void test_clrth(void);
void test_clrtl(void);
void test_clrt(void);

void test_inca(void);
void test_incb(void);
void test_inct(void);

void test_deca(void);
void test_decb(void);
void test_dect(void);

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

void test_negaa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGAA_ROM, sizeof(NEGAA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)-INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(NEGAA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGAB_ROM, sizeof(NEGAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL((uint8_t)-INIT_B_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(NEGAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGBA_ROM, sizeof(NEGBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL((uint8_t)-INIT_A_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(NEGBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negbb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGBB_ROM, sizeof(NEGBB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)-INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(NEGBB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negmema(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGMEMA_ROM, sizeof(NEGMEMA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)-INIT_A_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(NEGMEMA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negmemb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGMEMB_ROM, sizeof(NEGMEMB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)-INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(NEGMEMB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negmezpa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGMEZPA_ROM, sizeof(NEGMEZPA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)-INIT_A_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(NEGMEZPA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negmezpb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGMEZPB_ROM, sizeof(NEGMEZPB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)-INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(NEGMEZPB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negstca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGSTCA_ROM, sizeof(NEGSTCA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL((uint8_t)-INIT_A_REG_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(NEGSTCA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_negstcb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, NEGSTCB_ROM, sizeof(NEGSTCB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);
    TEST_ASSERT_EQUAL((uint8_t)-INIT_B_REG_VAL, bundle.emulator->stack[0]);

    TEST_ASSERT_EQUAL(sizeof(NEGSTCB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_adda(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ADDA_ROM, sizeof(ADDA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL + INIT_B_REG_VAL), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(ADDA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_addb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ADDB_ROM, sizeof(ADDB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL + INIT_B_REG_VAL), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(ADDB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_addmem(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ADDMEM_ROM, sizeof(ADDMEM_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL + INIT_B_REG_VAL), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(ADDMEM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_addmezp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ADDMEZP_ROM, sizeof(ADDMEZP_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL + INIT_B_REG_VAL), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(ADDMEZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_addstc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ADDSTC_ROM, sizeof(ADDSTC_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL + INIT_B_REG_VAL), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(ADDSTC_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_subaab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBAAB_ROM, sizeof(SUBAAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL - INIT_B_REG_VAL), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(SUBAAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_subaba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBABA_ROM, sizeof(SUBABA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL - INIT_A_REG_VAL), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(SUBABA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_subbab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBBAB_ROM, sizeof(SUBBAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL - INIT_B_REG_VAL), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SUBBAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_subbba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBBBA_ROM, sizeof(SUBBBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL - INIT_A_REG_VAL), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SUBBBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_submemab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBMEMAB_ROM, sizeof(SUBMEMAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL - INIT_B_REG_VAL), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SUBMEMAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_submemba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBMEMBA_ROM, sizeof(SUBMEMBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL - INIT_A_REG_VAL), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SUBMEMBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_submezpab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBMEZPAB_ROM, sizeof(SUBMEZPAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL - INIT_B_REG_VAL), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SUBMEZPAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_submezpba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBMEZPBA_ROM, sizeof(SUBMEZPBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL - INIT_A_REG_VAL), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SUBMEZPBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_substcab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBSTCAB_ROM, sizeof(SUBSTCAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL - INIT_B_REG_VAL), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(SUBSTCAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_substcba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SUBSTCBA_ROM, sizeof(SUBSTCBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL - INIT_A_REG_VAL), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(SUBSTCBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2aa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2AA_ROM, sizeof(DIV2AA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    uint8_t expected = INIT_A_REG_VAL < 128 ? INIT_A_REG_VAL / 2 : INIT_A_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(DIV2AA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2ab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2AB_ROM, sizeof(DIV2AB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    uint8_t expected = INIT_B_REG_VAL < 128 ? INIT_B_REG_VAL / 2 : INIT_B_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(DIV2AB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2ba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2BA_ROM, sizeof(DIV2BA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    uint8_t expected = INIT_A_REG_VAL < 128 ? INIT_A_REG_VAL / 2 : INIT_A_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(DIV2BA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2bb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2BB_ROM, sizeof(DIV2BB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    uint8_t expected = INIT_B_REG_VAL < 128 ? INIT_B_REG_VAL / 2 : INIT_B_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(DIV2BB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2mema(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2MEMA_ROM, sizeof(DIV2MEMA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    uint8_t expected = INIT_A_REG_VAL < 128 ? INIT_A_REG_VAL / 2 : INIT_A_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(DIV2MEMA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2memb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2MEMB_ROM, sizeof(DIV2MEMB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    uint8_t expected = INIT_B_REG_VAL < 128 ? INIT_B_REG_VAL / 2 : INIT_B_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(DIV2MEMB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2mezpa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2MEZPA_ROM, sizeof(DIV2MEZPA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    uint8_t expected = INIT_A_REG_VAL < 128 ? INIT_A_REG_VAL / 2 : INIT_A_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(DIV2MEZPA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2mezpb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2MEZPB_ROM, sizeof(DIV2MEZPB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    uint8_t expected = INIT_B_REG_VAL < 128 ? INIT_B_REG_VAL / 2 : INIT_B_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(DIV2MEZPB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2stca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2STCA_ROM, sizeof(DIV2STCA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    uint8_t expected = INIT_A_REG_VAL < 128 ? INIT_A_REG_VAL / 2 : INIT_A_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(DIV2STCA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_div2stcb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DIV2STCB_ROM, sizeof(DIV2STCB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    uint8_t expected = INIT_B_REG_VAL < 128 ? INIT_B_REG_VAL / 2 : INIT_B_REG_VAL / 2 + 128;
    TEST_ASSERT_EQUAL(expected, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(DIV2STCB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invaa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVAA_ROM, sizeof(INVAA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(INVAA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVAB_ROM, sizeof(INVAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_B_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(INVAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVBA_ROM, sizeof(INVBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_A_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(INVBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invbb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVBB_ROM, sizeof(INVBB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(INVBB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invmema(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVMEMA_ROM, sizeof(INVMEMA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_A_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(INVMEMA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invmemb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVMEMB_ROM, sizeof(INVMEMB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(INVMEMB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invmezpa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVMEZPA_ROM, sizeof(INVMEZPA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_A_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(INVMEZPA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invmezpb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVMEZPB_ROM, sizeof(INVMEZPB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(INVMEZPB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invstca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVSTCA_ROM, sizeof(INVSTCA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_A_REG_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(INVSTCA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_invstcb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INVSTCB_ROM, sizeof(INVSTCB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(255 ^ INIT_B_REG_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(INVSTCB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_oraa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ORAA_ROM, sizeof(ORAA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL | INIT_B_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(ORAA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_orab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ORAB_ROM, sizeof(ORAB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL | INIT_A_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(ORAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_ormem(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ORMEM_ROM, sizeof(ORMEM_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL | INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(ORMEM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_ormezp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ORMEZP_ROM, sizeof(ORMEZP_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL | INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(ORMEZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_orstc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ORSTC_ROM, sizeof(ORSTC_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL | INIT_B_REG_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(ORSTC_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_anda(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ANDA_ROM, sizeof(ANDA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL & INIT_B_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(ANDA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_andb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ANDB_ROM, sizeof(ANDB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL & INIT_A_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(ANDB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_andmem(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ANDMEM_ROM, sizeof(ANDMEM_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL & INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(ANDMEM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_andmezp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ANDMEZP_ROM, sizeof(ANDMEZP_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL & INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(ANDMEZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_andstc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, ANDSTC_ROM, sizeof(ANDSTC_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL & INIT_B_REG_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(ANDSTC_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_xora(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, XORA_ROM, sizeof(XORA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL ^ INIT_B_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(XORA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_xorb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, XORB_ROM, sizeof(XORB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL ^ INIT_A_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(XORB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_xormem(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, XORMEM_ROM, sizeof(XORMEM_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL ^ INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(XORMEM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_xormezp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, XORMEZP_ROM, sizeof(XORMEZP_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL ^ INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(XORMEZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_xorstc(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, XORSTC_ROM, sizeof(XORSTC_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL ^ INIT_B_REG_VAL, bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(XORSTC_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shraa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRAA_ROM, sizeof(SHRAA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL >> 1), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(SHRAA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRAB_ROM, sizeof(SHRAB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL >> 1), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(SHRAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRBA_ROM, sizeof(SHRBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL >> 1), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SHRBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrbb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRBB_ROM, sizeof(SHRBB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL >> 1), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SHRBB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrmema(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRMEMA_ROM, sizeof(SHRMEMA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL >> 1), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHRMEMA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrmemb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRMEMB_ROM, sizeof(SHRMEMB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL >> 1), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHRMEMB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrmezpa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRMEZPA_ROM, sizeof(SHRMEZPA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL >> 1), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHRMEZPA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrmezpb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRMEZPB_ROM, sizeof(SHRMEZPB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL >> 1), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHRMEZPB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrstca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRSTCA_ROM, sizeof(SHRSTCA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL >> 1), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(SHRSTCA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shrstcb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHRSTCB_ROM, sizeof(SHRSTCB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL >> 1), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(SHRSTCB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlaa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLAA_ROM, sizeof(SHLAA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL << 1), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(SHLAA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLAB_ROM, sizeof(SHLAB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL << 1), bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(SHLAB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLBA_ROM, sizeof(SHLBA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL << 1), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SHLBA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlbb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLBB_ROM, sizeof(SHLBB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL << 1), bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(SHLBB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlmema(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLMEMA_ROM, sizeof(SHLMEMA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL << 1), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHLMEMA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlmemb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLMEMB_ROM, sizeof(SHLMEMB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL << 1), bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHLMEMB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlmezpa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLMEZPA_ROM, sizeof(SHLMEZPA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL << 1), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHLMEZPA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlmezpb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLMEZPB_ROM, sizeof(SHLMEZPB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL << 1), bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(SHLMEZPB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlstca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLSTCA_ROM, sizeof(SHLSTCA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_A_REG_VAL << 1), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(SHLSTCA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_shlstcb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, SHLSTCB_ROM, sizeof(SHLSTCB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->stack_pointer);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL((uint8_t)(INIT_B_REG_VAL << 1), bundle.emulator->stack[0]);
    TEST_ASSERT_EQUAL(1, bundle.emulator->stack_pointer);

    TEST_ASSERT_EQUAL(sizeof(SHLSTCB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

uint8_t cmp_flags(uint8_t reg1, uint8_t reg2) {
    uint16_t reg1_u16 = reg1;
    uint16_t reg2_u16 = reg2;
    uint16_t reg2_xor_u16 = reg2_u16 ^ 0xFF;
    uint16_t result_u16 = reg1_u16 + reg2_xor_u16 + 1;

    int reg1_signed = reg1 < 128 ? reg1 : reg1 - 256;
    int reg2_signed = reg2 < 128 ? reg2 : reg2 - 256;
    int result_signed = reg1_signed - reg2_signed;

    uint8_t flags = 0;
    if (result_u16 & 0b10000000) {
        flags |= 0b10000000; // set sign flag
    }
    if (result_u16 & 0b00000001) {
        flags |= 0b01000000; // set parity flag
    }
    if (result_u16 == 0) {
        flags |= 0b00100000; // set zero flag
    }
    if (result_u16 > 255) {
        flags |= 0b00010000; // set carry flag
    }
    if (reg1_signed * reg2_signed > 0 && result_signed * reg1_signed < 0) {
        flags |= 0b00001000; // set overflow flag
    }

    return flags;
}

void test_cmpab(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPAB_ROM, sizeof(CMPAB_ROM));

    // bundle.emulator->a_register = INIT_A_REG_VAL;
    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_A_REG_VAL, INIT_B_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPAB_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpba(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPBA_ROM, sizeof(CMPBA_ROM));

    // bundle.emulator->a_register = INIT_A_REG_VAL;
    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_B_REG_VAL, INIT_A_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPBA_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpthtl(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPTHTL_ROM, sizeof(CMPTHTL_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;
    // bundle.emulator->tmp_register_16 = tmp.u16;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(tmp.high, tmp.low), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPTHTL_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmptlth(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPTLTH_ROM, sizeof(CMPTLTH_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;
    // bundle.emulator->tmp_register_16 = tmp.u16;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(tmp.low, tmp.high), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPTLTH_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmptha(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPTHB_ROM, sizeof(CMPTHB_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->a_register = INIT_A_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(tmp.high, INIT_A_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPTHB_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpath(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPATH_ROM, sizeof(CMPATH_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->a_register = INIT_A_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_A_REG_VAL, tmp.high), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPATH_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpthb(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPTHB_ROM, sizeof(CMPTHB_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(tmp.high, INIT_B_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPTHB_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpbth(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPBTH_ROM, sizeof(CMPBTH_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_B_REG_VAL, tmp.high), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPBTH_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmptla(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPTLA_ROM, sizeof(CMPTLA_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->a_register = INIT_A_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(tmp.low, INIT_A_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPTLA_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpatl(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPATL_ROM, sizeof(CMPATL_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->a_register = INIT_A_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_A_REG_VAL, tmp.low), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPATL_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmptlb(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPTLB_ROM, sizeof(CMPTLB_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(tmp.low, INIT_B_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPTLB_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpbtl(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPBTL_ROM, sizeof(CMPBTL_ROM));

    // u16_u8u8 tmp;
    // tmp.u16 = INIT_TMP_REG_VAL;

    // bundle.emulator->tmp_register_16 = tmp.u16;
    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_B_REG_VAL, tmp.low), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPBTL_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpinta(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPINTA_ROM, sizeof(CMPINTA_ROM));

    // bundle.emulator->a_register = INIT_A_REG_VAL;

    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_A_REG_VAL, 0), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPINTA_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpaint(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPAINT_ROM, sizeof(CMPAINT_ROM));

    // bundle.emulator->a_register = INIT_A_REG_VAL;

    // TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(0, INIT_A_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPAINT_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpintb(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPINTB_ROM, sizeof(CMPINTB_ROM));

    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(INIT_B_REG_VAL, 0), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPINTB_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_cmpbint(void) {
    TEST_IGNORE();
    // BundlePtr bundle;
    // initialize_bundle(&bundle, CMPBINT_ROM, sizeof(CMPBINT_ROM));

    // bundle.emulator->b_register = INIT_B_REG_VAL;

    // TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    // run_emulator(&bundle);

    // TEST_ASSERT_EQUAL(cmp_flags(0, INIT_B_REG_VAL), bundle.emulator->flag_register);

    // TEST_ASSERT_EQUAL(sizeof(CMPBINT_ROM), bundle.emulator->program_counter);

    // clear_emulator(&bundle);
}

void test_clra(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, CLRA_ROM, sizeof(CLRA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(0, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(CLRA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_clrb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, CLRB_ROM, sizeof(CLRB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(0, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(CLRB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_clrth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, CLRTH_ROM, sizeof(CLRTH_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(tmp.low, bundle.emulator->tmp_register_8[0]);

    TEST_ASSERT_EQUAL(sizeof(CLRTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_clrtl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, CLRTL_ROM, sizeof(CLRTL_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[0]);

    TEST_ASSERT_EQUAL(sizeof(CLRTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_clrt(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, CLRT_ROM, sizeof(CLRT_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(0, bundle.emulator->tmp_register_8[1]);

    TEST_ASSERT_EQUAL(sizeof(CLRT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_inca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INCA_ROM, sizeof(INCA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL + 1, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(INCA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_incb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INCB_ROM, sizeof(INCB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL + 1, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(INCB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_inct(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, INCT_ROM, sizeof(INCT_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(tmp.low + 1, bundle.emulator->tmp_register_8[0]);

    TEST_ASSERT_EQUAL(sizeof(INCT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_deca(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DECA_ROM, sizeof(DECA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL - 1, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(DECA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_decb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DECB_ROM, sizeof(DECB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL - 1, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(DECB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_dect(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, DECT_ROM, sizeof(DECT_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(tmp.low - 1, bundle.emulator->tmp_register_8[0]);

    TEST_ASSERT_EQUAL(sizeof(DECT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_negaa);
    RUN_TEST(test_negab);
    RUN_TEST(test_negba);
    RUN_TEST(test_negbb);
    RUN_TEST(test_negmema);
    RUN_TEST(test_negmemb);
    RUN_TEST(test_negmezpa);
    RUN_TEST(test_negmezpb);
    RUN_TEST(test_negstca);
    RUN_TEST(test_negstcb);

    RUN_TEST(test_adda);
    RUN_TEST(test_addb);
    RUN_TEST(test_addmem);
    RUN_TEST(test_addmezp);
    RUN_TEST(test_addstc);

    RUN_TEST(test_subaab);
    RUN_TEST(test_subaba);
    RUN_TEST(test_subbab);
    RUN_TEST(test_subbba);
    RUN_TEST(test_submemab);
    RUN_TEST(test_submemba);
    RUN_TEST(test_submezpab);
    RUN_TEST(test_submezpba);
    RUN_TEST(test_substcab);
    RUN_TEST(test_substcba);

    RUN_TEST(test_div2aa);
    RUN_TEST(test_div2ab);
    RUN_TEST(test_div2ba);
    RUN_TEST(test_div2bb);
    RUN_TEST(test_div2mema);
    RUN_TEST(test_div2memb);
    RUN_TEST(test_div2mezpa);
    RUN_TEST(test_div2mezpb);
    RUN_TEST(test_div2stca);
    RUN_TEST(test_div2stcb);

    RUN_TEST(test_invaa);
    RUN_TEST(test_invab);
    RUN_TEST(test_invba);
    RUN_TEST(test_invbb);
    RUN_TEST(test_invmema);
    RUN_TEST(test_invmemb);
    RUN_TEST(test_invmezpa);
    RUN_TEST(test_invmezpb);
    RUN_TEST(test_invstca);
    RUN_TEST(test_invstcb);

    RUN_TEST(test_oraa);
    RUN_TEST(test_orab);
    RUN_TEST(test_ormem);
    RUN_TEST(test_ormezp);
    RUN_TEST(test_orstc);

    RUN_TEST(test_anda);
    RUN_TEST(test_andb);
    RUN_TEST(test_andmem);
    RUN_TEST(test_andmezp);
    RUN_TEST(test_andstc);

    RUN_TEST(test_xora);
    RUN_TEST(test_xorb);
    RUN_TEST(test_xormem);
    RUN_TEST(test_xormezp);
    RUN_TEST(test_xorstc);

    RUN_TEST(test_shraa);
    RUN_TEST(test_shrab);
    RUN_TEST(test_shrba);
    RUN_TEST(test_shrbb);
    RUN_TEST(test_shrmema);
    RUN_TEST(test_shrmemb);
    RUN_TEST(test_shrmezpa);
    RUN_TEST(test_shrmezpb);
    RUN_TEST(test_shrstca);
    RUN_TEST(test_shrstcb);

    RUN_TEST(test_shlaa);
    RUN_TEST(test_shlab);
    RUN_TEST(test_shlba);
    RUN_TEST(test_shlbb);
    RUN_TEST(test_shlmema);
    RUN_TEST(test_shlmemb);
    RUN_TEST(test_shlmezpa);
    RUN_TEST(test_shlmezpb);
    RUN_TEST(test_shlstca);
    RUN_TEST(test_shlstcb);

    RUN_TEST(test_cmpab);
    RUN_TEST(test_cmpba);
    RUN_TEST(test_cmpthtl);
    RUN_TEST(test_cmptlth);
    RUN_TEST(test_cmptha);
    RUN_TEST(test_cmpath);
    RUN_TEST(test_cmpthb);
    RUN_TEST(test_cmpbth);
    RUN_TEST(test_cmptla);
    RUN_TEST(test_cmpatl);
    RUN_TEST(test_cmptlb);
    RUN_TEST(test_cmpbtl);
    RUN_TEST(test_cmpinta);
    RUN_TEST(test_cmpaint);
    RUN_TEST(test_cmpintb);
    RUN_TEST(test_cmpbint);

    RUN_TEST(test_clra);
    RUN_TEST(test_clrb);
    RUN_TEST(test_clrth);
    RUN_TEST(test_clrtl);
    RUN_TEST(test_clrt);

    RUN_TEST(test_inca);
    RUN_TEST(test_incb);
    RUN_TEST(test_inct);

    RUN_TEST(test_deca);
    RUN_TEST(test_decb);
    RUN_TEST(test_dect);

    return UNITY_END();
}
