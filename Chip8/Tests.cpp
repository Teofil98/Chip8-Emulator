#include "Chip8.h"


bool Chip8::testCLS(bool verbose)
{
    //set some values
    screen[0][0] = 0x12;
    screen[4][5] = 0xFF;
    screen[31][7] = 0x13;

    //clear screem
    CLS();

    for (uint8_t i = 0; i < 32; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            if (screen[i][j] != 0x00)
            {
                if (verbose)
                {
                    printf(" > CLS instruction does not clear all the bits from the screen\n");
                }
                return false;
            }
        }
    }

    return true;
}

bool Chip8::testRET(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldSP;
    uint16_t oldStackVal, oldPC;
    oldSP = SP;
    oldStackVal = stack[5];
    oldPC = PC;

    stack[4] = 0x80FD;
    SP = 5;

    RET();

    if (SP != 4)
    {
        passed = false;

        if (verbose)
        {
            printf(" > RET instruction does not update the stack pointer correctly.\n");
        }
    }

    if (PC != 0x80FD)
    {
        passed = false;

        if (verbose)
        {
            printf(" > RET instruction does not set the PC to the correct value.\n");
        }
    }

    //restore state
    SP = oldSP;
    PC = oldPC;
    stack[5] = oldStackVal;

    return passed;
}

bool Chip8::testJMP(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;

    uint16_t addr = 0x05B1;

    JMP(addr);

    if (PC != 0x5B1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > JMP instructioon doesn't set the PC counter correctly");
        }
    }

    return passed;
}

bool Chip8::testCALL(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;
    uint8_t oldSP = SP;

    SP = 2;
    PC = 0x0421;

    CALL(0x0FE2);

    if (PC != 0x0FE2)
    {
        passed = false;
        if (verbose)
        {
            printf(" > CALL instruction does not set the value of PC correctly.\n");
        }
    }

    if (SP != 3)
    {
        passed = false;
        if (verbose)
        {
            printf(" > CALL instruction does not set the value of SP correctly.\n");
        }
    }

    if (stack[(SP - 1)] != 0x0421)
    {
        passed = false;
        if (verbose)
        {
            printf(" > CALL instruction does not save the value of PC correctly.\n");
        }
    }

    //restore state
    SP = oldSP;
    PC = oldPC;

    return passed;
}

bool Chip8::testSE_IMM(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;
    uint8_t oldVx = V[0];

    V[0] = 0xFE80;

    SE_IMM(0, 0x1212);

    if (PC != oldPC)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SE instruction with immediate value increments the PC when values are not equal.\n");
        }
        PC = oldPC;
    }

    SE_IMM(0, 0xFE80);

    if (PC != (oldPC + 2))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SE instruction with immediate value does not increment the PC correctly.\n");
        }
    }

    PC = oldPC;
    V[0] = oldVx;

    return passed;
}

bool Chip8::testSNE_IMM(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;
    uint8_t oldVx = V[0];

    V[0] = 0xFE80;

    SNE_IMM(0, 0x1212);

    if (PC != (oldPC + 2))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SNE instruction with immediate value does not increment the PC correctly.\n");
        }
    }
    PC = oldPC;

    SNE_IMM(0, 0xFE80);

    if (PC != oldPC)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SNE instruction with immediate value increments the PC when the values are not equal.\n");
        }
    }

    PC = oldPC;
    V[0] = oldVx;

    return passed;
}

bool Chip8::testSE(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;
    uint8_t oldVx = V[0xA];
    uint8_t oldVy = V[0xB];

    V[0xA] = 0xFE80;
    V[0xB] = 0x1234;

    SE(0xA, 0xB);

    if (PC != oldPC)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SE instruction increments the PC when values are not equal.\n");
        }
        PC = oldPC;
    }

    V[0xB] = 0xFE80;
    SE(0xA, 0xB);

    if (PC != (oldPC + 2))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SE instruction does not increment the PC correctly.\n");
        }
    }

    PC = oldPC;
    V[0xA] = oldVx;
    V[0xB] = oldVy;

    return passed;
}

bool Chip8::runInstructionTests(bool verbose = false)
{
    const uint8_t totalTests = 7;
    uint8_t testsPassed = 0;

    if (testCLS(verbose))
    {
        printf("[PASS] CLS {00E0} - clear screen instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] CLS {00E0} - clear screen instruction test");
    }

    if (testRET(verbose))
    {
        printf("[PASS] RET {00EE} - return instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] RET {00EE} - return instruction test.\n");
    }

    if (testJMP(verbose))
    {
        printf("[PASS] JMP {1nnn} - jump instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] JMP {1nnn} - jump instruction test.\n");
    }

    if (testCALL(verbose))
    {
        printf("[PASS] CALL {2nnn} - call instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] CALL {2nnn} - call instruction test.\n");
    }

    if (testSE_IMM(verbose))
    {
        printf("[PASS] SE {3xkk} - skip if equal instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SE {3xkk} - skip if equal instruction test.\n");
    }

    if (testSNE_IMM(verbose))
    {
        printf("[PASS] SNE {4xkk} - skip if not equal instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SNE {4xkk} - skip if not equal instruction test.\n");
    }

    if (testSE(verbose))
    {
        printf("[PASS] SE {5xy0} - skip if not equal instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SE {5xy0} - skip if not equal instruction test.\n");
    }

    printf("Tests passed: %d/%d\n", testsPassed, totalTests);

    return (testsPassed == totalTests);
}