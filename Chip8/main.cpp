#include <stdio.h>
#include <stdlib.h>
#include "Chip8.h"


int main()
{

    Chip8 chip8 = Chip8();

    chip8.runInstructionTests(true);

    return 0;
}