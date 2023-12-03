#include "emulator.h"
#include <curses.h>
#include <stdio.h>

#define x_frame_character '#'
#define y_frame_character '|'

typedef enum : uint8_t { HEX, DEC, DEC_SIGNED, INST, ASCII } Memory_view_mode;

static int y, x, max_y, max_x;
static uint8_t memory_tables_count = 3;
static Memory_view_mode current_memory_view_mode[3] = {ASCII, HEX, DEC_SIGNED};

void print_emulator_status(Emulator *emulator) {
    int def_x = max_x - 30, def_y = 2;
    x = def_x;
    y = def_y;
    mvprintw(y++, x, "Emulator Registers:");
    mvprintw(y++, x, "A: signed: %d unsigned: %u", emulator->signed_a_register, emulator->a_register);
    mvprintw(y++, x, "B: signed: %d unsigned: %u", emulator->signed_b_register, emulator->b_register);
    mvprintw(y++, x, "tmp: %u", emulator->tmp_register);
    mvprintw(y++, x, "pc: %u", emulator->program_counter);
    mvprintw(y++, x, "sp: %u", emulator->stack_pointer);
    mvprintw(y++, x, "flag: S P Z C O - - -");
    x += 4;
    uint8_t a = emulator->flag_register;
    for (int i = 0; i < 8; i++) {
        mvprintw(y, x += 2, "%d ", ((a << i) & 0x80) != 0);
    }
    x = def_x;
    y += 2;
    mvprintw(y++, x, "Emulator Stats:");
    mvprintw(y++, x, "is_halted: %u", emulator->is_halted);
    mvprintw(y++, x, "clock_cycles_counter: %u", emulator->clock_cycles_counter);
    mvprintw(y++, x, "instruction_counter: %u", emulator->instruction_counter);
    x = def_x - 2;
    for (y = 1; y < 16; ++y) {
        mvaddch(y, x, y_frame_character);
    }
    for (; x < max_x - 1; x++) {
        mvaddch(y, x, x_frame_character);
    }
}

void print_memory(Emulator *emulator) {
    int def_x = 2, def_y = 2;
    int spacing[5] = {3, 4, 5, 5, 2};
    int spacing_between = 5;
    x = def_x;
    y = def_y;
    mvprintw(y++, x, "Memory:");
    x += 9;
    // print labels
    for (int i = 0; i < memory_tables_count; ++i) {
        switch (current_memory_view_mode[i]) {
        case HEX:
            mvprintw(y, x, "HEX");
            break;
        case DEC:
            mvprintw(y, x, "DEC");
            break;
        case DEC_SIGNED:
            mvprintw(y, x, "DEC_SIGNED");
            break;
        case INST:
            mvprintw(y, x, "INST");
            break;
        case ASCII:
            mvprintw(y, x, "ASCII");
            break;
        }
        x += 16 * spacing[current_memory_view_mode[i]] + 10;
    }
    // print top row
    y++;
    x = def_x + 2;
    for (int i = 0; i < memory_tables_count; ++i) {
        switch (current_memory_view_mode[i]) {
        case HEX:
            for (int j = 0; j < 16; j++) {
                mvprintw(y, x += spacing[current_memory_view_mode[i]], "%2X ", j);
            }
            break;
        case DEC:
            for (int j = 0; j < 16; j++) {
                mvprintw(y, x += spacing[current_memory_view_mode[i]], "%2X ", j);
            }
            break;
        case DEC_SIGNED:
            for (int j = 0; j < 16; j++) {
                mvprintw(y, x += spacing[current_memory_view_mode[i]], "%2X ", j);
            }
            break;
        case INST:
            for (int j = 0; j < 16; j++) {
                mvprintw(y, x += spacing[current_memory_view_mode[i]], "%2X ", j);
            }
            break;
        case ASCII:
            for (int j = 0; j < 16; j++) {
                mvprintw(y, x += spacing[current_memory_view_mode[i]], "%2X ", j);
            }
            break;
        }
        x += spacing_between;
    }
    // print vertical numbers
    x = def_x + 2;
    for (int i = 0; i < memory_tables_count; ++i) {
        y = def_y + 3;
        for (int j = 0; j < 16; j++) {
            mvprintw(y++, x, "%02X ", j * 16);
        }
        x += spacing[current_memory_view_mode[i]] * 16 + spacing_between;
    }
    y = def_y + 3;
    for (int j = 0; j < 16; j++) {
        mvprintw(y++, x, "%02X ", j * 16);
    }
    // print values

    for (int k = 0; k < 16; ++k) {
        x = def_x + 3;
        y = def_y + 3 + k;
        for (int i = 0; i < memory_tables_count; ++i) {
            switch (current_memory_view_mode[i]) {
            case HEX:
                for (int j = 0; j < 16; j++) {
                    mvprintw(y, x += spacing[current_memory_view_mode[i]], "%02X ", emulator->memory[j + 16 * k]);
                }
                break;
            case DEC:
                for (int j = 0; j < 16; j++) {
                    mvprintw(y, x += spacing[current_memory_view_mode[i]], "%u", emulator->memory[j + 16 * k]);
                }
            case DEC_SIGNED:
                for (int j = 0; j < 16; j++) {
                    mvprintw(y, x += spacing[current_memory_view_mode[i]], "%d", (int8_t)emulator->memory[j + 16 * k]);
                }
                break;
            case INST:
                mvprintw(y, x += spacing[current_memory_view_mode[i]], "X");
                x += 5;
                break;
            case ASCII:
                for (int j = 0; j < 16; j++) {
                    if (emulator->memory[j + 16 * k] > 32)
                        mvprintw(y, x += spacing[current_memory_view_mode[i]], "%c", emulator->memory[j + 16 * k]);
                    else
                        mvaddch(y, x += spacing[current_memory_view_mode[i]], '.');
                }
                break;
            }
            x += spacing_between;
        }
    }
}

