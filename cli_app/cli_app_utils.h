#pragma once
#include "config.h"
#include "stdint.h"
#include <stdbool.h>

typedef struct {
    char **messages;
    unsigned size;
    unsigned allocated_size;
    unsigned long *allocated_messages_size;
    unsigned displayable_amount;
    unsigned current_message;
} LogVector;

extern LogVector default_log_vector;
void init_log_vector(LogVector *log_vector, unsigned displayable_amount);
void add_log(LogVector *log_vector, char *message);
int get_log(LogVector *log_vector, char **buffer);
void free_log_vector(LogVector *log_vector);
bool load_rom(uint8_t **rom, unsigned *rom_size, const char *filename);
void console_log(log_level ll, const char *format, ...);
