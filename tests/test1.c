#include "emulator.h"
#include <stdio.h>

int main() {
    // Perform a test: check if add function works correctly
    int result = add(2, 2);

    // Check the result and return accordingly
    if (result == 4) {
        printf("Test passed.\n");
        return 0; // Return 0 for success
    } else {
        printf("Test failed: Expected 4, got %d.\n", result);
        return 1; // Return non-zero for failure
    }
}
