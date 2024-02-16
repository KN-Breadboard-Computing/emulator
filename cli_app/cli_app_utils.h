#pragma once
#include "stdint.h"
#include <stdbool.h>

extern const uint8_t SAMPLE_ROM[];
bool load_rom(uint8_t **rom, unsigned *rom_size, const char *filename);
