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
    //set display to 0
    memset(screen, 0x00, 32 * 8);
}

//WARNING: Changed spec, if something doesn't work, check here 
void Chip8::RET()
{
    if (SP > 0)
    {
        PC = stack[--SP];
    }
}

void Chip8::JMP(uint16_t addr)
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

    bool wrap_around = false;
    if (V[reg1_idx] + 8 > 63)
    {
        wrap_around = true;
    }

    V[0x0F] = 0;
    for (uint8_t i = 0; i < nbytes; i++)
    {
        //read byte at address specified in I
        uint8_t byte = mem[I + i];

        if (curr_row > 31)
        {
            curr_row = 0;
        }

        //get current byte at this position on screen

        uint8_t draw_byte_high;
        uint8_t draw_byte_low;

        uint8_t offset = curr_col % 8;
        draw_byte_high = byte >> offset;
        draw_byte_low = byte << (8 - offset);

        //if we have to wrap around, byte to the right of written value is on column 0
        uint8_t screen_low_byte_col;

        if (wrap_around)
        {
            screen_low_byte_col = 0;
        }
        else {
            screen_low_byte_col = curr_col / 8 + (offset > 0 ? 1 : 0);
        }

        uint8_t overlap_byte_high = screen[curr_row][curr_col / 8];
        uint8_t overlap_byte_low = screen[curr_row][screen_low_byte_col];

        //determine if bits are going to be flipped when we XOR and set flag
        if ((draw_byte_high & overlap_byte_high) || (draw_byte_low & overlap_byte_low))
        {
            V[0x0F] = 1;
        }

        //XOR the values
        screen[curr_row][curr_col / 8] = overlap_byte_high ^ draw_byte_high;
        screen[curr_row][screen_low_byte_col] = overlap_byte_low ^ draw_byte_low;
    }

}

void Chip8::SKP(uint8_t reg_idx)
{
    if (V[reg_idx] == getPressedKey())
    {
        PC += 2;
    }
}

void Chip8::SKNP(uint8_t reg_idx)
{
    if (V[reg_idx] != getPressedKey())
    {
        PC += 2;
    }
}

void Chip8::LD_DT(uint8_t reg_idx)
{
    V[reg_idx] = DT;
}

void Chip8::LD_KEY(uint8_t reg_idx)
{
    V[reg_idx] = requestKeyPress();
}

void Chip8::SET_DT(uint8_t reg_idx)
{
    DT = V[reg_idx];
}

void Chip8::SET_ST(uint8_t reg_idx)
{
    ST = V[reg_idx];
}

void Chip8::ADD_I(uint8_t reg_idx)
{
    I += V[reg_idx];
}

void Chip8::SET_DIGIT(uint8_t reg_idx)
{
    //set I to location of sprite for digit in V[reg_idx]
    I = V[reg_idx] * 5;
}

void Chip8::SET_BCD(uint8_t reg_idx)
{
    uint8_t aux = V[reg_idx];

    //ones place
    mem[I + 2] = aux % 10;
    aux /= 10;

    //tens place
    mem[I + 1] = aux % 10;
    aux /= 10;

    //hundreds place
    mem[I] = aux;
}

void Chip8::STORE_REGS(uint8_t reg_idx)
{
    for (uint8_t i = 0; i < reg_idx; i++)
    {
        mem[I + i] = V[i];
    }
}

void Chip8::LOAD_REGS(uint8_t reg_idx)
{
    for (uint8_t i = 0; i < reg_idx; i++)
    {
        V[i] = mem[I + i];
    }
}

uint8_t Chip8::getPressedKey()
{
    //TODO: Implement
    return 0;
}

uint8_t Chip8::requestKeyPress()
{
    //TODO: Implement
    return 0;
}

