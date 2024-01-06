#pragma once

#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define STACK_SIZE 65536
#define MEMORY_SIZE 65536
#define DEBUG

typedef struct {
    union {
        uint8_t a_register;
        int8_t signed_a_register;
    };

    union {
        uint8_t b_register;
        int8_t signed_b_register;
    };

    union {
        uint16_t tmp_register_16;
        uint8_t tmp_register_8[2];
    };

    uint8_t flag_register;
    uint16_t stack_pointer;
    uint16_t program_counter;
    bool is_halted;

    uint8_t memory[MEMORY_SIZE];
    uint8_t stack[STACK_SIZE];

    // for debug purposes
    uint32_t instruction_counter;
    uint32_t clock_cycles_counter;
} Emulator;

typedef struct {
    bool is16;

    union {
        uint8_t *mem8;
        uint16_t *mem16;
    };
} MemPtr;

extern void (*log_func)(const char *, ...);

void init_emulator(Emulator *emulator);

int run_next_emulator_instruction(Emulator *emulator, Config *config);

int run_instruction(Emulator *emulator, Instruction instruction);
