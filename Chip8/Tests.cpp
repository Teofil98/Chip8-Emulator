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
            printf(" > JMP instruction doesn't set the PC counter correctly");
        }
    }

    //restore state
    PC = oldPC;

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

    V[0] = 0x80;

    SE_IMM(0, 0x12);

    if (PC != oldPC)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SE instruction with immediate value increments the PC when values are not equal.\n");
        }
        PC = oldPC;
    }

    SE_IMM(0, 0x80);

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

    V[0] = 0x80;

    SNE_IMM(0, 0x12);

    if (PC != (oldPC + 2))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SNE instruction with immediate value does not increment the PC correctly.\n");
        }
    }
    PC = oldPC;

    SNE_IMM(0, 0x80);

    if (PC != oldPC)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SNE instruction with immediate value increments the PC when the values are equal.\n");
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

    V[0xA] = 0x80;
    V[0xB] = 0x34;

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

    V[0xB] = 0x80;
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

bool Chip8::testLD_IMM(bool verbose)
{
    bool passed = true;

    //save state
    int8_t oldVx = V[2];

    LD_IMM(2, 0x21);

    if (V[2] != 0x21)
    {
        passed = false;
        if (verbose)
        {
            printf(" > LD instruction with immediate value doesn't set the correct value in the register\n");
        }
    }

    //restore state
    V[2] = oldVx;

    return passed;
}

bool Chip8::testADD_IMM(bool verbose)
{
    bool passed = true;

    //save state 
    uint8_t oldVF = V[0xF];
    uint8_t oldVx = V[5];

    V[5] = 0x32;
    ADD_IMM(5, 0x25);

    if (V[5] != (0x32 + 0x25))
    {
        passed = false;
        if (verbose)
        {
            printf(" > Add instruction doesn't update the register correctly.\n");
        }
    }

    V[5] = 0xFF;
    V[0xF] = 0;

    ADD_IMM(5, 0x10);

    if (V[0xF] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > Add instruction with immediate value shouldn't set the carry flag in case of overflow");
        }
    }

    //restore state
    V[5] = oldVx;
    V[0xF] = oldVF;

    return passed;
}

bool Chip8::testLD(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[3];
    uint8_t oldVy = V[0xC];

    V[3] = 0x89;
    V[0xC] = 0x21;

    LD(3, 0xC);

    if (V[3] != 0x21)
    {
        passed = false;
        if (verbose)
        {
            printf(" > Load instruction did not set the correct value");
        }
    }

    //restore state
    V[3] = oldVx;
    V[0xC] = oldVy;

    return passed;
}

bool Chip8::testOR(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[3];
    uint8_t oldVy = V[0xC];

    V[3] = 0xE9;
    V[0xC] = 0xF5;

    OR(3, 0xC);

    if (V[3] != (0xFD))
    {
        passed = false;
        printf(" OR instruction did not update the register correctly.\n");
    }

    //restore state
    V[3] = oldVx;
    V[0xC] = oldVy;

    return passed;

}

bool Chip8::testAND(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[3];
    uint8_t oldVy = V[0xC];

    V[3] = 0xE9;
    V[0xC] = 0xF5;

    AND(3, 0xC);

    if (V[3] != (0xE1))
    {
        passed = false;
        printf(" AND instruction did not update the register correctly.\n");
    }

    //restore state
    V[3] = oldVx;
    V[0xC] = oldVy;

    return passed;
}

bool Chip8::testXOR(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[3];
    uint8_t oldVy = V[0xC];

    V[3] = 0xE9;
    V[0xC] = 0xF5;

    XOR(3, 0xC);

    if (V[3] != (0x1C))
    {
        passed = false;
        printf(" XOR instruction did not update the register correctly.\n");
    }

    //restore state
    V[3] = oldVx;
    V[0xC] = oldVy;

    return passed;
}

