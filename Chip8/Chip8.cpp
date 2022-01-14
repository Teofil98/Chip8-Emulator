#include "Chip8.h"

Chip8::Chip8()
{
    DT = 0;
    ST = 0;
    SP = 0;
}

void Chip8::SYS(uint16_t addr)
{
    //not used
}

void Chip8::CLS()
{
    //TODO: Clear the display once graphics are working
}

//WARNING: Changed spec, if something doesn't work, check here 
void Chip8::RET()
{
    if (SP > 0)
    {
        PC = stack[--SP];
    }
}

void Chip8::JP(uint16_t addr)
{
    PC = addr;
}

//WARNING: Changed spec, if something doesn't work, check here 
void Chip8::CALL(uint16_t addr)
{
    if (SP < 16)
    {
        stack[SP++] = PC;
        PC = addr;
    }
}

void Chip8::SE_IMM(uint8_t reg_idx, uint8_t imm)
{
    //reg_idx already guaranteed to be < 16, no bounds checks needed
    if (V[reg_idx] == imm)
    {
        PC += 2;
    }
}

void Chip8::SNE_IMM(uint8_t reg_idx, uint8_t imm)
{
    if (V[reg_idx] != imm)
    {
        PC += 2;
    }
}

int main()
{

    //TODO: Put main in appropriate place

    return 0;
}