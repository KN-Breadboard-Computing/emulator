#include "emulator.h"
#include <stdio.h>

void init_emulator(Emulator *emulator) {
    emulator->a_register = 0;
    emulator->b_register = 0;
    emulator->tmp_register = 0;
    emulator->stack_pointer = 0;
    emulator->program_counter = 0;
    emulator->flag_register = 0;
    emulator->is_halted = 0;
    emulator->clock_cycles_counter = 0;
    emulator->instruction_counter = 0;

    for (unsigned int i = 0; i < sizeof emulator->memory; i++) {
        emulator->memory[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof emulator->stack; i++) {
        emulator->stack[i] = 0;
    }
}

//TODO: decode instruction depending on config file
Instruction decode_instruction(uint8_t instruction_bytecode) {

    switch (instruction_bytecode) {
        case 0b00000000:
            return NOP;
        case 0b00000001:
            return MOVAB;
        case 0b00000010:
            return MOVBA;
        case 0b00001101:
            return MOVAIMM;
        case 0b00001110:
            return MOVBIMM;
        case 0b11111111:
            return HALT;
        case 0b00011010:
            return ADDA;
        case 0b11111110:
            return SKIP;
        default:
            return NOP;

    }
}

//TODO: add overflow flag
void calculate_flags(Emulator *emulator, uint8_t before, uint8_t after) {
    //sign flag
    if (after >> 7 == 1)
        emulator->flag_register |= 0b10000000;
    //parity flag
    if (after % 2 == 1)
        emulator->flag_register |= 0b01000000;
    //zero flag
    if (after == 0)
        emulator->flag_register |= 0b00100000;
    //carry flag
    if (after < before)
        emulator->flag_register |= 0b00010000;
}

int run_instruction(Emulator *emulator, Instruction instruction) {
    emulator->instruction_counter++;
    switch (instruction) {
        case MOVAB:
            emulator->clock_cycles_counter += 3;
            emulator->b_register = emulator->a_register;
            break;
        case MOVBA:
            emulator->clock_cycles_counter += 3;
            emulator->a_register = emulator->b_register;
            break;
        case NOP:
            emulator->clock_cycles_counter += 3;
            break;
        case MOVAIMM:
            emulator->clock_cycles_counter += 4;
            emulator->a_register = emulator->memory[emulator->program_counter];
            emulator->program_counter++;
            break;
        case MOVBIMM:
            emulator->clock_cycles_counter += 4;
            emulator->b_register = emulator->memory[emulator->program_counter];
            emulator->program_counter++;
            break;
        case HALT:
            emulator->clock_cycles_counter += 2;
            emulator->is_halted = 1;
            break;
        case ADDA:
            emulator->flag_register = 0;
            emulator->clock_cycles_counter += 4;
            uint8_t temp = emulator->a_register;
            emulator->a_register += emulator->b_register;
            calculate_flags(emulator, temp, emulator->a_register);
            break;
            //useless in physical computer used for debug printing Register A
        case SKIP:
            emulator->clock_cycles_counter += 2;
            printf("SKIP: %d\n", emulator->a_register);
            break;
        default:
            emulator->clock_cycles_counter += 1;
            printf("not implemented yet :<<\n");
            return 1;
    }
    return 0;
}

/* 0 - OK
 * 1 - ERR
 * 2 - HLT
 */
int run_next_emulator_instruction(Emulator *emulator) {
    Instruction instruction = decode_instruction(emulator->memory[emulator->program_counter]);
    if (emulator->is_halted)
        return 2;
    emulator->program_counter++;
    return run_instruction(emulator, instruction);
}

