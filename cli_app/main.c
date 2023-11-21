#include <stdio.h>
#include "emulator.h"

int main(void) {
    Emulator emulator;
    init_emulator(&emulator);

    printf("%lu", sizeof(emulator.memory));
    return 0;
}
