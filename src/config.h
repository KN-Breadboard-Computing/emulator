#pragma once

#define INST_SET_SIZE 256

typedef struct {
    char* mnemonic;
    char** operands;
    int num_operands;
} Instruction;

typedef struct {
    Instruction* instructions[INST_SET_SIZE];
} Config;

/* Load a config from a file
 * 0 - 0K
 * 1 - file not found
 */
int load_config(Config* config, char* filename);
void cleanup_config(Config* config);

Instruction* get_instruction(Config* config, char* mnemonic);

