#pragma once

#include "emulator.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_BREAKPOINTS 256

typedef enum { ADDRESS, VALUE, VALUE_GUARD } BreakpointType;

typedef struct {
    BreakpointType type;
    uint16_t address;
    uint8_t value;
    bool is_hit;
} Breakpoint;

typedef struct {
    bool emulator_running;
    Breakpoint *breakpoints[MAX_BREAKPOINTS];
    Breakpoint *last_breakpoint;
    volatile unsigned wait_time;
    volatile bool on_hold;
    volatile bool stop_emu_thread;
} Debugger;

void cleanup_debugger(Debugger *debugger);

void init_debugger(Debugger *debugger);

void switch_emulator_running(Debugger *debugger);

void add_breakpoint(Debugger *debugger, uint16_t address);

void add_value_breakpoint(Debugger *debugger, uint16_t address, uint8_t value);

void add_value_ch_breakpoint(Debugger *debugger, Emulator *emulator, uint16_t address);

bool check_breakpoints(Debugger *debugger, Emulator *emulator);

void remove_breakpoint(Debugger *debugger, uint16_t address, BreakpointType type);

void clear_breakpoints(Debugger *debugger);

void print_breakpoints(Debugger *debugger, uint16_t first_address, uint16_t last_address);