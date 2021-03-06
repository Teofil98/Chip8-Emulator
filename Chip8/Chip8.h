#pragma once
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<random>

#define SCREEN_W 8
#define SCREEN_H 32

#define NO_KEY_EVENT 0
#define KEY_RELEASED_EVENT 1
#define KEY_PRESSED_EVENT 2
//#define CHIP8_PROGRAM_START 0x600
#define CHIP8_PROGRAM_START 0x200


class Chip8
{
public:
    //constructor
    Chip8();

    union instruction
    {
        uint16_t instr;
        uint8_t bytes[2];

    };

    uint8_t keyEvent = NO_KEY_EVENT;
    uint8_t pressedKey;
    bool updatedScreen = false;
    
    //64 = 8 * 8
    uint8_t screen[32][8];

    void loadROM(const char* path);
    void clock();
    void decrementTimers();

    bool runInstructionTests(bool verbose);
    void printScreen();


    //4kb of memory
    //Set the first 5 * 16 bytes in the interpreter memory (0x000 -> 0x1FF) to be the digit sprites
    

    //registers
    uint8_t V[16] = { 0 };
    uint8_t DT = 0; 
    uint8_t ST = 0;;
    uint8_t SP = 0;
    uint16_t I = 0;
    uint16_t PC;

    //stack
    uint16_t stack[16];

    uint8_t mem[4 * 1024] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0XF0, 0x90, 0xF0, 0x10, 0x10, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F

    };


   
private:

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

    //instructions
    //not used
    void SYS(uint16_t addr);


    void CLS(); void RET(); void JMP(uint16_t addr); void CALL(uint16_t addr); 
    void SE_IMM(uint8_t reg_idx, uint8_t imm); void SNE_IMM(uint8_t reg_idx, uint8_t imm); 
    void SE(uint8_t reg1_idx, uint8_t reg2_idx); void LD_IMM(uint8_t reg_idx, uint8_t imm);
    void ADD_IMM(uint8_t reg_idx, uint8_t imm);  void LD(uint8_t reg1_idx, uint8_t reg2_idx);
    void OR(uint8_t reg1_idx, uint8_t reg2_idx); void AND(uint8_t reg1_idx, uint8_t reg2_idx);
    void XOR(uint8_t reg1_idx, uint8_t reg2_idx); void ADD(uint8_t reg1_idx, uint8_t reg2_idx);
    void SUB(uint8_t reg1_idx, uint8_t reg2_idx); void SHR(uint8_t reg_idx); //Maybe also needs an optional param?
    void SUBN(uint8_t reg1_idx, uint8_t reg2_idx);  void SHL(uint8_t reg_idx); //Maybe also needs an optional param?
    void SNE(uint8_t reg1_idx, uint8_t reg2_idx); void LD_I(uint16_t addr);
    void JMP_REL(uint16_t addr); void RND(uint8_t reg_idx, uint8_t val);
    void DRW(uint8_t reg1_idx, uint8_t reg2_idx, uint8_t nbytes);
    void DRW_ALT(uint8_t reg1_idx, uint8_t reg2_idx, uint8_t nbytes);
    void SKP(uint8_t reg_idx); void SKNP(uint8_t reg_idx);
    void LD_DT(uint8_t reg_idx); bool LD_KEY(uint8_t reg_idx); void SET_DT(uint8_t reg_idx);
    void SET_ST(uint8_t reg_idx); void ADD_I(uint8_t reg_idx); void SET_DIGIT(uint8_t reg_idx);
    void SET_BCD(uint8_t reg_idx); void STORE_REGS(uint8_t reg_idx); void LOAD_REGS(uint8_t reg_idx);

    //Tests
    bool testCLS(bool verbose);
    bool testRET(bool verbose);
    bool testJMP(bool verbose);
    bool testCALL(bool verbose);
    bool testSE_IMM(bool verbose); 
    bool testSNE_IMM(bool verbose);
    bool testSE(bool verbose);
    bool testLD_IMM(bool verbose);
    bool testADD_IMM(bool verbose);
    bool testLD(bool verbose);
    bool testOR(bool verbose);
    bool testAND(bool verbose);
    bool testXOR(bool verbose);
    bool testADD(bool verbose);
    bool testSUB(bool verbose);
    bool testSHR(bool verbose);
    bool testSUBN(bool verbose);
    bool testSHL(bool verbose);
    bool testSNE(bool verbose);
    bool testLD_I(bool verbose);
    bool testJMP_REL(bool verbose); 
    bool testRND(bool verbose);
    bool testDRW(bool verbose);
    bool testSKP(bool verbose); 
    bool testSKNP(bool verbose);
    bool testLD_DT(bool verbose); 
    bool testLD_KEY(bool verbose); 
    bool testSET_DT(bool verbose);
    bool testSET_ST(bool verbose); 
    bool testADD_I(bool verbose);
    bool testSET_DIGIT(bool verbose);
    bool testSET_BCD(bool verbose); 
    bool testSTORE_REGS(bool verbose);
    bool testLOAD_REGS(bool verbose);

    
};

