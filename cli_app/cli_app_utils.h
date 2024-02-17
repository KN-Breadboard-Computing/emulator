#pragma once
#include "config.h"
#include "stdint.h"
#include <stdbool.h>

bool load_rom(uint8_t **rom, unsigned *rom_size, const char *filename);
void console_log(log_level ll, const char *format, ...);