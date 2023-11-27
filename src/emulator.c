#include "emulator.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"



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


void* op_to_mem(Emulator *emulator, const char* op){
    if(strcmp(op,"A")==0)
        return &emulator->a_register;
    if(strcmp(op,"B")==0)
        return &emulator->b_register;
    return NULL;
}

static void calculate_flags(Emulator *emulator, uint8_t before, uint8_t after,bool is_sub) {
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

uint8_t *decode_operand(Emulator *emulator, const char *operand) {
    if (strcmp(operand, "A") == 0) {
        return &emulator->a_register;
    } else if (strcmp(operand, "B") == 0) {
        return &emulator->b_register;
    } else if (strcmp(operand, "CONST") == 0) {
        return fetch_next_byte(emulator);
    }
    return NULL;
}

int mov(uint8_t *destination, uint8_t *source) {
    *destination = *source;
    return 0;
}

// TODO: Increment clock_cycles_counter
// 0 - OK
// 1 - WRONG NUMBER OF OPERANDS
// 2 - INVALID OPERANDS
int handle_mov(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 2) {
        return 1;
    }

    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    uint8_t *source = decode_operand(emulator, instruction.operands[1]);

    if (destination == NULL || source == NULL) {
        return 2;
    }

    return mov(destination, source);
}

int handle_add(Emulator *emulator, Instruction instruction) {
    if (instruction.num_operands != 1) {
        return 1;
    }
    uint8_t *destination = decode_operand(emulator, instruction.operands[0]);
    if (destination == NULL) {
        return 2;
    }
    *destination = emulator->a_register + emulator->b_register;
    // TODO: Calculate flags
    // calculate_flags(emulator, temp, *destination, 0);
    return 0;
}

// TODO: Add error handling
int run_instruction(Emulator *emulator, Instruction instruction) {
    emulator->instruction_counter++;
    uint8_t temp;
    printf("\nrunning instruction: %s\n", instruction.mnemonic);
    printf("operands: ");
    for (int i = 0; i < instruction.num_operands; i++) {
        printf("%s ", instruction.operands[i]);
    }
    if (strcmp(instruction.mnemonic, "MOV") == 0) {
        handle_mov(emulator, instruction);
    } else if (strcmp(instruction.mnemonic, "NOP") == 0) {
        emulator->clock_cycles_counter += 3;
    }
    else if(!strcmp(instruction.mnemonic,"MOVIMM")){
        emulator->clock_cycles_counter += 4;
        *(uint8_t*) op_to_mem(emulator,instruction.operands[0])=emulator->memory[emulator->program_counter];
        emulator->program_counter++;

    }
    else if(!strcmp(instruction.mnemonic,"HALT")){
        emulator->clock_cycles_counter += 2;
        emulator->is_halted = 1;
    }
    else if(!strcmp(instruction.mnemonic,"ADD")){
        emulator->clock_cycles_counter += 4;
        temp = *(uint8_t*) op_to_mem(emulator,instruction.operands[0]);
        *(uint8_t*) op_to_mem(emulator,instruction.operands[0])=emulator->a_register+emulator->b_register;
        calculate_flags(emulator, temp, *(uint8_t*) op_to_mem(emulator,instruction.operands[0]),0);
    }
    else if(!strcmp(instruction.mnemonic,"SUB")){
        emulator->clock_cycles_counter += 4;
        temp = *(uint8_t*) op_to_mem(emulator,instruction.operands[0]);
        *(uint8_t*) op_to_mem(emulator,instruction.operands[2])=*(uint8_t*) op_to_mem(emulator,instruction.operands[0])-*(uint8_t*) op_to_mem(emulator,instruction.operands[1]);
        calculate_flags(emulator, temp, *(uint8_t*) op_to_mem(emulator,instruction.operands[0]),1);
    }
    else if(!strcmp(instruction.mnemonic,"SKIP")){
        emulator->clock_cycles_counter += 2;
        printf("(skip) A: signed: %d unsigned: %u\n", emulator->signed_a_register, emulator->a_register);
    } else {
        printf("not implemented yet :<<\n");
        emulator->program_counter += instruction.num_operands;
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