bool Chip8::testADD(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[2];
    uint8_t oldVy = V[3];
    uint8_t oldVf = V[0x0F];


    V[2] = 0x23;
    V[3] = 0x12;
    V[0x0F] = 0;

    ADD(2, 3);

    if (V[2] != (0x35))
    {
        passed = false;
        if (verbose)
        {
            printf(" > ADD instruction doesn't set the correct value of VX.\n");
        }
    }

    if (V[0xF] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > ADD instruction sets the carry flag when it shouldn't.\n");
        }
    }

    V[2] = 0xFF;
    V[3] = 0x01;
    V[0x0F] = 0;

    ADD(2, 3);

    if (V[2] != (0x00))
    {
        passed = false;
        if (verbose)
        {
            printf(" > ADD instruction doesn't set the correct value of VX.\n");
        }
    }

    if (V[0xF] != 1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > ADD instruction doesn't set the carry flag .\n");
        }
    }

    //restore state
    V[2] = oldVx;
    V[3] = oldVy;
    V[0x0F] = oldVf;

    return passed;
}

bool Chip8::testSUB(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[2];
    uint8_t oldVy = V[3];
    uint8_t oldVf = V[0x0F];


    V[2] = 0x12;
    V[3] = 0x23;
    V[0x0F] = 0;

    SUB(2, 3);

    if (V[2] != (0xEF))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction doesn't set the correct value of VX.\n");
        }
    }

    if (V[0xF] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction sets the NOT borrow flag when it shouldn't.\n");
        }
    }

    V[2] = 0x23;
    V[3] = 0x12;
    V[0x0F] = 0;

    SUB(2, 3);

    if (V[2] != (0x11))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction doesn't set the correct value of VX.\n");
        }
    }

    if (V[0xF] != 1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction doesn't set the NOT borrow flag.\n");
        }
    }

    //restore state
    V[2] = oldVx;
    V[3] = oldVy;
    V[0x0F] = oldVf;

    return passed;
}

bool Chip8::testSHR(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[2];
    uint8_t oldVF = V[0x0F];

    V[2] = 0x42;
    V[0x0F] = 0;

    SHR(2);

    if (V[2] != 0x21)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHR doesn't set the correct value in Vx.\n");
        }
    }

    if (V[0x0F] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHR sets the VF flag when it shouldn't.\n");
        }
    }

    V[2] = 0xF5;
    V[0x0F] = 0;

    SHR(2);

    if (V[2] != 0x7A)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHR doesn't set the correct value in Vx.\n");
        }
    }

    if (V[0x0F] != 1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHR doesn't set the VF flag.\n");
        }
    }

    // restore state
    V[2] = oldVx;
    V[0x0F] = oldVF;

    return passed;
}

bool Chip8::testSUBN(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[2];
    uint8_t oldVy = V[3];
    uint8_t oldVf = V[0x0F];


    V[2] = 0x23;
    V[3] = 0x12;
    
    V[0x0F] = 0;

    SUBN(2, 3);

    if (V[2] != (0xEF))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction doesn't set the correct value of VX.\n");
        }
    }

    if (V[0xF] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction sets the NOT borrow flag when it shouldn't.\n");
        }
    }

    V[2] = 0x12;
    V[3] = 0x23;
    V[0x0F] = 0;

    SUBN(2, 3);

    if (V[2] != (0x11))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUB instruction doesn't set the correct value of VX.\n");
        }
    }

    if (V[0xF] != 1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SUBN instruction doesn't set the NOT borrow flag.\n");
        }
    }

    //restore state
    V[2] = oldVx;
    V[3] = oldVy;
    V[0x0F] = oldVf;

    return passed;
}

bool Chip8::testSHL(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[2];
    uint8_t oldVF = V[0x0F];

    V[2] = 0x42;
    V[0x0F] = 0;

    SHL(2);

    if (V[2] != 0x84)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHL doesn't set the correct value in Vx.\n");
        }
    }

    if (V[0x0F] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHL sets the VF flag when it shouldn't.\n");
        }
    }

    V[2] = 0xF5;
    V[0x0F] = 0;

    SHL(2);

    if (V[2] != 0xEA)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHL doesn't set the correct value in Vx.\n");
        }
    }

    if (V[0x0F] != 1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SHL doesn't set the VF flag.\n");
        }
    }

    // restore state
    V[2] = oldVx;
    V[0x0F] = oldVF;

    return passed;
}

