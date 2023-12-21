#include "emulator.h"
#include "config.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
void (*log_func)(const char *, ...);

void init_emulator(Emulator *emulator) {
    emulator->a_register = 0;
    emulator->b_register = 0;
    emulator->tmp_register_16 = 0;
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

static void calculate_flags(Emulator *emulator, uint8_t before, uint8_t after, bool is_sub) {
    emulator->flag_register = 0;
    // sign flag
    if (after >> 7 == 1)
        emulator->flag_register |= 0b10000000;
    // parity flag
    if (after % 2 == 1)
        emulator->flag_register |= 0b01000000;
    // zero flag
    if (after == 0)
        emulator->flag_register |= 0b00100000;
    // carry flag
    if (after < before)
        emulator->flag_register |= 0b00010000;
    // overflow flag
    if ((is_sub == false && (uint8_t)(after + 128) < (uint8_t)(before + 128)) ||
        (is_sub == true && (uint8_t)(after + 128) > (uint8_t)(before + 128)))
        emulator->flag_register |= 0b00001000;
}

uint8_t *fetch_next_byte(Emulator *emulator) { return &emulator->memory[emulator->program_counter++]; }

void *decode_operand(Emulator *emulator, const char *operand) {
    if (!strcmp(operand, "A")) {
        return &emulator->a_register;
    } else if (!strcmp(operand, "B")) {
        return &emulator->b_register;
    } else if (!strcmp(operand, "CONST")) {
        return fetch_next_byte(emulator);
    } else if (!strcmp(operand, "MEM8")) {
        return &emulator->memory[*fetch_next_byte(emulator)];
    } else if (!strcmp(operand, "MEM16")) {
        union address {
            uint16_t a16;
            uint8_t a8[2];
        } address;

        address.a8[0] = *fetch_next_byte(emulator);
        address.a8[1] = *fetch_next_byte(emulator);
        return &emulator->memory[address.a16];
    } else if (!strcmp(operand, "F")) {
        return &emulator->flag_register;
    } else if (!strcmp(operand, "TMP")) {
        return &emulator->tmp_register_16;
    } else if (!strcmp(operand, "TMPL")) {
        return &emulator->tmp_register_8[0];
    } else if (!strcmp(operand, "TMPH")) {
        return &emulator->tmp_register_8[1];
    } else if (!strcmp(operand, "STC")) {
        return &emulator->stack[emulator->stack_pointer++];
    }
    return NULL;
}

// TODO: Increment clock_cycles_counter
// 0 - OK
// 1 - WRONG NUMBER OF OPERANDS
// 2 - INVALID OPERANDS
int handle_mov(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    uint8_t *source = decode_operand(emulator, instruction.operands[1]);

    *destination = *source;

    return 0;
}

int handle_inc(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    *destination++;
    return 0;
}

int handle_dec(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    *destination--;
    return 0;
}

int handle_neg(Emulator *emulator, Instruction instruction) {
    uint8_t *source = decode_operand(emulator, instruction.operands[0]);
    uint8_t *destination = decode_operand(emulator, instruction.operands[1]);
    uint8_t before = *source;
    *destination = (*source ^ 255) + 1;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}
int handle_inv(Emulator *emulator, Instruction instruction) {
    uint8_t *source = decode_operand(emulator, instruction.operands[0]);
    uint8_t *destination = decode_operand(emulator, instruction.operands[1]);
    uint8_t before = *source;
    *destination = *source ^ 255;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}
int handle_or(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    uint8_t before = *destination;

    *destination = emulator->a_register | emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}
int handle_and(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    uint8_t before = *destination;

    *destination = emulator->a_register & emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}
int handle_xor(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    uint8_t before = *destination;

    *destination = emulator->a_register ^ emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}
int handle_shl(Emulator *emulator, Instruction instruction) {
    uint8_t *source = decode_operand(emulator, instruction.operands[0]);
    uint8_t *destination = decode_operand(emulator, instruction.operands[1]);
    uint8_t before = *source;
    *destination = (uint8_t)(*source << 1);
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_div(Emulator *emulator, Instruction instruction) {
    uint8_t source = *(uint8_t *)decode_operand(emulator, instruction.operands[0]);
    uint8_t *destination = decode_operand(emulator, instruction.operands[1]);
    uint8_t before = source;
    *destination = source >> 1;
    if (source >> 7 == 1)
        *destination |= 0b10000000;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_shr(Emulator *emulator, Instruction instruction) {
    uint8_t *source = decode_operand(emulator, instruction.operands[0]);
    uint8_t *destination = decode_operand(emulator, instruction.operands[1]);
    uint8_t before = *source;
    *destination = *source >> 1;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_add(Emulator *emulator, Instruction instruction) {
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    uint8_t before = *destination;

    *destination = emulator->a_register + emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

int handle_sub(Emulator *emulator, Instruction instruction) {

    uint8_t *minuend = decode_operand(emulator, instruction.operands[0]);
    uint8_t *subtrahend = decode_operand(emulator, instruction.operands[1]);
    uint8_t *destination = decode_operand(emulator, instruction.operands[2]);
    uint8_t before = *minuend;

    *destination = *minuend - *subtrahend;
    calculate_flags(emulator, before, *destination, 1);

    return 0;
}

// TODO: Add error handling
int run_instruction(Emulator *emulator, Instruction instruction) {
    emulator->instruction_counter++;
    emulator->clock_cycles_counter += instruction.cycle_count;
#ifdef DEBUG
    printf("\nrunning instruction: %s\n", instruction.mnemonic);
    printf("operands: ");
    for (int i = 0; i < instruction.num_operands; i++) {
        printf("%s ", instruction.operands[i]);
    }
#endif

    if (!strcmp(instruction.mnemonic, "MOV")) {
        handle_mov(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "NOP")) {
        ;
    } else if (!strcmp(instruction.mnemonic, "HLT")) {
        emulator->is_halted = 1;
    } else if (!strcmp(instruction.mnemonic, "ADD")) {
        handle_add(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SUB")) {
        handle_sub(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "INC")) {
        handle_inc(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "DEC")) {
        handle_dec(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "NEG")) {
        handle_neg(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "MUL") || !strcmp(instruction.mnemonic, "SHL")) {
        handle_shl(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "DIV")) {
        handle_div(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SHR")) {
        handle_shr(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "OR")) {
        handle_or(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "AND")) {
        handle_and(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "XOR")) {
        handle_xor(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SKP")) {
        //(*log_func)("(skip) A: signed: %d unsigned: %u\n", emulator->signed_a_register, emulator->a_register);
    } else {
        //(*log_func)("not implemented yet :<<\n");
        for (unsigned i = 0; i < instruction.num_operands; ++i) {
            if (!strcmp(instruction.operands[i], "CONST") || !strcmp(instruction.operands[i], "MEM8"))
                emulator->program_counter++;
            if (!strcmp(instruction.operands[i], "MEM16"))
                emulator->program_counter += 2;
        }

        return 1;
    }
    return 0;
}

/* 0 - OK
 * 1 - ERR
 * 2 - HLT
 */
int run_next_emulator_instruction(Emulator *emulator, Config *config) {
    Instruction instruction = *config->instructions[emulator->memory[emulator->program_counter]];
    if (emulator->is_halted)
        return 2;
    emulator->program_counter++;
    return run_instruction(emulator, instruction);
}
