#include "cli_app_utils.h"
#include "emulator.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define x_frame_character '#'
#define y_frame_character '|'

typedef enum { HEX, DEC, DEC_SIGNED, INST, ASCII } Memory_view_mode;

LogVector default_log_vector;
static int y, x, max_y, max_x;
static uint8_t memory_tables_count = 2;
static Memory_view_mode current_memory_view_mode[2] = {ASCII, HEX};

void print_emulator_status(Emulator *emulator) {
    int def_x = max_x - 30, def_y = 2;
    x = def_x;
    y = def_y;
    mvprintw(y++, x, "Emulator Registers:");
    mvprintw(y++, x, "A: signed: %d unsigned: %u", emulator->signed_a_register, emulator->a_register);
    mvprintw(y++, x, "B: signed: %d unsigned: %u", emulator->signed_b_register, emulator->b_register);
    mvprintw(y++, x, "tmp: %u", emulator->tmp_register_16);
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
    char *buffer = NULL;
    while (get_log(&default_log_vector, &buffer) != -1) {
        mvprintw(y++, x + 1, "%s", buffer);
    }
    free(buffer);
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

void print_screen(Emulator *emulator, Config *config) {
    clear();
    getmaxyx(stdscr, max_y, max_x);
    print_frame();
    print_emulator_status(emulator);
    print_memory(emulator);
    print_console();
    refresh();
}

int main(int argc, char **argv) {
    int cmd_opt;
    uint8_t *rom;
    unsigned rom_size;
    char *filename = NULL;
    char *rom_filename = NULL;
    init_log_vector(&default_log_vector, 7);
    while ((cmd_opt = getopt(argc, argv, "hi:f:")) != -1) {
        switch (cmd_opt) {
        case 'h':
            printf("Usage: %s -i [FILE] -f [FILE]\n", argv[0]);
            printf("Emulator of KN Breadboard Computing's custom 8-Bit CPU\n");
            printf("Options:\n");
            printf("-h\tShow this help message\n");
            printf("-i\tSpecify path to instruction set json\n");
            printf("-f\tSpecify path to ROM file\n");
            return 0;
        case 'i':
            console_log(DEBUG, "Instruction set file: %s", optarg);
            filename = malloc(strlen(optarg) + 1);
            strcpy(filename, optarg);
            break;
        case 'f':
            console_log(DEBUG, "ROM file: %s", optarg);
            rom_filename = malloc(strlen(optarg) + 1);
            strcpy(rom_filename, optarg);
            break;
        case ':':
            printf("option needs a value\n");
            return 0;
        default:
            printf("unknown option: %c\n", cmd_opt);
            return 0;
        }
    }
    // emulator setup
    log_func = &console_log;
    Emulator emulator;
    Config config;
    if (filename == NULL) {
        console_log(WARNING, "No instruction set file specified, using default filename\n");
        filename = malloc(strlen("instructions.json") + 1);
        strcpy(filename, "instructions.json");
    }
    // log_func = &console_log
    if (!load_rom(&rom, &rom_size, rom_filename)) {
        console_log(ERROR, "could not load rom\n");
        goto end;
    }
    int ret = load_config(&config, filename);
    if (ret != 0) {
        console_log(ERROR, "could not load config err: %d\n", ret);
        goto end;
    }

    init_emulator(&emulator);
    // ncurses setup
    initscr();
    keypad(stdscr, TRUE);

#ifdef DBG
    print_config(&config);
#endif

    for (uint32_t i = 0; i < rom_size; ++i) {
        emulator.memory[i] = rom[i];
    }
    while (emulator.is_halted == 0 && emulator.program_counter < rom_size) {
        if (run_next_emulator_instruction(&emulator, &config) != 0) {
            break;
        }
        print_screen(&emulator, &config);
        usleep(100000);
    }
    console_log(NONE, "End of Execution (press any button to exit)");
    print_screen(&emulator, &config);
end:
    cleanup_config(&config);
    free_log_vector(&default_log_vector);
    free(rom_filename);
    free(filename);
    free(rom);
    getch();
    endwin();
    return 0;
}
