#include "commands.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static bool has_error = false;

uint16_t get_address(char *address_str) {
    uint16_t address;
    if (address_str[0] == '0' && address_str[1] == 'x') {
        address_str += 2;
        for (unsigned i = 0; address_str[i] != '\0'; i++) {
            if (!isxdigit(address_str[i])) {
                console_log(ERROR, "Invalid address");
                has_error = true;
                return 0;
            }
        }
        address = (uint16_t) strtol(address_str, NULL, 16);
        address_str -= 2;
    } else {
        for (unsigned i = 0; address_str[i] != '\0'; i++) {
            if (!isdigit(address_str[i])) {
                console_log(ERROR, "Invalid address");
                has_error = true;
                return 0;
            }
        }
        address = (uint16_t) strtol(address_str, NULL, 10);
    }
    return address;
}

void handle_break(Debugger *debugger, const unsigned argc, char **argv) {
    if (argc < 1) {
        console_log(ERROR, "Usage: break <address (0x)HEX|DEC>");
        return;
    }
    uint16_t address;
    address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    add_breakpoint(debugger, address);
}

void handle_clear(Debugger *debugger) {
    clear_breakpoints(debugger);
    console_log(INFO, "All breakpoints removed");
}

void handle_remove(Debugger *debugger, const unsigned argc, char **argv) {
    if (argc < 1) {
        console_log(ERROR, "Usage: remove <address (0x)HEX|DEC|last>");
        return;
    }
    if (!strcmp(argv[0], "last")) {
        if (debugger->last_breakpoint == NULL) {
            console_log(ERROR, "No last breakpoint");
            return;
        }
        remove_breakpoint(debugger, debugger->last_breakpoint->address);
        return;
    }
    uint16_t address;
    address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    remove_breakpoint(debugger, address);
}

void handle_list(Debugger *debugger, const unsigned argc, char **argv) {
    if (argc < 2) {
        //console_log(ERROR, "Usage: list <first address (0x)HEX|DEC> <last address (0x)HEX|DEC>");
        print_breakpoints(debugger, 0, UINT16_MAX);
        return;
    }
    uint16_t first_address, last_address;
    first_address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    last_address = get_address(argv[1]);
    if (has_error) {
        has_error = false;
        return;
    }
    print_breakpoints(debugger, first_address, last_address);
}

void handle_run(Debugger *debugger) { debugger->emulator_running = true; }

void handle_step(Emulator *emulator, Config *config) { run_next_emulator_instruction(emulator, config); }