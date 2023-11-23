#include "config.h"
#include <stdlib.h>

int load_config(Config* config, char* filename) {
    // 1. Parse the config json file using cJSON
    // 2. Fill the config struct
    for(int i = 0; i < INST_SET_SIZE; i++) {
        config->instructions[i] = NULL;
    }

    return 0;
}

// Hashes by turning the binary string into it's decimal representation
int hash_instruction(const char *binary_string) {
    int result = 0;

    for (int i = 0; i < 8; i++) {
        result <<= 1;
        if (binary_string[i] == '1') {
            result |= 1;
        }
    }

    return result;
}

Instruction* get_instruction(Config* config, char* mnemonic) {
    int hash = hash_instruction(mnemonic);
    return config->instructions[hash];
}

void cleanup_config(Config* config) {
    for (int i = 0; i < INST_SET_SIZE; i++) {
        Instruction *instr = config->instructions[i];
        if(!instr) continue;

        for (int j = 0; j < instr->num_operands; j++) {
            free(instr->operands[j]);
        }
        free(instr->operands);
        free(instr->mnemonic);
        free(instr);
    }
}
