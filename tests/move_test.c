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

const uint8_t MOVE_AB_ROM[] = {
    0b00000001, // MOVAB
    0b11111010  // HALT
};

const uint8_t MOVE_ATH_ROM[] = {
    0b00000010, // MOVATH
    0b11111010  // HALT
};

const uint8_t MOVE_ATL_ROM[] = {
    0b00000011, // MOVATL
    0b11111010  // HALT
};

const uint8_t MOVE_AF_ROM[] = {
    0b00000100, // MOVAF
    0b11111010  // HALT
};

const uint8_t MOVE_AINT_ROM[] = {
    0b00000101, // MOVAINT
    0b11111010  // HALT
};

const uint8_t MOVE_BA_ROM[] = {
    0b00000110, // MOVBA
    0b11111010  // HALT
};

const uint8_t MOVE_BTH_ROM[] = {
    0b00000111, // MOVBTH
    0b11111010  // HALT
};

const uint8_t MOVE_BTL_ROM[] = {
    0b00001000, // MOVBTL
    0b11111010  // HALT
};

const uint8_t MOVE_BF_ROM[] = {
    0b00001001, // MOVBF
    0b11111010  // HALT
};

const uint8_t MOVE_BINT_ROM[] = {
    0b00001010, // MOVBINT
    0b11111010  // HALT
};

const uint8_t MOVE_THA_ROM[] = {
    0b00001011, // MOVTHA
    0b11111010  // HALT
};

const uint8_t MOVE_THB_ROM[] = {
    0b00001100, // MOVTHB
    0b11111010  // HALT
};

const uint8_t MOVE_THTL_ROM[] = {
    0b00001101, // MOVTHTL
    0b11111010  // HALT
};

const uint8_t MOVE_THF_ROM[] = {
    0b00001110, // MOVTHF
    0b11111010  // HALT
};

const uint8_t MOVE_THINT_ROM[] = {
    0b00001111, // MOVTHINT
    0b11111010  // HALT
};

const uint8_t MOVE_TLA_ROM[] = {
    0b00010000, // MOVTLA
    0b11111010  // HALT
};

const uint8_t MOVE_TLB_ROM[] = {
    0b00010001, // MOVTLB
    0b11111010  // HALT
};

const uint8_t MOVE_TLTH_ROM[] = {
    0b00010010, // MOVTLTH
    0b11111010  // HALT
};

const uint8_t MOVE_TLF_ROM[] = {
    0b00010011, // MOVTLF
    0b11111010  // HALT
};

const uint8_t MOVE_TLINT_ROM[] = {
    0b00010100, // MOVTLINT
    0b11111010  // HALT
};

