#pragma once
#include<stdint.h>
#include<stdlib.h>
#include "Chip8.h"
class Disassembler
{
public:

    Disassembler(Chip8 c8)
    {
        chip8 = c8;
    }

    char* disassembleInstruction(uint16_t instruction);
    void disassembleFile(const char* filename, const char* disassembled_filename);

private:
    Chip8 chip8;
};