void print_console(void) {
    int def_x = 2, def_y = max_y - 15;
    x = def_x;
    y = def_y;
    for (int i = 0; i < max_x; i++) {
        mvaddch(y, i, x_frame_character);
    }
    y++;
    mvprintw(y++, x, "Console:");
    mvprintw(y++, x, "TODO");
}

void print_frame(void) {
    getmaxyx(stdscr, max_y, max_x);
    for (int i = 0; i < max_x; i++) {
        mvaddch(0, i, x_frame_character);
        mvaddch(max_y - 1, i, x_frame_character);
    }
    for (int i = 0; i < max_y; i++) {
        mvaddch(i, 0, y_frame_character);
        mvaddch(i, max_x - 1, y_frame_character);
    }
}

/*
void handle_log(const char *format, ...) {
    va_list args;
    va_start(args, format);

    va_end(args);

}
*/
void print_screen(Emulator *emulator, Config *config) {
    getmaxyx(stdscr, max_y, max_x);
    print_frame();
    print_emulator_status(emulator);
    print_memory(emulator);
    print_console();
    refresh();
}

const uint8_t ROM[] = {
    0b00001101, // MOVAIMM 1
    0b00000001,
    0b00001110, // MOVBIMM 67
    0b01000011,
    0b00011010, // ADDA
    0b00001110, // MOVBIMM 1
    0b00000001,
    0b00011010, // ADDA
    0b11111110, // SKIP (in emulator acts as print REG_A)
    0b00000010, // MOVBA
    0b00011010, // ADDA
    0b11111110, // SKIP
    0b00011010, // ADDA
    0b11111110, // SKIP
    0b00011010, // ADDA
    0b11111110, // SKIP
    0b00011101, // SUBABA
    0b11111110, // SKIP
    0b00011101, // SUBABA
    0b11111110, // SKIP
    0b00011101, // SUBABA
    0b11111110, // SKIP (in emulator acts as print REG_A)
    0b11111111, // HALT

};

int main(void) {
    // emulator setup
    Emulator emulator;
    Config config;
    const char *const filename = "instructions.json";
    load_config(&config, filename);
    // log_func = &handle_log;
    load_config_temp(&config);
    init_emulator(&emulator);
    // ncurses setup
    initscr();
    keypad(stdscr, TRUE);

#ifdef DEBUG
    print_config(&config);
#endif

    for (uint32_t i = 0; i < sizeof ROM; ++i) {
        emulator.memory[i] = ROM[i];
    }
    while (emulator.is_halted == 0 && emulator.program_counter < sizeof ROM) {
        run_next_emulator_instruction(&emulator, &config);
    }
    print_screen(&emulator, &config);
    cleanup_config(&config);
    getch();
    endwin();
    return 0;
}
