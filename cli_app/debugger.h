#pragma once

#include "emulator.h"
#include <stdbool.h>
#include <stdint.h>

#define MAX_BREAKPOINTS 256

typedef enum { ADDRESS, VALUE } BreakpointType;

typedef struct {
    BreakpointType type;
    uint16_t address;
    uint8_t value;
    bool is_hit;
} Breakpoint;

typedef struct {
    bool emulator_running;
    Breakpoint *breakpoints[MAX_BREAKPOINTS];
} Debugger;

void cleanup_debugger(Debugger *debugger);

void init_debugger(Debugger *debugger);

void switch_emulator_running(Debugger *debugger);

void add_breakpoint(Debugger *debugger, uint16_t address);

void add_value_breakpoint(Debugger *debugger, uint16_t address, uint8_t value);

bool check_breakpoints(Debugger *debugger, Emulator *emulator);