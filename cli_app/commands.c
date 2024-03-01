#include "commands.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void handle_break(Debugger *debugger, const unsigned argc, char **argv) {
    if (argc < 1) {
        console_log(ERROR, "Usage: break <address (0x)HEX|DEC>");
        return;
    }
    uint16_t address;
    if (argv[0][0] == '0' && argv[0][1] == 'x') {
        argv[0] += 2;
        for (unsigned i = 0; argv[0][i] != '\0'; i++) {
            if (!isxdigit(argv[0][i])) {
                console_log(ERROR, "Invalid address");
                return;
            }
        }
        address = (uint16_t)strtol(argv[0], NULL, 16);
        argv[0] -= 2;
    } else {
        for (unsigned i = 0; argv[0][i] != '\0'; i++) {
            if (!isdigit(argv[0][i])) {
                console_log(ERROR, "Invalid address");
                return;
            }
        }
        address = (uint16_t)strtol(argv[0], NULL, 10);
    }
    add_breakpoint(debugger, address);
}

void handle_run(Debugger *debugger) { debugger->emulator_running = true; }