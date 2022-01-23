#include <iostream>
#include <fstream>
#include "Disassembler.h"



char* Disassembler::disassembleInstruction(uint16_t instruction)
{
    //decode instruction
    uint8_t opcode = (instruction & 0xF000) >> 12;
    uint8_t tail_opcode = (instruction & 0x000F);
    uint8_t x = (instruction & 0x0F00) >> 8;
    uint8_t y = (instruction & 0x00F0) >> 4;
    uint16_t nnn = (instruction & 0x0FFF);
    uint8_t high_byte = (instruction & 0xFF00) >> 8;
    uint8_t low_byte = (instruction & 0x00FF);

    char* str_instruction = (char*)malloc(20 * sizeof(char));

    if (str_instruction == NULL)
    {
        printf("Could not allocate memory for intrusction string.\n");
        return NULL;
    }

    //execute instruction
    switch (opcode)
    {
    case 0:
        if (instruction == 0x0000)
        {
            sprintf(str_instruction, "NOP");
        }
        else if (instruction == 0x00E0)
        {
            sprintf(str_instruction, "CLS");
        }
        else if (instruction == 0x00EE)
        {
            sprintf(str_instruction,"RET");
        }
        else {
            sprintf(str_instruction,"SYS 0x%X", nnn);
        }
        break;
    case 1:
        sprintf(str_instruction, "JMP 0x%X", nnn);
        break;
    case 2:
        sprintf(str_instruction, "CALL 0x%X", nnn);
        break;
    case 3:
        sprintf(str_instruction, "SE V%X, 0x%X", x, low_byte);
        break;
    case 4:
        sprintf(str_instruction, "SNE V%X, 0x%X", x, low_byte);
        break;
    case 5:
        if (tail_opcode == 0)
        {
            sprintf(str_instruction, "SE V%X, V%X", x, y);;
        }
        break;
    case 6:
        sprintf(str_instruction, "LD V%X, 0x%X", x, low_byte);
        break;
    case 7:
        sprintf(str_instruction, "ADD V%X, 0x%X", x, low_byte);
        break;
    case 8:
        if (tail_opcode == 0)
        {
            sprintf(str_instruction, "LD V%X, V%X", x, y);
        }
        else if (tail_opcode == 1)
        {
            sprintf(str_instruction, "OR V%X, V%X", x, y);
        }
        else if (tail_opcode == 2)
        {
            sprintf(str_instruction, "AND V%X, V%X", x, y);
        }
        else if (tail_opcode == 3)
        {
            sprintf(str_instruction, "XOR V%X, V%X", x, y);
        }
        else if (tail_opcode == 4)
        {
            sprintf(str_instruction, "ADD V%X, V%X", x, y);
        }
        else if (tail_opcode == 5)
        {
            sprintf(str_instruction, "SUB V%X, V%X", x, y);
        }
        else if (tail_opcode == 6)
        {
            sprintf(str_instruction, "SHR V%X, {V%X}", x, y);
        }
        else if (tail_opcode == 7)
        {
            sprintf(str_instruction, "SUBN V%X, {V%X}", x, y);
        }
        else if (tail_opcode == 0xE)
        {
            sprintf(str_instruction, "SHL V%X, {V%X}", x, y);
        }
        break;

    case 9:
        if (tail_opcode == 0)
        {
            sprintf(str_instruction, "SNE V%X, V%X", x, y);
        }
        break;
    case 0xA:
        sprintf(str_instruction, "LD I, 0x%X", nnn);
        break;
    case 0xB:
        sprintf(str_instruction, "JMP V0, 0x%X", nnn);
        break;
    case 0xC:
        sprintf(str_instruction, "RND V%x, 0x%X", x, low_byte);
        break;
    case 0xD:
        sprintf(str_instruction, "DRW V%x, V%x, 0x%X", x, y, tail_opcode);
        break;
    case 0xE:
        if (low_byte == 0x9E)
        {
            sprintf(str_instruction, "SKP V%x",x);
        }
        else if (low_byte == 0xA1)
        {
            sprintf(str_instruction, "SKNP V%x", x);
        }
        break;
    case 0xF:
        if (low_byte == 0x07)
        {
            sprintf(str_instruction, "LD V%x, DT", x);
        }
        else if (low_byte == 0x0A)
        {
            sprintf(str_instruction, "SKP V%x, K", x);
        }
        else if (low_byte == 0x15)
        {
            sprintf(str_instruction, "LD DT, V%x", x);
        }
        else if (low_byte == 0x18)
        {
            sprintf(str_instruction, "LD ST, V%x", x);
        }
        else if (low_byte == 0x1E)
        {
            sprintf(str_instruction, "ADD I, V%x", x);
        }
        else if (low_byte == 0x29)
        {
            sprintf(str_instruction, "LD F, V%x", x);
        }
        else if (low_byte == 0x33)
        {
            sprintf(str_instruction, "LD B, V%x", x);
        }
        else if (low_byte == 0x55)
        {
            sprintf(str_instruction, "LD [I], V%x", x);
        }
        else if (low_byte == 0x65)
        {
            sprintf(str_instruction, "LD V%x, [I]", x);
        }
        break;


    default:
        break;
    }

    return str_instruction;
}

void Disassembler::disassembleFile(const char* filename, const char* disassembled_filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    std::ofstream dissassembled_file;
    dissassembled_file.open(disassembled_filename, std::ios::out);

    if (!file)
    {
        printf("Error! The file does not exist!\n");
        return;
    }

    if (!dissassembled_file)
    {
        printf("Error! Could not create file!\n");
        return;
    }

    //get file length
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    for (uint32_t i = 0; i < length; i += 2)
    {
        Chip8::instruction instr;
        //read instruction
        file.read((char*)(&instr.bytes[1]), 1);
        file.read((char*)(&instr.bytes[0]), 1);

        //disassemble instruction and write to file

        char* disassembled_instr = disassembleInstruction(instr.instr);
        dissassembled_file.write(disassembled_instr, strlen(disassembled_instr));
        dissassembled_file << "\n";
        free(disassembled_instr);
    }
    
    file.close();
    dissassembled_file.close();
}
