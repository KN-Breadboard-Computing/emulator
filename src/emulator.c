#include "emulator.h"

void init_emulator(Emulator *emulator) {
    emulator->a_register = 0;
    emulator->b_register = 0;
    emulator->tmp_register = 0;
    emulator->stack_pointer = 0;
    emulator->program_counter = 0;

    for (unsigned int i = 0; i < sizeof(emulator->memory); i++) {
        emulator->memory[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(emulator->stack); i++) {
        emulator->stack[i] = 0;
    }
}
