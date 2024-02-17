#include "cli_app_utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const uint8_t SAMPLE_ROM[] = {
    0b00010001, // MOVAIMM 16
    16,
    0b11000101, // push a
    0b00010001, // MOVAIMM 1
    1,
    0b00010010, // MOVBIMM 67
    67,
    0b00111100, // ADDA
    0b00010010, // MOVBIMM 1
    1,
    0b00111100, // ADDA
    0b00000101, // MOVBA
    0b00111100, // ADDA
    0b00111100, // ADDA
    0b00111100, // ADDA
    0b01000001, // SUBABA
    0b01000001, // SUBABA
    0b01000001, // SUBABA
    0b11010101, // skip
    0b11001110, // POP B
    0b10010100, // inc b
    0b00010001, // MOVAIMM 20
    20,
    0b10000010, // CMPAB
    0b10100010, // JMPIMMZ
    31,         0,
    0b11000110, // PUSH b
    0b10011001, // JMPIMM 3
    3,          0,
    0b11011000, // HALT
    0b00000000,

};

bool load_rom(uint8_t **rom, unsigned *rom_size, const char *filename) {
    if (filename == NULL) {
        printf("No ROM file specified, using default\n");
        *rom_size = sizeof SAMPLE_ROM;
        *rom = (uint8_t *)malloc(*rom_size);
        memcpy(*rom, SAMPLE_ROM, *rom_size);
    } else {
        FILE *rom_file = fopen(filename, "r");
        if (rom_file == NULL)
            return false;
        fseek(rom_file, 0, SEEK_END);
        *rom_size = (unsigned)ftell(rom_file);
        fseek(rom_file, 0, SEEK_SET);
        *rom = (uint8_t *)malloc(*rom_size);
        fread(*rom, 1, *rom_size, rom_file);
        fclose(rom_file);
    }
    return true;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"

void console_log(log_level ll, const char *restrict format, ...) {
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    unsigned size = (unsigned)vsnprintf(NULL, 0, format, args_copy) + 1;
    va_end(args_copy);
    char *result = (char *)malloc(size);
    vsprintf(result, format, args);
    // TODO
    switch (ll) {
    case INFO:
        printf("INFO: %s", result);
        break;
    case WARNING:
        printf("WARNING: %s", result);
        break;
    case ERROR:
        printf("ERROR: %s", result);
        break;
    case DEBUG:
        printf("DEBUG: %s", result);
        break;
    }
    free(result);
    va_end(args);
}

#pragma clang diagnostic pop