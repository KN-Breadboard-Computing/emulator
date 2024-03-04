#include "cli_app_utils.h"
#include "commands.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const uint8_t SAMPLE_ROM[] = {0b00010001, // MOVAIMM 16
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
                              0b11010101, // skip
                              0b10000010, // CMPAB
                              0b10100010, // JMPIMMZ
                              32,
                              0,
                              0b11000110, // PUSH b
                              0b10011001, // JMPIMM 3
                              3,
                              0,
                              0b11011000, // HALT
                              0b00000000,
                              1

};

void init_log_vector(LogVector *log_vector, unsigned displayable_amount) {
    log_vector->size = 0;
    log_vector->allocated_size = 2;
    log_vector->messages = (char **)malloc(log_vector->allocated_size * sizeof(char *));
    log_vector->allocated_messages_size = (unsigned long *)calloc(log_vector->allocated_size, sizeof(unsigned long));
    log_vector->displayable_amount = displayable_amount;
    log_vector->current_message = 0;
}

void add_log(LogVector *log_vector, char *message) {
    if (log_vector->size >= log_vector->allocated_size) {
        log_vector->messages = realloc(log_vector->messages, (log_vector->allocated_size * 2) * sizeof(char *));
        log_vector->allocated_messages_size =
            realloc(log_vector->allocated_messages_size, (log_vector->allocated_size * 2) * sizeof(unsigned long));
        for (unsigned i = log_vector->allocated_size; i < log_vector->allocated_size * 2; i++) {
            log_vector->allocated_messages_size[i] = 0;
        }
        for (unsigned i = log_vector->allocated_size; i < log_vector->allocated_size * 2; i++) {
            log_vector->messages[i] = NULL;
        }
        log_vector->allocated_size *= 2;
    }
    if (log_vector->allocated_messages_size[log_vector->size] <= (strlen(message) + 1)) {
        log_vector->messages[log_vector->size] =
            (char *)realloc(log_vector->messages[log_vector->size], strlen(message) + 1);
        log_vector->allocated_messages_size[log_vector->size] = strlen(message) + 1;
    }
    strcpy(log_vector->messages[log_vector->size], message);
    log_vector->size++;
    log_vector->current_message += log_vector->size > log_vector->displayable_amount ? 1 : 0;
}

int get_log(LogVector *log_vector, char **buffer) {
    if (log_vector->size <= log_vector->current_message) {
        log_vector->current_message =
            log_vector->size > log_vector->displayable_amount ? log_vector->size - log_vector->displayable_amount : 0;
        return -1;
    }
    *buffer = (char *)realloc(*buffer, strlen(log_vector->messages[log_vector->current_message]) + 1);
    strcpy(*buffer, log_vector->messages[log_vector->current_message]);
    log_vector->current_message++;
    return 0;
}

void free_log_vector(LogVector *log_vector) {
    for (unsigned i = 0; i < log_vector->allocated_size; i++) {
        free(log_vector->messages[i]);
    }
    free(log_vector->messages);
    free(log_vector->allocated_messages_size);
}

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
    char *result;
    const char *info_prefix = "[INFO] ";
    const char *warning_prefix = "[WARNING] ";
    const char *error_prefix = "[ERROR] ";
    const char *debug_prefix = "[DEBUG] ";
    unsigned long offset = 0;
    switch (ll) {
    case INFO:
        offset = strlen(info_prefix);
        result = (char *)malloc(size + offset);
        strcpy(result, info_prefix);
        break;
    case WARNING:
        offset = strlen(warning_prefix);
        result = (char *)malloc(size + offset);
        strcpy(result, warning_prefix);
        break;
    case ERROR:
        offset = strlen(error_prefix);
        result = (char *)malloc(size + offset);
        strcpy(result, error_prefix);
        break;
    case DEBUG:
        offset = strlen(debug_prefix);
        result = (char *)malloc(size + offset);
        strcpy(result, debug_prefix);
        break;
    case NONE:
        result = (char *)malloc(size);
        break;
    }
    vsprintf((result + offset), format, args);
    if (default_log_vector.messages == NULL) {
        printf("%s\n", result);
    } else {
        add_log(&default_log_vector, result);
    }
    free(result);
    va_end(args);
}

#pragma clang diagnostic pop

void execute_command(Debugger *debugger, Emulator *emulator, Config *config, char *command) {
    char *rest = command;
    unsigned size;
    for (size = 0; command[size] != '\0'; command[size] == ' ' ? size++ : (unsigned)*command++)
        ;
    char *cmd = strtok_r(rest, " ", &rest);
    char **args = (char **)malloc(size * sizeof(char *) + 1);
    for (unsigned i = 0; i < size; i++) {
        char *arg = strtok_r(rest, " ", &rest);
        args[i] = malloc(strlen(arg) + 1);
        strcpy(args[i], arg);
    }
    if ((!strcmp(cmd, "break")) || (!strcmp(cmd, "br"))) {
        handle_break(debugger, size, args);
    } else if ((!strcmp(cmd, "clear")) || (!strcmp(cmd, "brc"))) {
        handle_clear(debugger);
    } else if ((!strcmp(cmd, "remove")) || (!strcmp(cmd, "brr"))) {
        handle_remove(debugger, size, args);
    } else if ((!strcmp(cmd, "list") || (!strcmp(cmd, "brl")))) {
        handle_list(debugger, size, args);
    } else if (!strcmp(cmd, "run")) {
        handle_run(debugger);
    } else if (!strcmp(cmd, "peek")) {
        handle_peek(emulator, size, args);
    } else if (!strcmp(cmd, "poke")) {
        handle_poke(emulator, size, args);
    } else if (!strcmp(cmd, "step")) {
        handle_step(emulator, config);
    } else if (!strcmp(cmd, "quit")) {
        //  handle_quit(debugger);
    } else {
        console_log(ERROR, "Unknown command: %s", cmd);
    }
    for (unsigned i = 0; i < size; i++) {
        free(args[i]);
    }
    free(args);
}

bool call_debugger(Debugger *debugger, Emulator *emulator) {
    if (check_breakpoints(debugger, emulator)) {
        debugger->emulator_running = false;
    }
    return debugger->emulator_running;
}