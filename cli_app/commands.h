#pragma once

#include "cli_app_utils.h"
#include "debugger.h"
#include "emulator.h"

void handle_break(Debugger *debugger, unsigned argc, char **argv);

void handle_remove(Debugger *debugger, unsigned argc, char **argv);

void handle_clear(Debugger *debugger);

void handle_list(Debugger *debugger, unsigned argc, char **argv);

void handle_run(Debugger *debugger);

void handle_step(Emulator *emulator, Config *config);

void handle_peek(Emulator *emulator, unsigned argc, char **argv);

void handle_poke(Emulator *emulator, unsigned argc, char **argv);
