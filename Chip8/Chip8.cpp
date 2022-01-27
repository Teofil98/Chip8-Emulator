#include "Chip8.h"
#include <fstream>
#include <random>

Chip8::Chip8()
{
    //start of Chip-8 programs
    PC = CHIP8_PROGRAM_START;

    //set up random number generation
    distribution = std::uniform_int_distribution<int>(0, 255);
    generator.seed(time(NULL));
   
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
    //Online specification says that carry flag is NOT changed
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

    V[reg_idx] <<= 1;
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

void Chip8::JMP_REL(uint16_t addr)
{
    PC = addr + V[0];
}

void Chip8::RND(uint8_t reg_idx, uint8_t val)
{
    uint8_t rnd = distribution(generator);  // generates number in the range 0...255
   // printf("Generated random number: 0x%X\n", rnd);
    V[reg_idx] = (uint8_t) (rnd & val);
}

void Chip8::DRW(uint8_t reg1_idx, uint8_t reg2_idx, uint8_t nbytes)
{
    //Vx column, Vy row
    uint8_t curr_col = V[reg1_idx];
    uint8_t curr_row = V[reg2_idx];

    bool collison = false;
    bool wrap_around = false;
    if (V[reg1_idx] + 8 > 63)
    {
        wrap_around = true;
    }

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
            collison = true;
        }

        //XOR the values
        screen[curr_row][curr_col / 8] = overlap_byte_high ^ draw_byte_high;

        if (offset != 0)
        {
            screen[curr_row][screen_low_byte_col] = overlap_byte_low ^ draw_byte_low;
        }

        curr_row++;
    }

    if (collison)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0x0F] = 0;
    }

}

//draw without wraparound 
void Chip8::DRW_ALT(uint8_t reg1_idx, uint8_t reg2_idx, uint8_t nbytes)
{
    //Vx column, Vy row
    uint8_t curr_col = V[reg1_idx] % 64;
    uint8_t curr_row = V[reg2_idx] % 32;

    bool collison = false;
    bool wrap_around = false;
    if (V[reg1_idx] + 8 > 63)
    {
        wrap_around = true;
    }

    for (uint8_t i = 0; i < nbytes; i++)
    {
        //read byte at address specified in I
        uint8_t byte = mem[I + i];

        if (curr_row > 31)
        {
            break;
        }

        //get current byte at this position on screen

        uint8_t draw_byte_high;
        uint8_t draw_byte_low;

        uint8_t offset = curr_col % 8;
        draw_byte_high = byte >> offset;
        draw_byte_low = byte << (8 - offset);

        //if we have to wrap around, byte to the right of written value is on column 0
        uint8_t screen_low_byte_col;

        
        screen_low_byte_col = curr_col / 8 + (offset > 0 ? 1 : 0);
        

        uint8_t overlap_byte_high = screen[curr_row][curr_col / 8];
        uint8_t overlap_byte_low = 0;
        if (!wrap_around)
        {
            overlap_byte_low = screen[curr_row][screen_low_byte_col];
        }

        //determine if bits are going to be flipped when we XOR and set flag
        if ((draw_byte_high & overlap_byte_high) || (draw_byte_low & overlap_byte_low))
        {
            collison = true;
        }

        //XOR the values
        screen[curr_row][curr_col / 8] = overlap_byte_high ^ draw_byte_high;

        if (offset != 0 && !wrap_around)
        {
            screen[curr_row][screen_low_byte_col] = overlap_byte_low ^ draw_byte_low;
        }

        curr_row++;
    }

    if (collison)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0x0F] = 0;
    }

}

void Chip8::SKP(uint8_t reg_idx)
{
    if (keyEvent == KEY_PRESSED_EVENT && V[reg_idx] == pressedKey)
    {
        PC += 2;
    }
}

void Chip8::SKNP(uint8_t reg_idx)
{
    if (keyEvent != NO_KEY_EVENT && V[reg_idx] != pressedKey || keyEvent == NO_KEY_EVENT)
    {
        PC += 2;
    }
}

void Chip8::LD_DT(uint8_t reg_idx)
{
    V[reg_idx] = DT;
}

bool Chip8::LD_KEY(uint8_t reg_idx)
{
    // only on release so as not to bleed key press into next frame
    if (keyEvent == KEY_RELEASED_EVENT)
    {
        V[reg_idx] = pressedKey;
        return true;
    }

    return false;
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
    for (uint8_t i = 0; i <= reg_idx; i++)
    {
        mem[I + i] = V[i];
    }
}

void Chip8::LOAD_REGS(uint8_t reg_idx)
{
    for (uint8_t i = 0; i <= reg_idx; i++)
    {
        V[i] = mem[I + i];
    }
}

char* byteToString(uint8_t byte)
{
    char* str_rep = (char*)malloc(9 * sizeof(char));

    if (str_rep == NULL)
    {
        printf("Error allocating string representation of screent");
        return NULL;
    }

    for (int i = 0; i < 8; i++)
    {
        str_rep[i] = ((byte & 0x80) >> 7) == 1 ? '*' : ' ';
        byte <<= 1;
    }
    str_rep[8] = 0;

    return str_rep;
}

