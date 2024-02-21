#include "config.h"
#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void (*log_func)(log_level ll, const char *format, ...) = NULL;

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

// Hashes by turning the binary string into it's decimal representation
static int hash_instruction(const char *binary_string) {
    int result = 0;
    for (int i = 0; i < 10; i++) {
        if (binary_string[i] == '\0')
            break;
        result <<= 1;
        if (binary_string[i] == '1')
            result |= 1;
    }
    return result;
}

/// 0 - OK
/// 1 - No File
/// 2 - invalid json
int load_config(Config *config, const char *const filename) {
    // 1. Parse the config json file using cJSON
    // 2. Fill the config struct
    FILE *config_file = fopen(filename, "r");
    if (config_file == NULL) {
        if (log_func != NULL)
            log_func(ERROR, "File not found\n");
        return 1;
    }
    char *buffer;
    unsigned length;
    fseek(config_file, 0, SEEK_END);
    length = (unsigned)ftell(config_file);
    fseek(config_file, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer)
        fread(buffer, 1, length, config_file);
    fclose(config_file);
    cJSON *conf = cJSON_Parse(buffer);
    free(buffer);
    if (conf == NULL) {
        cJSON_Delete(conf);
        if (log_func != NULL)
            log_func(ERROR, "Invalid JSON\n");
        return 2;
    }
    cJSON *inst;
    inst = conf->child;
    if (inst == NULL) {
        cJSON_Delete(conf);
        if (log_func != NULL)
            log_func(ERROR, "Invalid JSON\n");
        return 2;
    }
    while (inst != NULL) {
        int opcode = hash_instruction(cJSON_GetStringValue(cJSON_GetObjectItem(inst, "opcode")));
        char *mnemonic = cJSON_GetStringValue(cJSON_GetObjectItem(inst, "mnemonic"));
        char *flag_dependence = cJSON_GetStringValue(cJSON_GetObjectItem(inst, "depend-on-flag"));
        cJSON *operands_arr = cJSON_GetObjectItem(inst, "arguments");
        unsigned num_operands = (unsigned)cJSON_GetArraySize(operands_arr);
        config->instructions[opcode] = (Instruction *)malloc(sizeof(Instruction));
        config->instructions[opcode]->mnemonic = (char *)malloc(strlen(mnemonic) + 1);
        config->instructions[opcode]->num_operands = num_operands;
        strcpy(config->instructions[opcode]->mnemonic, mnemonic);
        config->instructions[opcode]->operands = (char **)malloc(sizeof(char *) * num_operands);
        config->instructions[opcode]->flag_dependence = (char *)malloc(strlen(flag_dependence) + 1);
        strcpy(config->instructions[opcode]->flag_dependence, flag_dependence);
        config->instructions[opcode]->cycle_count =
            (unsigned)cJSON_GetNumberValue(cJSON_GetObjectItem(inst, "min-cycles-number"));
        config->instructions[opcode]->pessimistic_cycle_count =
            (unsigned)cJSON_GetNumberValue(cJSON_GetObjectItem(inst, "max1-cycles-number"));
        cJSON *operand = NULL;
        unsigned it = 0;
        cJSON_ArrayForEach(operand, operands_arr) {
            char *opp = cJSON_GetStringValue(operand);
            config->instructions[opcode]->operands[it] = (char *)malloc(strlen(opp) + 1);
            strcpy(config->instructions[opcode]->operands[it], opp);
            it++;
        }
        inst = inst->next;
    }
    cJSON_Delete(conf);
    return 0;
}

void cleanup_config(Config *config) {
    for (int i = 0; i < INST_SET_SIZE; i++) {
        Instruction *instr = config->instructions[i];
        if (!instr)
            continue;

        for (unsigned j = 0; j < instr->num_operands; j++) {
            free(instr->operands[j]);
        }
        free(instr->operands);
        free(instr->mnemonic);
        free(instr->flag_dependence);
        free(instr);
    }
}
