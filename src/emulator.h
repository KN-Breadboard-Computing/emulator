#pragma once
#include <stdint.h>

typedef struct {
    uint8_t a_register;
    uint8_t b_register;
    uint16_t tmp_register;
    uint16_t stack_pointer;
    uint16_t program_counter;

    uint8_t memory[65536];
    uint8_t stack[65536];
} Emulator;

void init_emulator(Emulator *emulator);
