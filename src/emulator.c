#include "emulator.h"
#include "config.h"
#include <stdbool.h>
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

void calculate_flags(Emulator *emulator, uint8_t before, uint8_t after, bool is_sub) {
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

MemPtr process_operand(Emulator *emulator, const char *operand) {
    MemPtr ret;
    ret.mem16 = NULL;
    ret.mem8 = NULL;
    ret.is16 = 0;
    if (!strcmp(operand, "A")) {
        ret.is16 = false;
        ret.mem8 = &emulator->a_register;
    } else if (!strcmp(operand, "B")) {
        ret.is16 = false;
        ret.mem8 = &emulator->b_register;
    } else if (!strcmp(operand, "CONST")) {
        ret.is16 = false;
        ret.mem8 = fetch_next_byte(emulator);
    } else if ((!strcmp(operand, "MEM8")) || (!strcmp(operand, "MEMZP"))) {
        emulator->tmp_register_8[0] = *fetch_next_byte(emulator);
        ret.is16 = true;
        ret.mem16 = &emulator->tmp_register_16;
    } else if ((!strcmp(operand, "MEM16")) || (!strcmp(operand, "MEM"))) {
        emulator->tmp_register_8[0] = *fetch_next_byte(emulator);
        emulator->tmp_register_8[1] = *fetch_next_byte(emulator);
        ret.is16 = true;
        ret.mem16 = &emulator->tmp_register_16;
    } else if (!strcmp(operand, "F")) {
        ret.is16 = false;
        ret.mem8 = &emulator->flag_register;
    } else if (!strcmp(operand, "TL")) {
        ret.is16 = false;
        ret.mem8 = &emulator->tmp_register_8[0];
    } else if (!strcmp(operand, "TH")) {
        ret.is16 = false;
        ret.mem8 = &emulator->tmp_register_8[1];
    } else if (!strcmp(operand, "STC")) {
        ret.is16 = false;
        ret.mem8 = &emulator->stack[emulator->stack_pointer];
        emulator->stack_pointer++;
    } else if (!strcmp(operand, "T")) {
        ret.is16 = true;
        ret.mem16 = &emulator->tmp_register_16;
    }
    return ret;
}

// 0 - OK
// 1 - WRONG NUMBER OF OPERANDS
// 2 - INVALID OPERANDS
int handle_mov(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr des_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (des_op.is16 || src_op.is16 || src_op.mem16 == NULL)
        return 2;
    *des_op.mem8 = *src_op.mem8;
    return 0;
}

int handle_inc(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr des_op = process_operand(emulator, instruction.operands[0]);
    if (des_op.mem16 == NULL)
        return 2;
    if (des_op.is16)
        (*des_op.mem16)++;
    else
        (*des_op.mem8)++;
    return 0;
}

int handle_movat(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr src_op = process_operand(emulator, instruction.operands[0]);
    MemPtr adr_op = process_operand(emulator, instruction.operands[1]);
    if (src_op.is16 || src_op.mem16 == NULL)
        return 2;
    if (adr_op.is16)
        emulator->tmp_register_16 = *adr_op.mem16;
    else
        emulator->tmp_register_8[0] = *adr_op.mem8;
    uint8_t *destination = &emulator->memory[emulator->tmp_register_16];
    *destination = *src_op.mem8;
    return 0;
}

int handle_dec(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr des_op = process_operand(emulator, instruction.operands[0]);
    if (des_op.mem16 == NULL)
        return 2;
    if (des_op.is16)
        (*des_op.mem16)--;
    else
        (*des_op.mem8)--;
    return 0;
}

int handle_neg(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (adr_op.mem16 == NULL || src_op.is16 || src_op.mem16 == NULL)
        return 2;
    uint8_t before = *src_op.mem8;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = (*src_op.mem8 ^ 255) + 1;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_inv(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (adr_op.mem16 == NULL || src_op.is16 || src_op.mem16 == NULL)
        return 2;
    uint8_t before = *src_op.mem8;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = *src_op.mem8 ^ 255;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_or(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;
    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register | emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

int handle_and(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;
    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register & emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

int handle_xor(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;
    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register ^ emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

int handle_shl(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (adr_op.mem16 == NULL || src_op.is16 || src_op.mem16 == NULL)
        return 2;
    uint8_t before = *src_op.mem8;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = (uint8_t)(*src_op.mem8 << 1);
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_div(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (adr_op.mem16 == NULL || src_op.is16 || src_op.mem16 == NULL)
        return 2;
    uint8_t before = *src_op.mem8;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = *src_op.mem8 >> 1;
    if (*src_op.mem8 >> 7 == 1)
        *destination |= 0b10000000;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_shr(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (adr_op.mem16 == NULL || src_op.is16 || src_op.mem16 == NULL)
        return 2;
    uint8_t before = *src_op.mem8;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = *src_op.mem8 >> 1;
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

int handle_add(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;
    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register + emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

int handle_sub(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 3)
        return 1;
    MemPtr minuend = process_operand(emulator, instruction.operands[1]);
    MemPtr subtrahend = process_operand(emulator, instruction.operands[2]);
    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (minuend.mem16 == NULL || subtrahend.mem16 == NULL || adr_op.mem16 == NULL || subtrahend.is16 || minuend.is16)
        return 2;
    uint8_t before = *minuend.mem8;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else
        destination = adr_op.mem8;
    *destination = *minuend.mem8 - *subtrahend.mem8;
    calculate_flags(emulator, before, *destination, 1);

    return 0;
}

int handle_cmp(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;
    MemPtr minuend = process_operand(emulator, instruction.operands[0]);
    MemPtr subtrahend = process_operand(emulator, instruction.operands[1]);
    if (minuend.mem16 == NULL || subtrahend.mem16 == NULL || subtrahend.is16 || minuend.is16)
        return 2;
    uint8_t temp = *minuend.mem8 - *subtrahend.mem8;
    calculate_flags(emulator, *minuend.mem8, temp, 1);
    return 0;
}

int handle_clr(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.mem16 == NULL || target.is16)
        return 2;
    *target.mem8 = 0;

    return 0;
}

int handle_pop(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.mem16 == NULL)
        return 2;
    uint8_t *destination;
    if (target.is16)
        destination = &emulator->memory[*target.mem16];
    else
        destination = target.mem8;
    emulator->stack_pointer--;
    *destination = emulator->stack[emulator->stack_pointer];
    return 0;
}

int handle_push(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;
    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.mem16 == NULL)
        return 2;
    uint8_t *destination;
    if (target.is16)
        destination = &emulator->memory[*target.mem16];
    else
        destination = target.mem8;
    emulator->stack[emulator->stack_pointer] = *destination;
    emulator->stack_pointer++;
    return 0;
}

// 0 - OK
// 1 - WRONG NUMBER OF OPERANDS
// 2 - INVALID OPERANDS
// 3 - NOT IMPLEMENTED YET
int run_instruction(Emulator *emulator, Instruction instruction) {
    emulator->instruction_counter++;
    emulator->clock_cycles_counter += instruction.cycle_count;
#ifdef DEBUG
    printf("\nrunning instruction: %s\n", instruction.mnemonic);
    printf("operands: ");
    for (unsigned i = 0; i < instruction.num_operands; i++) {
        printf("%s ", instruction.operands[i]);
    }
    printf("clock cycles: %u\n", instruction.cycle_count);
#endif
    int ret = 0;
    if (!strcmp(instruction.mnemonic, "MOV")) {
        ret = handle_mov(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "NOP")) {
        ;
    } else if (!strcmp(instruction.mnemonic, "HLT")) {
        ret = emulator->is_halted = 1;
    } else if (!strcmp(instruction.mnemonic, "MOVAT")) {
        ret = handle_movat(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "ADD")) {
        ret = handle_add(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SUB")) {
        ret = handle_sub(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "INC")) {
        ret = handle_inc(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "DEC")) {
        ret = handle_dec(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "NEG")) {
        ret = handle_neg(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SHL")) {
        ret = handle_shl(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "DIV")) {
        ret = handle_div(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SHR")) {
        ret = handle_shr(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "OR")) {
        ret = handle_or(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "AND")) {
        ret = handle_and(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "XOR")) {
        ret = handle_xor(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "INV")) {
        ret = handle_inv(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "CMP")) {
        ret = handle_cmp(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "CLR")) {
        ret = handle_clr(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "PUSH")) {
        ret = handle_push(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "POP")) {
        ret = handle_pop(emulator, instruction);
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

        return 3;
    }
    return ret;
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