void Chip8::printScreen()
{
    for (int i = 0; i < SCREEN_H; i++)
    {
        for (int j = 0; j < SCREEN_W; j++)
        {
            char* s = byteToString(screen[i][j]);
            //printf("0x%X ", screen[i][j]);
            printf("%s", s);
            free(s);
        }
        printf("\n");
    }
}

void Chip8::decrementTimers()
{
    if (ST > 0)
    {
        ST--;
    }

    if (DT > 0)
    {
        DT--;
    }
}

void Chip8::loadROM(const char* path)
{
    std::ifstream rom (path, std::ios::in | std::ios::binary);
   
    if (!rom)
    {
        printf("Error! The ROM does not exist!\n");
        return;
    }

    //get rom length
    rom.seekg(0, std::ios::end);
    size_t length = rom.tellg();
    rom.seekg(0, std::ios::beg);

    //printf("Read rom of length %u\n", length);
    //read rom in memory
    rom.read((char*)(mem + CHIP8_PROGRAM_START), length);
    //printf("Read %d bytes!\n",rom.gcount());

    //print memory
    /*for (int i = CHIP8_PROGRAM_START - 0x2; i < CHIP8_PROGRAM_START + 0x10; i++)
    {
        printf("[0x%X] 0x%X, ",i,  mem[i]);
    }
    printf("\n");*/

    rom.close();
}

void Chip8::clock()
{
    instruction i;
    bool incrementPC = true;
    //fetch instruction
    //my CPU is little endian
    i.bytes[1] = mem[PC];
    i.bytes[0] = mem[PC + 1];
    //decode instruction
    uint8_t opcode = (i.instr & 0xF000) >> 12;
    uint8_t tail_opcode = (i.instr & 0x000F);
    uint8_t x = (i.instr & 0x0F00) >> 8;
    uint8_t y = (i.instr & 0x00F0) >> 4;
    uint16_t nnn = (i.instr & 0x0FFF);

   // printf("instruction: 0x%X, byte[0] = 0x%X, byte[1] = 0x%X, PC = 0x%X\n", i.instr, i.bytes[0],i.bytes[1],PC);

    updatedScreen = false;

    //execute instruction
    switch (opcode)
    {
    case 0:
        if (i.instr == 0x00E0)
        {
            CLS();
            updatedScreen = true;
        }
        else if (i.instr == 0x00EE)
        {
            RET();
        }
        else {
            SYS(nnn);
        }
        break;
    case 1:
        JMP(nnn);
        incrementPC = false;
        break;
    case 2:
        CALL(nnn);
        incrementPC = false;
        break;
    case 3:
        SE_IMM(x, i.bytes[0]);
        break;
    case 4:
        SNE_IMM(x, i.bytes[0]);
        break;
    case 5:
        if (tail_opcode == 0)
        {
            SE(x, y);
        }
        break;
    case 6:
        LD_IMM(x, i.bytes[0]);
        break;
    case 7:
        ADD_IMM(x, i.bytes[0]);
        break;
    case 8:
        if (tail_opcode == 0)
        {
            LD(x, y);
        }
        else if (tail_opcode == 1)
        {
            OR(x, y);
        }
        else if (tail_opcode == 2)
        {
            AND(x, y);
        }
        else if (tail_opcode == 3)
        {
            XOR(x, y);
        }
        else if (tail_opcode == 4)
        {
            ADD(x, y);
        }
        else if (tail_opcode == 5)
        {
            SUB(x, y);
        }
        else if (tail_opcode == 6)
        {
            SHR(x);
        }
        else if (tail_opcode == 7)
        {
            SUBN(x, y);
        }
        else if (tail_opcode == 0xE)
        {
            SHL(x);
        }
        break;

    case 9:
        if (tail_opcode == 0)
        {
            SNE(x, y);
        }
        break;
    case 0xA:
        LD_I(nnn);
        break;
    case 0xB:
        JMP_REL(nnn);
        incrementPC = false;
        break;
    case 0xC:
        RND(x, i.bytes[0]);
        break;
    case 0xD:
        DRW_ALT(x, y, tail_opcode);
        updatedScreen = true;
        break;
    case 0xE:
        if (i.bytes[0] == 0x9E)
        {
            SKP(x);
        }
        else if (i.bytes[0] == 0xA1)
        {
            SKNP(x);
        }
        break;
    case 0xF:
        if (i.bytes[0] == 0x07)
        {
            LD_DT(x);
        } else if (i.bytes[0] == 0x0A)
        {
            incrementPC = LD_KEY(x);
        } else if (i.bytes[0] == 0x15)
        {
            SET_DT(x);
        } else if (i.bytes[0] == 0x18)
        {
            SET_ST(x);
        } else if (i.bytes[0] == 0x1E)
        {
            ADD_I(x);
        } else if (i.bytes[0] == 0x29)
        {
            SET_DIGIT(x);
        } else if (i.bytes[0] == 0x33)
        {
            SET_BCD(x);
        } else if (i.bytes[0] == 0x55)
        {
            STORE_REGS(x);
        } else if (i.bytes[0] == 0x65)
        {
            LOAD_REGS(x);
        }
        break;


    default:
        break;
    }

    if (incrementPC)
    {
        PC += 2;
    }
}