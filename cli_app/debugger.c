#include "debugger.h"
#include "cli_app_utils.h"
#include <stdlib.h>

void init_debugger(Debugger *debugger) {
    debugger->breakpoints[0] = NULL;
    debugger->emulator_running = true;
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

bool check_breakpoints(Debugger *debugger, Emulator *emulator) {
    bool is_breakpoint_hit = false;
    for (unsigned i = 0; i < MAX_BREAKPOINTS; i++) {
        if (debugger->breakpoints[i] == NULL)
            continue;
        if (debugger->breakpoints[i]->type == ADDRESS) {
            if (debugger->breakpoints[i]->address == emulator->program_counter) {
                if (debugger->breakpoints[i]->is_hit)
                    continue;
                is_breakpoint_hit = true;
                console_log(INFO, "Breakpoint hit at 0x%04X", emulator->program_counter);
                debugger->breakpoints[i]->is_hit = true;
            } else if (debugger->breakpoints[i]->is_hit) {
                debugger->breakpoints[i]->is_hit = false;
            }
        }
    }
    return is_breakpoint_hit;
}