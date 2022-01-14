#pragma once
#include<stdint.h>
#include<iostream>
class Chip8
{
public:
    //constructor
    Chip8();

private:
    //4kb of memory
    uint8_t mem[4 * 1024] = {0};

    //registers
    uint8_t V[16] = { 0 };
    uint8_t DT, ST;
    uint8_t SP;
    uint16_t I;
    uint16_t PC;

    //stack
    uint16_t stack[16];

    //instructions
    //not used
    void SYS(uint16_t addr);


    void CLS(); void RET(); void JP(uint16_t addr); void CALL(uint16_t addr); 
    void SE_IMM(uint8_t reg_idx, uint8_t imm); void SNE_IMM(uint8_t reg_idx, uint8_t imm); 
    void SE(uint8_t reg1_idx, uint8_t reg2_idx); void LD_IMM(uint8_t reg_idx, uint8_t imm);
    void ADD_IMM(uint8_t reg_idx, uint8_t imm);  void LD(uint8_t reg1_idx, uint8_t reg2_idx);
    void OR(uint8_t reg1_idx, uint8_t reg2_idx); void AND(uint8_t reg1_idx, uint8_t reg2_idx);
    void XOR(uint8_t reg1_idx, uint8_t reg2_idx); void ADD(uint8_t reg1_idx, uint8_t reg2_idx);
    void SUB(uint8_t reg1_idx, uint8_t reg2_idx); void SHR(uint8_t reg_idx); //Maybe also needs an optional param?
    void SUBN(uint8_t reg1_idx, uint8_t reg2_idx);  void SHL(uint8_t reg_idx); //Maybe also needs an optional param?
    void SNE(uint8_t reg1_idx, uint8_t reg2_idx); void LD_I(uint16_t addr);
    void JP_REL(uint16_t addr); void RND(uint8_t reg_idx, uint8_t val);
    void DRW(uint8_t reg1_idx, uint8_t reg2_idx, uint8_t nbytes);
    void SKP(uint8_t reg_idx); void SKNP(uint8_t reg_idx);
    void LD_DT(uint8_t reg_idx); void LD_K(uint8_t value); void SET_DT(uint8_t reg_idx);
    void SET_ST(uint8_t reg_idx); void ADD_I(uint8_t reg_idx); void SET_DIGIT(uint8_t reg_idx);
    void SET_BCD(uint8_t reg_idx); void STORE_REGS(uint8_t reg_idx); void LOAD_REGS(uint8_t reg_idx);


    //TODO: Set SP = 0 in init function


};

