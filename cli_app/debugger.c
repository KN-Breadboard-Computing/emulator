#include "debugger.h"
#include "cli_app_utils.h"
#include <stdlib.h>

void init_debugger(Debugger *debugger) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        debugger->breakpoints[i] = NULL;
    }
    debugger->emulator_running = true;
    debugger->last_breakpoint = NULL;
}

void cleanup_debugger(Debugger *debugger) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        free(debugger->breakpoints[i]);
    }
}

void switch_emulator_running(Debugger *debugger) { debugger->emulator_running = !debugger->emulator_running; }

void add_breakpoint(Debugger *debugger, uint16_t address) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        if (address == debugger->breakpoints[i]->address && debugger->breakpoints[i]->type == ADDRESS) {
            console_log(ERROR, "Breakpoint already exists at 0x%04X", address);
            return;
        }
    }
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] != NULL)
            continue;
        debugger->breakpoints[i] = malloc(sizeof(Breakpoint));
        debugger->breakpoints[i]->type = ADDRESS;
        debugger->breakpoints[i]->is_hit = false;
        debugger->breakpoints[i]->address = address;
        console_log(INFO, "Breakpoint added at 0x%04X", address);
        return;
    }
    console_log(ERROR, "No more space for breakpoints");
}
void add_value_breakpoint(Debugger *debugger, uint16_t address, uint8_t value) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        if (address == debugger->breakpoints[i]->address && debugger->breakpoints[i]->type == VALUE) {
            console_log(ERROR, "Breakpoint already exists at 0x%04X", address);
            return;
        }
    }
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] != NULL)
            continue;
        debugger->breakpoints[i] = malloc(sizeof(Breakpoint));
        debugger->breakpoints[i]->type = VALUE;
        debugger->breakpoints[i]->is_hit = false;
        debugger->breakpoints[i]->address = address;
        debugger->breakpoints[i]->value = value;
        console_log(INFO, "Value breakpoint added at 0x%04X", address);
        return;
    }
    console_log(ERROR, "No more space for breakpoints");
}

void add_value_ch_breakpoint(Debugger *debugger,Emulator *emulator, uint16_t address) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        if (address == debugger->breakpoints[i]->address && debugger->breakpoints[i]->type == VALUE_GUARD) {
            console_log(ERROR, "Value guard already exists at 0x%04X", address);
            return;
        }
    }
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] != NULL)
            continue;
        debugger->breakpoints[i] = malloc(sizeof(Breakpoint));
        debugger->breakpoints[i]->type = VALUE_GUARD;
        debugger->breakpoints[i]->is_hit = false;
        debugger->breakpoints[i]->address = address;
        debugger->breakpoints[i]->value = emulator->memory[address];
        console_log(INFO, "Value guard added at 0x%04X", address);
        return;
    }
    console_log(ERROR, "No more space for breakpoints");
}
void remove_breakpoint(Debugger *debugger, uint16_t address,BreakpointType type) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        if (debugger->breakpoints[i]->address == address && debugger->breakpoints[i]->type == type) {
            if (debugger->last_breakpoint == debugger->breakpoints[i])
                debugger->last_breakpoint = NULL;
            free(debugger->breakpoints[i]);
            debugger->breakpoints[i] = NULL;
            console_log(INFO, "Breakpoint removed at 0x%04X", address);
            return;
        }
    }
    console_log(ERROR, "Breakpoint not found at 0x%04X", address);
}
void clear_breakpoints(Debugger *debugger) {
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        free(debugger->breakpoints[i]);
        debugger->breakpoints[i] = NULL;
    }
    debugger->last_breakpoint = NULL;
}

bool check_breakpoints(Debugger *debugger, Emulator *emulator) {
    bool is_breakpoint_hit = false;
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        switch (debugger->breakpoints[i]->type) {
        case ADDRESS:
            if (debugger->breakpoints[i]->address == emulator->program_counter) {
                if (debugger->breakpoints[i]->is_hit)
                    continue;
                is_breakpoint_hit = true;
                console_log(INFO, "Breakpoint hit at 0x%04X", emulator->program_counter);
                debugger->breakpoints[i]->is_hit = true;
                debugger->last_breakpoint = debugger->breakpoints[i];
            } else if (debugger->breakpoints[i]->is_hit) {
                debugger->breakpoints[i]->is_hit = false;
            }
            break;
        case VALUE:
            if (emulator->memory[debugger->breakpoints[i]->address] == debugger->breakpoints[i]->value) {
                if (debugger->breakpoints[i]->is_hit)
                    continue;
                is_breakpoint_hit = true;
                console_log(INFO, "Value at 0x%04X is 0x%02X", debugger->breakpoints[i]->address, debugger->breakpoints[i]->value);
                debugger->breakpoints[i]->is_hit = true;
                debugger->last_breakpoint = debugger->breakpoints[i];
            } else if (debugger->breakpoints[i]->is_hit) {
                debugger->breakpoints[i]->is_hit = false;
            }
            break;
        case VALUE_GUARD:
            if (emulator->memory[debugger->breakpoints[i]->address] != debugger->breakpoints[i]->value) {
                if (debugger->breakpoints[i]->is_hit)
                    continue;
                is_breakpoint_hit = true;
                console_log(INFO, "Value at 0x%04X is 0x%02X", debugger->breakpoints[i]->address, debugger->breakpoints[i]->value);
                debugger->breakpoints[i]->is_hit = true;
                debugger->last_breakpoint = debugger->breakpoints[i];
            } else if (debugger->breakpoints[i]->is_hit) {
                debugger->breakpoints[i]->is_hit = false;
                debugger->breakpoints[i]->value = emulator->memory[debugger->breakpoints[i]->address];
            }
        }
    }
    return is_breakpoint_hit;
}

void print_breakpoints(Debugger *debugger, uint16_t first_address, uint16_t last_address) {
    uint8_t count = 0;
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        if (debugger->breakpoints[i]->address >= first_address && debugger->breakpoints[i]->address <= last_address) {
            count++;
            if (debugger->breakpoints[i]->type == ADDRESS)
                console_log(INFO, "Breakpoint found at 0x%04X", debugger->breakpoints[i]->address);
            else
                console_log(INFO, "Value breakpoint found at 0x%04X with value 0x%02X", debugger->breakpoints[i]->address, debugger->breakpoints[i]->value);
        }
    }
    if (count == 0)
        console_log(INFO, "No breakpoints found");
}