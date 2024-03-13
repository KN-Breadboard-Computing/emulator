#include "commands.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static bool has_error = false;

uint16_t get_address(char *address_str) {
    uint16_t address;
    has_error = false;
    if (address_str[0] == '\0') {
        has_error = true;
        return 0;
    }
    if (address_str[0] == '0' && address_str[1] == 'x') {
        address_str += 2;
        for (unsigned i = 0; address_str[i] != '\0'; i++) {
            if (!isxdigit(address_str[i])) {
                console_log(ERROR, "Invalid address");
                has_error = true;
                return 0;
            }
        }
        address = (uint16_t)strtol(address_str, NULL, 16);
        address_str -= 2;
    } else {
        for (unsigned i = 0; address_str[i] != '\0'; i++) {
            if (!isdigit(address_str[i])) {
                console_log(ERROR, "Invalid address");
                has_error = true;
                return 0;
            }
        }
        address = (uint16_t)strtol(address_str, NULL, 10);
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
        remove_breakpoint(debugger, debugger->last_breakpoint->address, ADDRESS);
        return;
    }
    uint16_t address;
    address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    remove_breakpoint(debugger, address, ADDRESS);
}

void handle_list(Debugger *debugger, const unsigned argc, char **argv) {
    if (argc < 2) {
        // console_log(ERROR, "Usage: list <first address (0x)HEX|DEC> <last address (0x)HEX|DEC>");
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

void handle_step(Debugger *debugger, Emulator *emulator, Config *config) {
    debugger->on_hold = true;
    run_next_emulator_instruction(emulator, config);
    debugger->on_hold = false;
}

void handle_peek(Emulator *emulator, unsigned argc, char **argv) {
    if (argc < 1) {
        console_log(ERROR, "Usage: peek <address (0x)HEX|DEC>");
        return;
    }
    uint16_t address;
    address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    console_log(INFO, "0x%04X: 0x%02X", address, emulator->memory[address]);
}

void handle_poke(Emulator *emulator, unsigned argc, char **argv) {
    if (argc < 2) {
        console_log(ERROR, "Usage: poke <address (0x)HEX|DEC> <value>");
        return;
    }
    uint16_t address;
    address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    uint8_t value;
    value = (uint8_t)get_address(argv[1]);
    if (has_error) {
        has_error = false;
        return;
    }
    emulator->memory[address] = value;
    console_log(INFO, "Changed 0x%04X to 0x%02X", address, value);
}

void handle_speed(Debugger *debugger, const unsigned argc, char **argv) {
    if (argc < 1) {
        console_log(ERROR, "Usage: speed <value>");
        return;
    }
    for (unsigned i = 0; argv[0][i] != '\0'; i++) {
        if (!isdigit(argv[0][i])) {
            console_log(ERROR, "Invalid value");
            return;
        }
    }
    unsigned speed = (unsigned)strtol(argv[0], NULL, 10);

    debugger->wait_time = speed;
}

void handle_quit(Debugger *debugger) {
    debugger->stop_emu_thread = true;
    console_log(INFO, "Stopping emulator thread...");
}

void handle_dump(Emulator *emulator, unsigned argc, char **argv) {
    if (argc < 2) {
        console_log(ERROR, "Usage: dump <first address (0x)HEX|DEC> <last address (0x)HEX|DEC> [format HEX|DEC|ASCII]");
        return;
    }
    uint8_t format;
    if (argc < 3) {
        format = 0;
    } else {
        if (!strcmp(argv[2], "HEX") || !strcmp(argv[2], "H")) {
            format = 0;
        } else if (!strcmp(argv[2], "DEC") || !strcmp(argv[2], "D")) {
            format = 1;
        } else if (!strcmp(argv[2], "ASCII") || !strcmp(argv[2], "A")) {
            format = 2;
        } else {
            console_log(ERROR, "Invalid format");
            return;
        }
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
    char *output;
    switch (format) {
    case 0:
        output = (char *)malloc((last_address - first_address + 1) * 3);
        break;
    case 1:
        output = (char *)malloc((last_address - first_address + 1) * 4);
        break;
    case 2:
        output = (char *)malloc((last_address - first_address + 1) * 2);
        break;
    }

    for (uint16_t i = first_address; i <= last_address; i++) {
        switch (format) {
        case 0:
            sprintf(output + (i - first_address) * 3, "%02X ", emulator->memory[i]);
            break;
        case 1:
            sprintf(output + (i - first_address) * 4, "%03d ", emulator->memory[i]);
            break;
        case 2:
            output[(i - first_address) * 2] = (char)emulator->memory[i];
            output[(i - first_address) * 2 + 1] = ' ';
            break;
        }
    }
    console_log(INFO, output);
    free(output);
}

void handle_load(Emulator *emulator, unsigned argc, char **argv) {
    if (argc < 2) {
        console_log(ERROR, "Usage: load <first address (0x)HEX|DEC> <HEX String>|(<HEX> <HEX> ...)");
        return;
    }
    uint16_t address;
    address = get_address(argv[0]);
    if (has_error) {
        has_error = false;
        return;
    }
    for (unsigned i = 1; i < argc; ++i) {
        unsigned long length = strlen(argv[i]);
        for (int j = (int)length - 1; j >= 0; j -= 2) {
            char *byte = (char *)malloc(2);
            byte[0] = argv[i][j - 1];
            byte[1] = argv[i][j];
            byte[2] = '\0';
            emulator->memory[address + j / 2] = (uint8_t)strtol(byte, NULL, 16);
            free(byte);
        }
        address += length / 2;
    }
}