bool Chip8::testSNE(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;
    uint8_t oldVx = V[0];
    uint8_t oldVy = V[1];

    V[0] = 0x80;
    V[1] = 0x90;

    SNE(0, 1);

    if (PC != (oldPC + 2))
    {
        passed = false;
        if (verbose)
        {
            printf(" > SNE instruction does not increment the PC correctly.\n");
        }
    }

    PC = oldPC;
    V[1] = 0x80;

    SNE(0, 1);

    if (PC != oldPC)
    {
        passed = false;
        if (verbose)
        {
            printf(" > SNE instruction increments the PC when the values are equal.\n");
        }
    }

    PC = oldPC;
    V[0] = oldVx;
    V[1] = oldVy;

    return passed;
}

bool Chip8::testLD_I(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldI = I;
    
    I = 0;
 
    LD_I(0x0647);

    if (I != 0x0647)
    {
        passed = false;
        if (verbose)
        {
            printf(" > Load value into I instruction does not set correct value for I.\n");
        }
    }

    //restore state
    I = oldI;

    return passed;
}

bool Chip8::testJMP_REL(bool verbose)
{
    bool passed = true;

    //save state
    uint16_t oldPC = PC;
    uint8_t oldV0 = V[0];

    V[0] = 0x13;
    uint16_t addr = 0x05B1;

    JMP_REL(addr);

    if (PC != 0x5C4)
    {
        passed = false;
        if (verbose)
        {
            printf(" > JMP instruction relative doesn't set the PC counter correctly.\n");
        }
    }


    //restore state
    PC = oldPC;
    V[0] = oldV0;

    return passed;
}

bool Chip8::testRND(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[4];

    uint8_t mask = 0xFF;

    RND(4, mask);
    uint8_t rand1 = V[4];

    RND(4, mask);
    uint8_t rand2 = V[4];

    if (rand1 == rand2)
    {
        passed = false;
        if (verbose)
        {
            printf(" > RND instruction doesn't generate 2 different numbers.\n");
        }
    }

    mask = 0x00;

    RND(4, mask);
    rand1 = V[4];

    if (rand1 != 0x00)
    {
        passed = false;
        if (verbose)
        {
            printf(" > RND instruction doesn't AND with the argument correctly.\n");
        }
    }

    //restore state
    V[4] = oldVx;

    return passed;
}

bool Chip8::testSET_DIGIT(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[0];
    uint16_t oldI = I;

    V[0] = 0x0;
    SET_DIGIT(0);

    if (I != 0x0000)
    {
        passed = false;
        if (verbose)
        {
            printf(" > Load digit sprite address into I doesn't set the correct value\n");
        }
    }

    V[0] = 0xB;
    SET_DIGIT(0);

    if (I != 0x0037)
    {
        passed = false;
        if (verbose)
        {
            printf(" > Load digit sprite address into I doesn't set the correct value\n");
        }
    }

    //restore state
    I = oldI;
    V[0] = oldVx;

    return passed;
}

bool Chip8::testDRW(bool verbose)
{
    bool passed = true;

    //save state
    uint8_t oldVx = V[0];
    uint8_t oldVy = V[1];
    uint16_t oldI = I;

    V[0] = 0X7;

    SET_DIGIT(0);

    //set coords to fit at byte allignment

    //column
    V[0] = 32;
    //row
    V[1] = 16;

    CLS();

   
    DRW(0, 1, 5);
    if (verbose)
    {
        printf("Draw a sprite on the screen\n");
        printScreen();
    }

    if (V[0xF] != 0)
    {
        passed = false;
        if (verbose)
        {
            printf(" > DRW instruction sets overlap flag when it shouldn't\n");
        }
    }

    //set another digit to test overlap
    V[0] = 0xA;
    SET_DIGIT(0);

    //column 
    V[0] = 34;
    //row
    V[1] = 14;

    
    DRW(0, 1, 5);
    if (verbose)
    {
        printf("Draw anoter sprite to check for overlap\n");
        printScreen();
    }
    
    if (V[0xF] != 1)
    {
        passed = false;
        if (verbose)
        {
            printf(" > DRW instruction doesn't set overlap flag\n");
        }
    }

    CLS();

    //test wraparound on rows
    V[0] = 0x4;
    SET_DIGIT(0);

    //column
    V[0] = 32;
    //row
    V[1] = 30;

   
    DRW(0, 1, 5);

    if (verbose)
    {
        printf("Test wraparound on rows.\n");
        printScreen();
    }
    //test wraparound on columns
    CLS();
    //column
    V[0] = 62;
    //row
    V[1] = 16;
   
    DRW(0, 1, 5);

    if (verbose)
    {
        printf("Test wraparound on columns.\n");
        printScreen();
    }

    //test wraparound on rows and columns
    CLS();
    //column
    V[0] = 62;
    //row
    V[1] = 30;

    
    DRW(0, 1, 5);

    if (verbose)
    {
        printf("Test wraparound on rows and columns columns.\n");
        printScreen();
    }

    CLS();

    //restore state
    V[0] = oldVx;
    V[1] = oldVy;
    I = oldI;

    return passed;
}