const uint8_t MOVE_AIMM_ROM[] = {
    0b00010101, // MOVAIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_BIMM_ROM[] = {
    0b00010110, // MOVBIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_THIMM_ROM[] = {
    0b00010111, // MOVTHIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_TLIMM_ROM[] = {
    0b00011000, // MOVTIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_AABS_ROM[] = {
    0b00011001, // MOVAABS
    ADDR_HIGH_VAL, ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_AABSZP_ROM[] = {
    0b00011010, // MOVAABSZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_BABS_ROM[] = {
    0b00011011, // MOVBABS
    ADDR_HIGH_VAL, ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_BABSZP_ROM[] = {
    0b00011100, // MOVBABSZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVE_AATT_ROM[] = {
    0b00011101, // MOVAATT
    0b11111010  // HALT
};

const uint8_t MOVE_BATT_ROM[] = {
    0b00011110, // MOVBATT
    0b11111010  // HALT
};

const uint8_t MOVE_AATB_ROM[] = {
    0b00011111, // MOVAATB
    0b11111010  // HALT
};

const uint8_t MOVE_AATTH_ROM[] = {
    0b00100000, // MOVAATTH
    0b11111010  // HALT
};

const uint8_t MOVE_AATTL_ROM[] = {
    0b00100001, // MOVAATTL
    0b11111010  // HALT
};

const uint8_t MOVE_BATA_ROM[] = {
    0b00100010, // MOVBATA
    0b11111010  // HALT
};

const uint8_t MOVE_BATTH_ROM[] = {
    0b00100011, // MOVBATTH
    0b11111010  // HALT
};

const uint8_t MOVE_BATTL_ROM[] = {
    0b00100100, // MOVBATTL
    0b11111010  // HALT
};

const uint8_t MOVE_THATA_ROM[] = {
    0b00100101, // MOVTHATA
    0b11111010  // HALT
};

const uint8_t MOVE_THATB_ROM[] = {
    0b00100110, // MOVTHATB
    0b11111010  // HALT
};

const uint8_t MOVE_THATTL_ROM[] = {
    0b00100111, // MOVTHATTL
    0b11111010  // HALT
};

const uint8_t MOVE_TLATA_ROM[] = {
    0b00101000, // MOVTLATA
    0b11111010  // HALT
};

const uint8_t MOVE_TLATB_ROM[] = {
    0b00101001, // MOVTLATB
    0b11111010  // HALT
};

const uint8_t MOVE_TLATTH_ROM[] = {
    0b00101010, // MOVTLATTH
    0b11111010  // HALT
};

const uint8_t MOVEAT_ABSA_ROM[] = {
    0b00101011, // MOVEAT_ABSA
    ADDR_HIGH_VAL, ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSAZP_ROM[] = {
    0b00101100, // MOVEAT_ABSAZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSB_ROM[] = {
    0b00101101, // MOVEAT_ABSB
    ADDR_HIGH_VAL, ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSBZP_ROM[] = {
    0b00101110, // MOVEAT_ABSBZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSINT_ROM[] = {
    0b00101111, // MOVEAT_ABSINT
    ADDR_HIGH_VAL, ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSINTZP_ROM[] = {
    0b00110000, // MOVEAT_ABSINTZP
    ADDR_LOW_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSIMM_ROM[] = {
    0b00110001, // MOVEAT_ABSIMM
    ADDR_HIGH_VAL, ADDR_LOW_VAL, IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_ABSIMMZP_ROM[] = {
    0b00110010, // MOVEAT_ABSIMMZP
    ADDR_LOW_VAL, IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_TA_ROM[] = {
    0b00110011, // MOVEAT_TA
    0b11111010  // HALT
};

const uint8_t MOVEAT_TB_ROM[] = {
    0b00110100, // MOVEAT_TB
    0b11111010  // HALT
};

const uint8_t MOVEAT_TINT_ROM[] = {
    0b00110101, // MOVEAT_TINT
    0b11111010  // HALT
};

const uint8_t MOVEAT_AB_ROM[] = {
    0b00110110, // MOVEAT_AB
    0b11111010  // HALT
};

const uint8_t MOVEAT_ATH_ROM[] = {
    0b00110111, // MOVEAT_ATH
    0b11111010  // HALT
};

const uint8_t MOVEAT_ATL_ROM[] = {
    0b00111000, // MOVEAT_ATL
    0b11111010  // HALT
};

const uint8_t MOVEAT_BA_ROM[] = {
    0b00111001, // MOVEAT_BA
    0b11111010  // HALT
};

const uint8_t MOVEAT_BTH_ROM[] = {
    0b00111010, // MOVEAT_BTH
    0b11111010  // HALT
};

const uint8_t MOVEAT_BTL_ROM[] = {
    0b00111011, // MOVEAT_BTL
    0b11111010  // HALT
};

const uint8_t MOVEAT_THA_ROM[] = {
    0b00111100, // MOVEAT_THA
    0b11111010  // HALT
};

const uint8_t MOVEAT_THB_ROM[] = {
    0b00111101, // MOVEAT_THB
    0b11111010  // HALT
};

const uint8_t MOVEAT_THTL_ROM[] = {
    0b00111110, // MOVEAT_THTL
    0b11111010  // HALT
};

const uint8_t MOVEAT_TLA_ROM[] = {
    0b00111111, // MOVEAT_TLA
    0b11111010  // HALT
};

const uint8_t MOVEAT_TLB_ROM[] = {
    0b01000000, // MOVEAT_TLB
    0b11111010  // HALT
};

const uint8_t MOVEAT_TLTH_ROM[] = {
    0b01000001, // MOVEAT_TLTH
    0b11111010  // HALT
};

const uint8_t MOVEAT_TIMM_ROM[] = {
    0b01000010, // MOVEAT_TIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_AIMM_ROM[] = {
    0b01000011, // MOVEAT_AIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_BIMM_ROM[] = {
    0b01000100, // MOVEAT_BIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_THIMM_ROM[] = {
    0b01000101, // MOVEAT_THIMM
    IMM_VAL,
    0b11111010 // HALT
};

const uint8_t MOVEAT_TLIMM_ROM[] = {
    0b01000110, // MOVEAT_TLIMM
    IMM_VAL,
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

void test_move_ab(void);
void test_move_ath(void);
void test_move_atl(void);
void test_move_af(void);
void test_move_aint(void);

void test_move_ba(void);
void test_move_bth(void);
void test_move_btl(void);
void test_move_bf(void);
void test_move_bint(void);

void test_move_tha(void);
void test_move_thb(void);
void test_move_thtl(void);
void test_move_thf(void);
void test_move_thint(void);

void test_move_tla(void);
void test_move_tlb(void);
void test_move_tlth(void);
void test_move_tlf(void);
void test_move_tlint(void);

void test_move_aimm(void);
void test_move_bimm(void);
void test_move_thimm(void);
void test_move_tlimm(void);

void test_move_aabs(void);
void test_move_aabszp(void);
void test_move_babs(void);
void test_move_babszp(void);

void test_move_aatt(void);
void test_move_batt(void);
void test_move_aatb(void);
void test_move_aatth(void);
void test_move_aattl(void);
void test_move_bata(void);
void test_move_batth(void);
void test_move_battl(void);
void test_move_thata(void);
void test_move_thatb(void);
void test_move_thattl(void);
void test_move_tlata(void);
void test_move_tlatb(void);
void test_move_tlatth(void);

void test_moveat_absa(void);
void test_moveat_absazp(void);
void test_moveat_absb(void);
void test_moveat_absbzp(void);
void test_moveat_absint(void);
void test_moveat_absintzp(void);
void test_moveat_absimm(void);
void test_moveat_absimmzp(void);

void test_moveat_ta(void);
void test_moveat_tb(void);
void test_moveat_tint(void);
void test_moveat_ab(void);
void test_moveat_ath(void);
void test_moveat_atl(void);
void test_moveat_ba(void);
void test_moveat_bth(void);
void test_moveat_btl(void);
void test_moveat_tha(void);
void test_moveat_thb(void);
void test_moveat_thtl(void);
void test_moveat_tla(void);
void test_moveat_tlb(void);
void test_moveat_tlth(void);

void test_moveat_timm(void);
void test_moveat_aimm(void);
void test_moveat_bimm(void);
void test_moveat_thimm(void);
void test_moveat_tlimm(void);

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

void test_move_ab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AB_ROM, sizeof(MOVE_AB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_ath(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_ATH_ROM, sizeof(MOVE_ATH_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_ATH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_atl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_ATL_ROM, sizeof(MOVE_ATL_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.low, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_ATL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_af(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AF_ROM, sizeof(MOVE_AF_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->flag_register = INIT_FLAG_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AF_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AINT_ROM, sizeof(MOVE_AINT_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AINT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_ba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BA_ROM, sizeof(MOVE_BA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_bth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BTH_ROM, sizeof(MOVE_BTH_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_btl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BTL_ROM, sizeof(MOVE_BTL_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.low, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_bf(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BF_ROM, sizeof(MOVE_BF_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->flag_register = INIT_FLAG_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BF_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_bint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BINT_ROM, sizeof(MOVE_BINT_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BINT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tha(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THA_ROM, sizeof(MOVE_THA_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THB_ROM, sizeof(MOVE_THB_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thtl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THTL_ROM, sizeof(MOVE_THTL_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = tmp.low;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thf(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THF_ROM, sizeof(MOVE_THF_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->flag_register = INIT_FLAG_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = INIT_FLAG_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THF_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THINT_ROM, sizeof(MOVE_THINT_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THINT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tla(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLA_ROM, sizeof(MOVE_TLA_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLB_ROM, sizeof(MOVE_TLB_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLTH_ROM, sizeof(MOVE_TLTH_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = tmp.high;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlf(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLF_ROM, sizeof(MOVE_TLF_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->flag_register = INIT_FLAG_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = INIT_FLAG_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_FLAG_REG_VAL, bundle.emulator->flag_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLF_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLINT_ROM, sizeof(MOVE_TLINT_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLINT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AIMM_ROM, sizeof(MOVE_AIMM_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_bimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BIMM_ROM, sizeof(MOVE_BIMM_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THIMM_ROM, sizeof(MOVE_THIMM_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = IMM_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLIMM_ROM, sizeof(MOVE_TLIMM_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);

    run_emulator(&bundle);

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = IMM_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aabs(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AABS_ROM, sizeof(MOVE_AABS_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->memory[ADDR_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AABS_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aabszp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AABSZP_ROM, sizeof(MOVE_AABSZP_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->memory[ADDR_ZP_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AABSZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_babs(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BABS_ROM, sizeof(MOVE_BABS_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->memory[ADDR_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BABS_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_babszp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BABSZP_ROM, sizeof(MOVE_BABSZP_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->memory[ADDR_ZP_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BABSZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aatt(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AATT_ROM, sizeof(MOVE_AATT_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TMP_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TMP_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TMP_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AATT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_batt(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BATT_ROM, sizeof(MOVE_BATT_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TMP_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TMP_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TMP_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BATT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aatb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AATB_ROM, sizeof(MOVE_AATB_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_B_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_B_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_B_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AATB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aatth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AATTH_ROM, sizeof(MOVE_AATTH_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TH_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TH_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TH_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AATTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_aattl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_AATTL_ROM, sizeof(MOVE_AATTL_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TL_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TL_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TL_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_AATTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_bata(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BATA_ROM, sizeof(MOVE_BATA_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_A_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_A_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_A_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BATA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_batth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BATTH_ROM, sizeof(MOVE_BATTH_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TH_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TH_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TH_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BATTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_battl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_BATTL_ROM, sizeof(MOVE_BATTL_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TL_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TL_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TL_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_BATTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thata(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THATA_ROM, sizeof(MOVE_THATA_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->memory[INIT_A_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_A_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_A_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THATA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thatb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THATB_ROM, sizeof(MOVE_THATB_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->memory[INIT_B_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_B_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_B_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THATB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_thattl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_THATTL_ROM, sizeof(MOVE_THATTL_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TL_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TL_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TL_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_THATTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlata(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLATA_ROM, sizeof(MOVE_TLATA_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;
    bundle.emulator->memory[INIT_A_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_A_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_A_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLATA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlatb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLATB_ROM, sizeof(MOVE_TLATB_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;
    bundle.emulator->memory[INIT_B_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_B_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_B_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLATB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_move_tlatth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVE_TLATTH_ROM, sizeof(MOVE_TLATTH_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;
    bundle.emulator->memory[INIT_TH_REG_VAL] = IMM_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TH_REG_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->tmp_register_8[0]);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->tmp_register_8[1]);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TH_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVE_TLATTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absa(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSA_ROM, sizeof(MOVEAT_ABSA_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absazp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSAZP_ROM, sizeof(MOVEAT_ABSAZP_ROM));

    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSAZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSB_ROM, sizeof(MOVEAT_ABSB_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absbzp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSBZP_ROM, sizeof(MOVEAT_ABSBZP_ROM));

    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSBZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSINT_ROM, sizeof(MOVEAT_ABSINT_ROM));

    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSINT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absintzp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSINTZP_ROM, sizeof(MOVEAT_ABSINTZP_ROM));

    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSINTZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSIMM_ROM, sizeof(MOVEAT_ABSIMM_ROM));

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_absimmzp(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ABSIMMZP_ROM, sizeof(MOVEAT_ABSIMMZP_ROM));

    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ABSIMMZP_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_ta(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TA_ROM, sizeof(MOVEAT_TA_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->memory[ADDR_VAL]);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TB_ROM, sizeof(MOVEAT_TB_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->memory[ADDR_VAL]);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tint(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TINT_ROM, sizeof(MOVEAT_TINT_ROM));

    bundle.emulator->tmp_register_16 = ADDR_VAL;
    bundle.emulator->interrupt_data = INIT_INT_DATA_VAL;

    TEST_ASSERT_EQUAL(ADDR_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->memory[ADDR_VAL]);
    TEST_ASSERT_EQUAL(INIT_INT_DATA_VAL, bundle.emulator->interrupt_data);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TINT_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_ab(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_AB_ROM, sizeof(MOVEAT_AB_ROM));

    bundle.emulator->a_register = ADDR_LOW_VAL;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_AB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_ath(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ATH_ROM, sizeof(MOVEAT_ATH_ROM));

    bundle.emulator->a_register = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ATH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_atl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_ATL_ROM, sizeof(MOVEAT_ATL_ROM));

    bundle.emulator->a_register = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_ATL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_ba(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_BA_ROM, sizeof(MOVEAT_BA_ROM));

    bundle.emulator->b_register = ADDR_LOW_VAL;
    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_BA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_bth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_BTH_ROM, sizeof(MOVEAT_BTH_ROM));

    bundle.emulator->b_register = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TH_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_BTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_btl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_BTL_ROM, sizeof(MOVEAT_BTL_ROM));

    bundle.emulator->b_register = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(INIT_TL_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_BTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tha(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_THA_ROM, sizeof(MOVEAT_THA_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;
    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_THA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_thb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_THB_ROM, sizeof(MOVEAT_THB_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_THB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_thtl(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_THTL_ROM, sizeof(MOVEAT_THTL_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(tmp.low, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_THTL_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tla(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TLA_ROM, sizeof(MOVEAT_TLA_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;
    bundle.emulator->a_register = INIT_A_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);
    TEST_ASSERT_EQUAL(INIT_A_REG_VAL, bundle.emulator->a_register);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TLA_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tlb(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TLB_ROM, sizeof(MOVEAT_TLB_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;
    bundle.emulator->b_register = INIT_B_REG_VAL;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);
    TEST_ASSERT_EQUAL(INIT_B_REG_VAL, bundle.emulator->b_register);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TLB_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tlth(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TLTH_ROM, sizeof(MOVEAT_TLTH_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(tmp.high, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TLTH_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_timm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TIMM_ROM, sizeof(MOVEAT_TIMM_ROM));

    bundle.emulator->tmp_register_16 = INIT_TMP_REG_VAL;

    TEST_ASSERT_EQUAL(INIT_TMP_REG_VAL, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[INIT_TMP_REG_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_aimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_AIMM_ROM, sizeof(MOVEAT_AIMM_ROM));

    bundle.emulator->a_register = ADDR_LOW_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->a_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_AIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_bimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_BIMM_ROM, sizeof(MOVEAT_BIMM_ROM));

    bundle.emulator->b_register = ADDR_LOW_VAL;

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(ADDR_LOW_VAL, bundle.emulator->b_register);
    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_BIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_tlimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_TLIMM_ROM, sizeof(MOVEAT_TLIMM_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.low = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_TLIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

void test_moveat_thimm(void) {
    BundlePtr bundle;
    initialize_bundle(&bundle, MOVEAT_THIMM_ROM, sizeof(MOVEAT_THIMM_ROM));

    u16_u8u8 tmp;
    tmp.u16 = INIT_TMP_REG_VAL;
    tmp.high = ADDR_LOW_VAL;
    bundle.emulator->tmp_register_16 = tmp.u16;

    TEST_ASSERT_EQUAL(tmp.u16, bundle.emulator->tmp_register_16);
    TEST_ASSERT_EQUAL(0, bundle.emulator->memory[ADDR_ZP_VAL]);

    run_emulator(&bundle);

    TEST_ASSERT_EQUAL(IMM_VAL, bundle.emulator->memory[ADDR_ZP_VAL]);

    TEST_ASSERT_EQUAL(sizeof(MOVEAT_THIMM_ROM), bundle.emulator->program_counter);

    clear_emulator(&bundle);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_move_ab);
    RUN_TEST(test_move_ath);
    RUN_TEST(test_move_atl);
    RUN_TEST(test_move_af);
    RUN_TEST(test_move_aint);

    RUN_TEST(test_move_ba);
    RUN_TEST(test_move_bth);
    RUN_TEST(test_move_btl);
    RUN_TEST(test_move_bf);
    RUN_TEST(test_move_bint);

    RUN_TEST(test_move_tha);
    RUN_TEST(test_move_thb);
    RUN_TEST(test_move_thtl);
    RUN_TEST(test_move_thf);
    RUN_TEST(test_move_thint);

    RUN_TEST(test_move_tla);
    RUN_TEST(test_move_tlb);
    RUN_TEST(test_move_tlth);
    RUN_TEST(test_move_tlf);
    RUN_TEST(test_move_tlint);

    RUN_TEST(test_move_aimm);
    RUN_TEST(test_move_bimm);
    RUN_TEST(test_move_thimm);
    RUN_TEST(test_move_tlimm);

    RUN_TEST(test_move_aabs);
    RUN_TEST(test_move_aabszp);
    RUN_TEST(test_move_babs);
    RUN_TEST(test_move_babszp);

    RUN_TEST(test_move_aatt);
    RUN_TEST(test_move_batt);
    RUN_TEST(test_move_aatb);
    RUN_TEST(test_move_aatth);
    RUN_TEST(test_move_aattl);
    RUN_TEST(test_move_bata);
    RUN_TEST(test_move_batth);
    RUN_TEST(test_move_battl);
    RUN_TEST(test_move_thata);
    RUN_TEST(test_move_thatb);
    RUN_TEST(test_move_thattl);
    RUN_TEST(test_move_tlata);
    RUN_TEST(test_move_tlatb);
    RUN_TEST(test_move_tlatth);

    RUN_TEST(test_moveat_absa);
    RUN_TEST(test_moveat_absazp);
    RUN_TEST(test_moveat_absb);
    RUN_TEST(test_moveat_absbzp);
    RUN_TEST(test_moveat_absint);
    RUN_TEST(test_moveat_absintzp);
    RUN_TEST(test_moveat_absimm);
    RUN_TEST(test_moveat_absimmzp);

    RUN_TEST(test_moveat_ta);
    RUN_TEST(test_moveat_tb);
    RUN_TEST(test_moveat_tint);
    RUN_TEST(test_moveat_ab);
    RUN_TEST(test_moveat_ath);
    RUN_TEST(test_moveat_atl);
    RUN_TEST(test_moveat_ba);
    RUN_TEST(test_moveat_bth);
    RUN_TEST(test_moveat_btl);
    RUN_TEST(test_moveat_tha);
    RUN_TEST(test_moveat_thb);
    RUN_TEST(test_moveat_thtl);
    RUN_TEST(test_moveat_tla);
    RUN_TEST(test_moveat_tlb);
    RUN_TEST(test_moveat_tlth);

    RUN_TEST(test_moveat_timm);
    RUN_TEST(test_moveat_aimm);
    RUN_TEST(test_moveat_bimm);
    RUN_TEST(test_moveat_tlimm);
    RUN_TEST(test_moveat_thimm);

    return UNITY_END();
}
