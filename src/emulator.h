#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#define STACK_SIZE 65536
#define MEMORY_SIZE 65536
typedef enum {
    MOVAB,
    MOVBA,
    MOVAF,
    MOVBF,
    MOVATH,
    MOVBTH,
    MOVATL,
    MOVBTL,
    MOVTHA,
    MOVTHB,
    MOVTLA,
    MOVTLB,
    MOVAIMM,
    MOVBIMM,
    MOVAABS,
    MOVBABS,
    MOVABSA,
    MOVABSB,
    MOVABSIMM,
    NEGAA,
    NEGAB,
    NEGAMEM,
    NEGBA,
    NEGBB,
    NEGBMEM,
    ADDA,
    ADDB,
    ADDMEM,
    SUBABA,
    SUBABB,
    SUBABMEM,
    SUBBAA,
    SUBBAB,
    SUBBAMEM,
    MULA2A,
    MULA2B,
    MULA2MEM,
    MULB2A,
    MULB2B,
    MULB2MEM,
    DIVA2A,
    DIVA2B,
    DIVA2MEM,
    DIVB2A,
    DIVB2B,
    DIVB2MEM,
    INVAA,
    INVAB,
    INVAMEM,
    INVBA,
    INVBB,
    INVBMEM,
    ORA,
    ORB,
    ORMEM,
    ANDA,
    ANDB,
    ANDMEM,
    XORA,
    XORB,
    XORMEM,
    SHRAA,
    SHRAB,
    SHRAMEM,
    SHRBA,
    SHRBB,
    SHRBMEM,
    SHLAA,
    SHLAB,
    SHLAMEM,
    SHLBA,
    SHLBB,
    SHLBMEM,
    CMPAB,
    CMPBA,
    CMPTHTL,
    CMPTTLT,
    CMPATH,
    CMPTHA,
    CMPATL,
    CMPTLA,
    CMPBTH,
    CMPTHB,
    CMPBTL,
    CMPTLB,
    JMPIMM,
    JMPS,
    JMPNS,
    JMPP,
    JMPNP,
    JMPZ,
    JIMPNZ,
    JMPNC,
    JMPO,
    JMPNO,
    JMPFUN,
    JMPRET,
    PUSHA,
    PUSHB,
    PUSHTH,
    PUSHTL,
    PUSHIMM,
    PUSHABS,
    POPA,
    POPB,
    POPTH,
    POPTL,
    POPMEM,
    NOP,
    SKIP,
    HALT
} Instruction;

typedef struct {
    //nwm czy to top 10 eleganckich rozwiazan te unie
    union {
        uint8_t a_register;
        int8_t signed_a_register;
    };
    union {
        uint8_t b_register;
        int8_t signed_b_register;
    };
    uint8_t flag_register;
    uint16_t tmp_register;
    uint16_t stack_pointer;
    uint16_t program_counter;
    bool is_halted;

    uint8_t memory[MEMORY_SIZE];
    uint8_t stack[STACK_SIZE];

    //for debug purposes
    uint32_t instruction_counter;
    uint32_t clock_cycles_counter;
} Emulator;

void init_emulator(Emulator *emulator);

int run_next_emulator_instruction(Emulator *emulator);

int run_instruction(Emulator *emulator, Instruction instruction);

Instruction decode_instruction(uint8_t instruction_bytecode);

