#include <stdio.h>
#include <ncurses.h>
#include "emulator.h"


void print_emulator_status(Emulator *emulator) {
    printf("A: signed: %d unsigned: %u\n", emulator->signed_a_register,emulator->a_register);
    printf("B: signed: %d unsigned: %u\n", emulator->signed_b_register,emulator->b_register);
    printf("tmp: %u\n", emulator->tmp_register);
    printf("pc: %u\n", emulator->program_counter);
    printf("sp: %u\n", emulator->stack_pointer);
    printf("flag: S P Z C O - - -\n      ");
    uint8_t a = emulator->flag_register;
    for (int i = 0; i < 8; i++) { printf("%d ", ((a << i) & 0x80) != 0); }
    printf("\nis_halted: %u\n", emulator->is_halted);
    printf("clock_cycles_counter: %u\n", emulator->clock_cycles_counter);
    printf("instruction_counter: %u\n", emulator->instruction_counter);
}

const uint8_t ROM[] = {
        0b00001101, // MOVAIMM 1
        0b00000001,
        0b00001110, // MOVBIMM 67
        0b01000011,
        0b00011010, // ADDA
        0b00001110, // MOVBIMM 1
        0b00000001,
        0b00011010, // ADDA
        0b11111110, // SKIP (in emulator acts as print REG_A)
        0b00000010, // MOVBA
        0b00011010, // ADDA
        0b11111110, // SKIP
        0b00011010, // ADDA
        0b11111110, // SKIP
        0b00011010, // ADDA
        0b11111110, // SKIP
        0b00011101, // SUBABA
        0b11111110, // SKIP
        0b00011101, // SUBABA
        0b11111110, // SKIP
        0b00011101, // SUBABA
        0b11111110, // SKIP (in emulator acts as print REG_A)
        0b11111111, // HALT
};

int main(void) {
    Emulator emulator;
    Config config;
    load_config_temp(&config);
    print_config(&config);
    init_emulator(&emulator);
    for (uint32_t i = 0; i < sizeof ROM; ++i) {
        emulator.memory[i] = ROM[i];
    }
    emulator.a_register=11;
    while (emulator.is_halted == 0&&emulator.program_counter<sizeof ROM) {
       run_next_emulator_instruction(&emulator,&config);
    }
    print_emulator_status(&emulator);
    cleanup_config(&config);
    return 0;
}
