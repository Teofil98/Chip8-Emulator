#include "Chip8.h"

Chip8::Chip8()
{
    DT = 0;
    ST = 0;
    SP = 0;

    srand(time(NULL));
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

void Chip8::SE(uint8_t reg1_idx, uint8_t reg2_idx)
{
    if (V[reg1_idx] == V[reg2_idx])
    {
        PC += 2;
    }
}

void Chip8::LD_IMM(uint8_t reg_idx, uint8_t imm)
{
    V[reg_idx] = imm;
}

void Chip8::ADD_IMM(uint8_t reg_idx, uint8_t imm)
{
    V[reg_idx] += imm;
}

void Chip8::LD(uint8_t reg1_idx, uint8_t reg2_idx)
{
    V[reg1_idx] = V[reg2_idx];
}

void Chip8::OR(uint8_t reg1_idx, uint8_t reg2_idx)
{
    V[reg1_idx] |= V[reg2_idx];
}

void Chip8::AND(uint8_t reg1_idx, uint8_t reg2_idx)
{
    V[reg1_idx] &= V[reg2_idx];
}

void Chip8::XOR(uint8_t reg1_idx, uint8_t reg2_idx)
{
    V[reg1_idx] ^= V[reg2_idx];
}

void Chip8::ADD(uint8_t reg1_idx, uint8_t reg2_idx)
{
    if ((uint16_t)V[reg1_idx] + V[reg2_idx] > 0xFF)
    {
        V[0x0F] = 1;
    }
    else {
        V[0x0F] = 0;
    }

    V[reg1_idx] += V[reg2_idx];
}

void Chip8::SUB(uint8_t reg1_idx, uint8_t reg2_idx)
{
    if (V[reg1_idx] > V[reg2_idx])
    {
        V[0x0F] = 1;
    }
    else {
        V[0x0F] = 0;
    }

    V[reg1_idx] -= V[reg2_idx];
}

void Chip8::SHR(uint8_t reg_idx)
{

    if (V[reg_idx] & 0x01)
    {
        V[0x0F] = 1;
    }
    else {
        V[0x0F] = 0;
    }

    V[reg_idx] >>= 1;
}

void Chip8::SUBN(uint8_t reg1_idx, uint8_t reg2_idx)
{
    if (V[reg2_idx] > V[reg1_idx])
    {
        V[0x0F] = 1;
    }
    else {
        V[0x0F] = 0;
    }

    V[reg1_idx] = V[reg2_idx] - V[reg1_idx];
}

void Chip8::SHL(uint8_t reg_idx)
{
    if (V[reg_idx] & 0x80)
    {
        V[0x0F] = 1;
    }
    else {
        V[0x0F] = 0;
    }

    V[reg_idx] <<= 2;
}

void Chip8::SNE(uint8_t reg1_idx, uint8_t reg2_idx)
{
    if (V[reg1_idx] != V[reg2_idx])
    {
        PC += 2;
    }
}

void Chip8::LD_I(uint16_t addr)
{
    I = addr;
}

void Chip8::JP_REL(uint16_t addr)
{
    PC = addr + V[0];
}

void Chip8::RND(uint8_t reg_idx, uint8_t val)
{
    V[reg_idx] = (rand() % 256) & val;
}

void Chip8::DRW(uint8_t reg1_idx, uint8_t reg2_idx, uint8_t nbytes)
{
    //Vx column, Vy row
    uint8_t curr_col = V[reg1_idx];
    uint8_t curr_row = V[reg2_idx];

    uint8_t x_lelftover = 0;
    if (V[reg1_idx] + 8 > 63)
    {
       x_lelftover = (V[reg1_idx] + 8) - 63;
    }

    for (uint8_t i = 0; i < nbytes; i++)
    {
        //uint8_t x_lelftover = 0;
        //uint8_t y_lelftover = 0;
        //read byte at address specified in I
        uint8_t byte = mem[I + i];

        if (curr_row > 31)
        {
            curr_row = 0;
        }

        if (x_lelftover > 0)
        {

        }
        else {
            //get current byte at this position on screen

            uint8_t draw_byte_high;
            uint8_t draw_byte_low;

            uint8_t offset = curr_col % 8;
            draw_byte_high = byte >> offset;
            draw_byte_low = byte << (8 - offset);

            uint8_t overlap_byte_high = screen[curr_row][curr_col / 8];
            uint8_t overlap_byte_low = screen[curr_row][curr_col / 8 + offset > 0 ? 1 : 0];


        }

        ////check if sprite is outsite on the x axis
        //if (V[reg1_idx] + 8 > 63)
        //{
        //    x_lelftover = (V[reg1_idx] + 8) - 63;
        //}

        ////check if sprite is outsite on the y axis
        //if (V[reg2_idx] + nbytes > 31)
        //{
        //    y_lelftover = (V[reg2_idx] + nbytes) - 31;
        //}

        ////draw non-colision lines
        //for (uint8_t line = V[reg2_idx]; line < (V[reg2_idx] + nbytes - y_lelftover); line++)
        //{
        //    //draw lines 

        //}
    }
}

int main()
{

    //TODO: Put main in appropriate place

    return 0;
}