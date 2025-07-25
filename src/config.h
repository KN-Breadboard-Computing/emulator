#pragma once

#undef CJSON_WITH_CMAKE

#define INST_SET_SIZE 256

typedef struct {
    unsigned id;
    char *mnemonic;
    char **operands;
    char *flag_dependence;
    unsigned cycle_count;
    unsigned pessimistic_cycle_count;
    unsigned num_operands;
} Instruction;

typedef struct {
    Instruction *instructions[INST_SET_SIZE];
} Config;

/* Load a config from a file
 * 0 - 0K
 * 1 - file not found
 */
typedef enum { INFO, WARNING, ERROR, DEBUG, NONE } LogLevel;

extern void (*log_func)(LogLevel ll, const char *format, ...);

int load_config(Config *config, const char *filename);

int print_config(Config *config);

void cleanup_config(Config *config);
