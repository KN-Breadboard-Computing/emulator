#include "emulator.h"
#include <string.h>

void init_emulator(Emulator *emulator) {
    emulator->a_register = 0;
    emulator->b_register = 0;
    emulator->tmp_register_16 = 0;
    emulator->stack_pointer = 0xFFFF;
    emulator->program_counter = 0;
    emulator->flag_register = 0;
    emulator->interrupt_data = 0;
    emulator->interrupt_signals = 0;
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

static bool check_flags(Emulator *emulator, const char *flag) {
    if (flag[0] == '\0')
        return true;
    bool ret;
    char test;
    if (flag[0] == 'N') {
        ret = false;
        test = flag[1];
    } else {
        ret = true;
        test = flag[0];
    }
    switch (test) {
    case 'S':
        return ((emulator->flag_register & 0b10000000) >> 7) == ret;
    case 'P':
        return ((emulator->flag_register & 0b01000000) >> 6) == ret;
    case 'Z':
        return ((emulator->flag_register & 0b00100000) >> 5) == ret;
    case 'C':
        return ((emulator->flag_register & 0b00010000) >> 4) == ret;
    case 'O':
        return ((emulator->flag_register & 0b00001000) >> 3) == ret;
    default:
        return false;
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

static uint8_t *fetch_next_byte(Emulator *emulator) { return &emulator->memory[emulator->program_counter++]; }

static MemPtr process_operand(Emulator *emulator, const char *operand) {
    MemPtr ret;
    ret.mem16 = NULL;
    ret.mem8 = NULL;
    ret.is16 = 0;
    ret.isTmp16 = false;
    ret.isZeroPage = false;

    if (!strcmp(operand, "A")) {
        ret.is16 = false;
        ret.mem8 = &emulator->a_register;
    } else if (!strcmp(operand, "B")) {
        ret.is16 = false;
        ret.mem8 = &emulator->b_register;
    } else if (!strcmp(operand, "INT")) {
        ret.is16 = false;
        ret.mem8 = &emulator->interrupt_data;
    } else if (!strcmp(operand, "CONST")) {
        ret.is16 = false;
        ret.mem8 = fetch_next_byte(emulator);
    } else if ((!strcmp(operand, "MEM8")) || (!strcmp(operand, "MEMZP"))) {
        emulator->tmp_register_8[0] = *fetch_next_byte(emulator);
        ret.is16 = false;
        ret.isZeroPage = true;
        ret.mem8 = &emulator->tmp_register_8[0];
    } else if ((!strcmp(operand, "MEM16")) || (!strcmp(operand, "MEM"))) {
        emulator->tmp_register_8[1] = *fetch_next_byte(emulator);
        emulator->tmp_register_8[0] = *fetch_next_byte(emulator);
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
        emulator->stack_pointer--;
    } else if (!strcmp(operand, "T")) {
        ret.is16 = true;
        ret.isTmp16 = true;
        ret.mem16 = &emulator->tmp_register_16;
    }

    return ret;
}

// 0 - OK
// 1 - WRONG NUMBER OF OPERANDS
// 2 - INVALID OPERANDS
static int handle_mov(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;

    MemPtr des_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (des_op.is16 || src_op.mem16 == NULL) {
        return 2;
    }

    if (src_op.is16)
        *des_op.mem8 = emulator->memory[emulator->tmp_register_16];
    else if (src_op.isZeroPage)
        *des_op.mem8 = emulator->memory[*src_op.mem8];
    else
        *des_op.mem8 = *src_op.mem8;

    return 0;
}

static int handle_movindirect(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;

    MemPtr des_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (des_op.mem16 == NULL || src_op.mem16 == NULL)
        return 2;

    if (src_op.is16)
        *des_op.mem8 = emulator->memory[*src_op.mem16];
    else {
        emulator->tmp_register_8[0] = *src_op.mem8;
        *des_op.mem8 = emulator->memory[*src_op.mem8];
    }

    return 0;
}

static int handle_inc(Emulator *emulator, Instruction instruction) {
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

static int handle_movat(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;

    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    MemPtr src_op = process_operand(emulator, instruction.operands[1]);
    if (src_op.is16 && src_op.mem16 == NULL)
        return 2;

    uint8_t src_val = *src_op.mem8; // src value (TL case) can be changed by TMP register usage

    if (adr_op.is16)
        emulator->tmp_register_16 = *adr_op.mem16;
    else
        emulator->tmp_register_8[0] = *adr_op.mem8;

    if (adr_op.is16) {
        uint8_t *destination = &emulator->memory[emulator->tmp_register_16];
        *destination = src_val;
    } else {
        uint8_t *destination = &emulator->memory[emulator->tmp_register_8[0]];
        *destination = src_val;
    }

    return 0;
}

static int handle_dec(Emulator *emulator, Instruction instruction) {
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

static int handle_neg(Emulator *emulator, Instruction instruction) {
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
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = (*src_op.mem8 ^ 255) + 1;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_inv(Emulator *emulator, Instruction instruction) {
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
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = *src_op.mem8 ^ 255;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_or(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;

    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register | emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_and(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;

    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register & emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_xor(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;

    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register ^ emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_shl(Emulator *emulator, Instruction instruction) {
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
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = (uint8_t)(*src_op.mem8 << 1);
    calculate_flags(emulator, before, *destination, 0);
    return 0;
}

static int handle_div(Emulator *emulator, Instruction instruction) {
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
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = *src_op.mem8 >> 1;
    if (before >> 7 == 1)
        *destination |= 0b10000000;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_shr(Emulator *emulator, Instruction instruction) {
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
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = *src_op.mem8 >> 1;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_add(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr adr_op = process_operand(emulator, instruction.operands[0]);
    if (adr_op.mem16 == NULL)
        return 2;

    uint8_t before = emulator->a_register;
    uint8_t *destination;
    if (adr_op.is16)
        destination = &emulator->memory[emulator->tmp_register_16];
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = emulator->a_register + emulator->b_register;
    calculate_flags(emulator, before, *destination, 0);

    return 0;
}

static int handle_sub(Emulator *emulator, Instruction instruction) {
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
    else if (adr_op.isZeroPage)
        destination = &emulator->memory[*adr_op.mem8];
    else
        destination = adr_op.mem8;
    *destination = *minuend.mem8 - *subtrahend.mem8;
    calculate_flags(emulator, before, *destination, 1);

    return 0;
}

static int handle_cmp(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2)
        return 1;

    MemPtr minuend = process_operand(emulator, instruction.operands[0]);
    MemPtr subtrahend = process_operand(emulator, instruction.operands[1]);
    if (minuend.mem16 == NULL || subtrahend.mem16 == NULL || subtrahend.is16 || minuend.is16)
        return 2;

    uint8_t lhs = *minuend.mem8;
    uint8_t rhs = *subtrahend.mem8;
    uint8_t temp = lhs - rhs;
    calculate_flags(emulator, lhs, temp, 1);

    if (minuend.mem8 != &emulator->a_register && subtrahend.mem8 != &emulator->b_register) {
        emulator->a_register = lhs;
        emulator->b_register = rhs;
    } else {
        if (minuend.mem8 == &emulator->a_register) {
            emulator->b_register = rhs;
        } else if (subtrahend.mem8 == &emulator->a_register) {
            emulator->b_register = lhs;
        } else if (minuend.mem8 == &emulator->b_register) {
            emulator->a_register = rhs;
        } else if (subtrahend.mem8 == &emulator->b_register) {
            emulator->a_register = lhs;
        }
    }

    return 0;
}

static int handle_clr(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.is16) {
        *target.mem16 = 0;
    } else {
        *target.mem8 = 0;
    }

    return 0;
}

static int handle_pop(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.mem16 == NULL)
        return 2;

    if (target.isTmp16) {
        emulator->stack_pointer++;
        emulator->tmp_register_8[0] = emulator->stack[emulator->stack_pointer];
        emulator->stack_pointer++;
        emulator->tmp_register_8[1] = emulator->stack[emulator->stack_pointer];
    } else {
        uint8_t *destination;
        if (target.is16)
            destination = &emulator->memory[*target.mem16];
        else if (target.isZeroPage)
            destination = &emulator->memory[*target.mem8];
        else
            destination = target.mem8;
        emulator->stack_pointer++;
        *destination = emulator->stack[emulator->stack_pointer];
    }

    return 0;
}

static int handle_push(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.mem16 == NULL)
        return 2;

    if (target.isTmp16) {
        emulator->stack[emulator->stack_pointer] = emulator->tmp_register_8[0];
        emulator->stack_pointer--;
        emulator->stack[emulator->stack_pointer] = emulator->tmp_register_8[1];
        emulator->stack_pointer--;
    } else {
        uint8_t *destination;
        if (target.is16)
            destination = &emulator->memory[*target.mem16];
        else if (target.isZeroPage)
            destination = &emulator->memory[*target.mem8];
        else
            destination = target.mem8;
        emulator->stack[emulator->stack_pointer] = *destination;
        emulator->stack_pointer--;
    }

    return 0;
}

static int handle_jmpimm(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.mem16 == NULL)
        return 2;

    if (check_flags(emulator, instruction.flag_dependence)) {
        emulator->program_counter = *target.mem16;
        emulator->clock_cycles_counter += instruction.pessimistic_cycle_count - instruction.cycle_count;
    }

    return 0;
}

static int handle_jmprel(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (target.is16)
        return 2;

    if (check_flags(emulator, instruction.flag_dependence)) {
        if (!strcmp(instruction.operands[0], "TL")) {
            uint8_t tl_value = emulator->tmp_register_8[0];
            emulator->tmp_register_16 = emulator->program_counter + 1;
            emulator->tmp_register_8[0] += tl_value;
            emulator->program_counter = emulator->tmp_register_16;
        } else {
            emulator->tmp_register_16 = emulator->program_counter;
            emulator->tmp_register_8[0] += *target.mem8;
            emulator->program_counter = emulator->tmp_register_16;
        }

        emulator->clock_cycles_counter += instruction.pessimistic_cycle_count - instruction.cycle_count;
    }

    return 0;
}

static int handle_jmp_fun(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1)
        return 1;

    MemPtr target = process_operand(emulator, instruction.operands[0]);
    if (!target.is16)
        return 2;

    uint8_t pc_low = (uint8_t)emulator->program_counter & 0xFF;
    uint8_t pc_high = (uint8_t)emulator->program_counter >> 8;

    emulator->stack[emulator->stack_pointer] = pc_low;
    emulator->stack_pointer--;

    emulator->stack[emulator->stack_pointer] = pc_high;
    emulator->stack_pointer--;

    emulator->program_counter = *target.mem16;

    return 0;
}

static int handle_jmp_ret(Emulator *emulator, Instruction /*instruction*/) {
    emulator->stack_pointer++;
    uint8_t pc_high = emulator->stack[emulator->stack_pointer];
    emulator->stack_pointer++;
    uint8_t pc_low = emulator->stack[emulator->stack_pointer];

    emulator->program_counter = (int16_t)((pc_high << 8) | pc_low);

    return 0;
}

static int handle_skip(Emulator *emulator, uint8_t steps) {
    emulator->program_counter += steps;
    return 0;
}

static int handle_int(Emulator *emulator, uint8_t signal_number) {
    emulator->interrupt_signals |= (1 << signal_number);
    return 0;
}

// 0 - OK
// 1 - WRONG NUMBER OF OPERANDS
// 2 - INVALID OPERANDS
// 3 - NOT IMPLEMENTED YET
int run_instruction(Emulator *emulator, Instruction instruction) {
    emulator->instruction_counter++;
    emulator->clock_cycles_counter += instruction.cycle_count;
#ifdef DBG
    log_func(DEBUG, "running instruction: %s", instruction.mnemonic);
    log_func(NONE, "operands: ");
    for (unsigned i = 0; i < instruction.num_operands; i++) {
        log_func(NONE, "%s ", instruction.operands[i]);
    }
    log_func(NONE, "clock cycles: %u\n", instruction.cycle_count);
#endif
    int ret = 0;
    if (!strcmp(instruction.mnemonic, "MOV")) {
        if (instruction.id < 28)
            ret = handle_mov(emulator, instruction);
        else
            ret = handle_movindirect(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "NOP")) {
        ;
    } else if (!strcmp(instruction.mnemonic, "HALT")) {
        emulator->is_halted = 1;
        ret = 0;
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
    } else if (!strcmp(instruction.mnemonic, "DIV2")) {
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
    } else if (!strcmp(instruction.mnemonic, "JMPIMM")) {
        ret = handle_jmpimm(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "JMPREL")) {
        ret = handle_jmprel(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "JMPFUN")) {
        ret = handle_jmp_fun(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "JMPRET")) {
        ret = handle_jmp_ret(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "CLR")) {
        ret = handle_clr(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "CLR")) {
        ret = handle_clr(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "PUSH")) {
        ret = handle_push(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "POP")) {
        ret = handle_pop(emulator, instruction);
    } else if (!strcmp(instruction.mnemonic, "SKIP")) {
        ret = handle_skip(emulator, 0);
        if (log_func != NULL) {
            log_func(INFO, "(skip) A: (s %d u: %u), B: (s %d u: %u), PC: %u", emulator->signed_a_register,
                     emulator->a_register, emulator->signed_b_register, emulator->b_register,
                     emulator->program_counter);
        }
    } else if (!strcmp(instruction.mnemonic, "SKIP1")) {
        ret = handle_skip(emulator, 1);
        if (log_func != NULL) {
            log_func(INFO, "(skip1) A: (s %d u: %u), B: (s %d u: %u), PC: %u", emulator->signed_a_register,
                     emulator->a_register, emulator->signed_b_register, emulator->b_register,
                     emulator->program_counter);
        }
    } else if (!strcmp(instruction.mnemonic, "SKIP2")) {
        ret = handle_skip(emulator, 2);
        if (log_func != NULL) {
            log_func(INFO, "(skip2) A: (s %d u: %u), B: (s %d u: %u), PC: %u", emulator->signed_a_register,
                     emulator->a_register, emulator->signed_b_register, emulator->b_register,
                     emulator->program_counter);
        }
    } else if (!strcmp(instruction.mnemonic, "INT0")) {
        ret = handle_int(emulator, 0);
    } else if (!strcmp(instruction.mnemonic, "INT1")) {
        ret = handle_int(emulator, 1);
    } else if (!strcmp(instruction.mnemonic, "INT2")) {
        ret = handle_int(emulator, 2);
    } else if (!strcmp(instruction.mnemonic, "INT3")) {
        ret = handle_int(emulator, 3);
    } else if (!strcmp(instruction.mnemonic, "INT4")) {
        ret = handle_int(emulator, 4);
    } else {
        if (log_func != NULL)
            log_func(INFO, "%s is not implemented yet :<<\n", instruction.mnemonic);
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
    Instruction *instruction = config->instructions[emulator->memory[emulator->program_counter]];
    if (instruction == NULL) {
        if (log_func != NULL)
            log_func(ERROR, "Instruction: %x is missing from config", emulator->memory[emulator->program_counter]);
        return 1;
    }
    if (emulator->is_halted) {
        if (log_func != NULL)
            log_func(INFO, "Processor is halted");
        return 2;
    }
    emulator->program_counter++;
    return run_instruction(emulator, *instruction);
}
