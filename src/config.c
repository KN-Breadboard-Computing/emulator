#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// temporally hardcoded config
int load_config_temp(Config *config) {
    for (int i = 0; i < INST_SET_SIZE; i++) {
        config->instructions[i] = NULL;
    }
    // MOVAB
    config->instructions[0b00000001] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00000001]->mnemonic = (char *)malloc(sizeof(char) * 4);
    config->instructions[0b00000001]->num_operands = 2;
    strcpy(config->instructions[0b00000001]->mnemonic, "MOV");
    config->instructions[0b00000001]->operands = (char **)malloc(sizeof(char *) * 2);
    config->instructions[0b00000001]->operands[0] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00000001]->operands[0], "A");
    config->instructions[0b00000001]->operands[1] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00000001]->operands[1], "B");
    // NOP
    config->instructions[0b00000000] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00000000]->mnemonic = (char *)malloc(sizeof(char) * 4);
    config->instructions[0b00000000]->num_operands = 0;
    strcpy(config->instructions[0b00000000]->mnemonic, "NOP");
    config->instructions[0b00000000]->operands = NULL;
    // MOVBA
    config->instructions[0b00000010] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00000010]->mnemonic = (char *)malloc(sizeof(char) * 4);
    config->instructions[0b00000010]->num_operands = 2;
    strcpy(config->instructions[0b00000010]->mnemonic, "MOV");
    config->instructions[0b00000010]->operands = (char **)malloc(sizeof(char *) * 2);
    config->instructions[0b00000010]->operands[0] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00000010]->operands[0], "B");
    config->instructions[0b00000010]->operands[1] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00000010]->operands[1], "A");
    // MOVAIMM
    config->instructions[0b00001101] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00001101]->mnemonic = (char *)malloc(sizeof(char) * 7);
    config->instructions[0b00001101]->num_operands = 1;
    strcpy(config->instructions[0b00001101]->mnemonic, "MOVIMM");
    config->instructions[0b00001101]->operands = (char **)malloc(sizeof(char *));
    config->instructions[0b00001101]->operands[0] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00001101]->operands[0], "A");
    // MOVBIMM
    config->instructions[0b00001110] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00001110]->mnemonic = (char *)malloc(sizeof(char) * 7);
    config->instructions[0b00001110]->num_operands = 1;
    strcpy(config->instructions[0b00001110]->mnemonic, "MOVIMM");
    config->instructions[0b00001110]->operands = (char **)malloc(sizeof(char *));
    config->instructions[0b00001110]->operands[0] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00001110]->operands[0], "B");
    // HALT
    config->instructions[0b11111111] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b11111111]->mnemonic = (char *)malloc(sizeof(char) * 5);
    config->instructions[0b11111111]->num_operands = 0;
    strcpy(config->instructions[0b11111111]->mnemonic, "HALT");
    config->instructions[0b11111111]->operands = NULL;
    // SKIP
    config->instructions[0b11111110] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b11111110]->mnemonic = (char *)malloc(sizeof(char) * 5);
    config->instructions[0b11111110]->num_operands = 0;
    strcpy(config->instructions[0b11111110]->mnemonic, "SKIP");
    config->instructions[0b11111110]->operands = NULL;
    // ADDA
    config->instructions[0b00011010] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00011010]->mnemonic = (char *)malloc(sizeof(char) * 4);
    config->instructions[0b00011010]->num_operands = 1;
    strcpy(config->instructions[0b00011010]->mnemonic, "ADD");
    config->instructions[0b00011010]->operands = (char **)malloc(sizeof(char *));
    config->instructions[0b00011010]->operands[0] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00011010]->operands[0], "B");
    // SUBABA
    config->instructions[0b00011101] = (Instruction *)malloc(sizeof(Instruction));
    config->instructions[0b00011101]->mnemonic = (char *)malloc(sizeof(char) * 4);
    config->instructions[0b00011101]->num_operands = 3;
    strcpy(config->instructions[0b00011101]->mnemonic, "SUB");
    config->instructions[0b00011101]->operands = (char **)malloc(sizeof(char *) * 3);
    config->instructions[0b00011101]->operands[0] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00011101]->operands[0], "A");
    config->instructions[0b00011101]->operands[1] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00011101]->operands[1], "B");
    config->instructions[0b00011101]->operands[2] = (char *)malloc(sizeof(char) * 2);
    strcpy(config->instructions[0b00011101]->operands[2], "A");

    return 0;
}
int print_config(Config *config) {
    for (int i = 0; i < INST_SET_SIZE; i++) {
        if (config->instructions[i] == NULL)
            continue;
        printf("%d: %s ", i, config->instructions[i]->mnemonic);
        for (unsigned j = 0; j < config->instructions[i]->num_operands; j++) {
            printf("%s ", config->instructions[i]->operands[j]);
        }
        printf("\n");
    }
    return 0;
}
int load_config(Config *config, char *filename) {
    // 1. Parse the config json file using cJSON
    // 2. Fill the config struct
    for (int i = 0; i < INST_SET_SIZE; i++) {
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

Instruction *get_instruction(Config *config, char *mnemonic) {
    int hash = hash_instruction(mnemonic);
    return config->instructions[hash];
}

void cleanup_config(Config *config) {
    for (int i = 0; i < INST_SET_SIZE; i++) {
        Instruction *instr = config->instructions[i];
        if (!instr)
            continue;

        for (int j = 0; j < instr->num_operands; j++) {
            free(instr->operands[j]);
        }
        free(instr->operands);
        free(instr->mnemonic);
        free(instr);
    }
}
