#pragma once

#include "cli_app_utils.h"
#include "debugger.h"
#include "emulator.h"

void handle_break(Debugger *debugger, unsigned argc, char **argv);

void handle_run(Debugger *debugger);