bool Chip8::runInstructionTests(bool verbose = false)
{
    const uint8_t totalTests = 35;
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

    if (testLD_IMM(verbose))
    {
        printf("[PASS] LD {6xkk} - load immediate instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] LD {6xkk} - load immediate instruction test.\n");
    }

    if (testADD_IMM(verbose))
    {
        printf("[PASS] ADD {7xkk} - add immediate instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] LD {7xkk} - add immediate instruction test.\n");
    }

    if (testLD(verbose))
    {
        printf("[PASS] LD {8xy0} - load instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] LD {8xy0} - load instruction test.\n");
    }

    if (testOR(verbose))
    {
        printf("[PASS] OR {8xy1} - or instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] OR {8xy1} - or instruction test.\n");
    }

    if (testAND(verbose))
    {
        printf("[PASS] AND {8xy2} - and instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] AND {8xy2} - and instruction test.\n");
    }

    if (testXOR(verbose))
    {
        printf("[PASS] XOR {8xy3} - xor instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] XOR {8xy3} - xor instruction test.\n");
    }

    if (testADD(verbose))
    {
        printf("[PASS] ADD {8xy4} - add instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] ADD {8xy4} - add instruction test.\n");
    }

    if (testSUB(verbose))
    {
        printf("[PASS] SUB {8xy5} - sub instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SUB {8xy5} - sub instruction test.\n");
    }

    if (testSHR(verbose))
    {
        printf("[PASS] SHR {8xy6} - shift right instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SHR {8xy6} - shift right instruction test.\n");
    }

    if (testSUBN(verbose))
    {
        printf("[PASS] SUBN {8xy7} - subn instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SUBN {8xy7} - subn instruction test.\n");
    }

    if (testSHL(verbose))
    {
        printf("[PASS] SHL {8xyE} - shift left instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SHL {8xyE} - shift left instruction test.\n");
    }

    if (testSNE(verbose))
    {
        printf("[PASS] SNE {9xy0} - skip if not equal instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] SNE {9xy0} - skip if not equal instruction test.\n");
    }

    if (testLD_I(verbose))
    {
        printf("[PASS] LD {Annn} - Load into I instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL] LD {Annn} - Load into I instruction test.\n");
    }

    if (testJMP_REL(verbose))
    {
        printf("[PASS] JMP {Bnnn} - Jump relative instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL]  JMP {Bnnn} - Jump relative instruction test.\n");
    }

    if (testRND(verbose))
    {
        printf("[PASS] RND {Cxkk} - 'random' instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL]  RND {Cxkk} - 'random' instruction test.\n");
    }

    if (testDRW(verbose))
    {
        printf("[PASS] DRW {Dxyn} - draw instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL]  DRW {Dxyn} - draw  instruction test.\n");
    }

    if (testSET_DIGIT(verbose))
    {
        printf("[PASS] LD {Fx33} - load digit sprite address into I instruction test.\n");
        testsPassed++;
    }
    else {
        printf("[FAIL]  LD {Fx33} - load digit sprite address into I instruction test.\n");
    }

   
    printf("Tests passed: %d/%d\n", testsPassed, totalTests);

    return (testsPassed == totalTests);
}