#include <stdio.h>
#include <stdlib.h>
#include "Chip8.h"
#include "Disassembler.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define TIMER_CLOCK_FREQ 60
#define CHIP8_CLOCK_FREQ 700

#define DEBUGGER_MODE false

class Chip8Emulator : public olc::PixelGameEngine
{
public:
    Chip8Emulator(int screen_width, int screen_height, int pixel_width, int pixel_height, int debugger_width, int debugger_height)
    {
        screen_w = screen_width;
        screen_h = screen_height;
        pixel_w = pixel_width;
        pixel_h = pixel_height;
        debugger_w = debugger_width;
        debugger_h = debugger_height;


        sAppName = "Chip-8 Emulator";
    }

    Chip8 chip8  = Chip8();
    Disassembler disassembler = Disassembler(chip8);

    int screen_w;
    int screen_h;
    int pixel_w;
    int pixel_h;

    int debugger_w;
    int debugger_h;

    float secondpassed = 0;
    int instr_exec = 0;

    bool OnUserCreate() override
    {
       // chip8.loadROM("roms/test_opcode.ch8");
       // chip8.loadROM("roms/Paddles.ch8");
       // chip8.loadROM("roms/IBM Logo.ch8");
        chip8.loadROM("roms/Clock_Program.ch8");
       // chip8.loadROM("roms/Breakout.ch8");
       //chip8.loadROM("roms/Astro_Dodge_Hires.ch8");
      // chip8.loadROM("roms/Space_Invaders.ch8");
        //chip8.loadROM("roms/SQRT_Test.ch8");
      // chip8.loadROM("roms/Pong.ch8");
       // chip8.loadROM("roms/Random_Number_Test.ch8");
       // chip8.loadROM("roms/tetris_mod.ch8");
       // chip8.loadROM("roms/tetris.ch8");

        //disassembler.disassembleFile("roms/Astro_Dodge_Hires.ch8", "disassembled_Astro_Dodge_Hires.txt");

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        frameTotalTime += fElapsedTime;
        timerTotalTime += fElapsedTime;
        //set the state of the keyboard asynch with the clock of the chip
        setKeyboardState();

        //check if step to next instruction requested (only in debugger mode)
        if (DEBUGGER_MODE && (GetKey(olc::SPACE).bReleased || GetKey(olc::V).bHeld))
        {
            stepNextInstruction = true;
        }

        if (timerTotalTime >=  1.0 /  TIMER_CLOCK_FREQ)
        {
            //TODO: Double check timers, may have bug 
            timerTotalTime = 0;
            chip8.decrementTimers();
        }

        if (frameTotalTime >= 1.0 / CHIP8_CLOCK_FREQ)
        {

            secondpassed += frameTotalTime;

            

            frameTotalTime = 0;

             
            if (!DEBUGGER_MODE || stepNextInstruction)
            {
                //update CPU state
                chip8.clock();

                //log how many instructions executed per second
                /*instr_exec++;
                if (secondpassed > 1.0)
                {
                    printf("1 seconds has passed: executed %d instructions\n",instr_exec);
                    secondpassed = 0;
                    instr_exec = 0;
                }*/

                if (chip8.updatedScreen)
                {
                    //draw a frame
                    Clear(olc::BLACK);
                    drawFrame();
                }

            }
            
            if (DEBUGGER_MODE && stepNextInstruction)
            {
               // drawDebugger();
                system("cls");
                printCPUState();
            }

            
            if (!DEBUGGER_MODE || stepNextInstruction)
            {
                //reset button states (will be set again next frame if needed)
                chip8.keyEvent = NO_KEY_EVENT;
                chip8.pressedKey = 0x10; //i.e. not a valid key

                stepNextInstruction = false;
            }
           
        }

        return true;
    }

private:
    float frameTotalTime = 0;
    float timerTotalTime = 0;
    bool stepNextInstruction = false;
    

    struct KeyMap
    {
        olc::Key kbrd;
        uint8_t hexGrid;
    };

    KeyMap keyMap[16] =
    {
        {olc::Key::K1, 0x1}, {olc::Key::K2, 0x2}, {olc::Key::K3, 0x3}, {olc::Key::K4, 0xC},
        {olc::Key::Q, 0x4},  {olc::Key::W, 0x5},  {olc::Key::E, 0x6},  {olc::Key::R, 0xD},
        {olc::Key::A, 0x7},  {olc::Key::S, 0x8},  {olc::Key::D, 0x9},  {olc::Key::F, 0xE},
        {olc::Key::Z, 0xA},  {olc::Key::X, 0x0},  {olc::Key::C, 0xB},  {olc::Key::F, 0xF}
    };

    void setKeyboardState()
    {
        for (int i = 0; i < 16; i++)
        {
            if (GetKey(keyMap[i].kbrd).bHeld)
            {
                chip8.keyEvent = KEY_PRESSED_EVENT;
                chip8.pressedKey = keyMap[i].hexGrid;
                break;
            }
            else if (GetKey(keyMap[i].kbrd).bReleased)
            {
                chip8.keyEvent = KEY_RELEASED_EVENT;
                chip8.pressedKey = keyMap[i].hexGrid;
                break;
            }
        }
    }

    void drawRegisters()
    {
        DrawString(screen_w + 4, 4, std::string("V1: 0x1234"));
    }

    void drawDebugger()
    {
        //draw frame around region debugger
        //left line
        DrawLine(screen_w + 2, 2, screen_w + 2, screen_h + debugger_h - 2, olc::YELLOW);

        //right line
        DrawLine(screen_w + debugger_w - 2, 2, screen_w + debugger_w - 2, 
            screen_h + debugger_h - 2, olc::YELLOW);

        //top line
        DrawLine(screen_w + 2, 2, screen_w + debugger_w -2, 2, olc::YELLOW);

        //bottom line
        DrawLine(screen_w + 2, screen_h + debugger_h - 2, screen_w + debugger_w - 2,
            screen_h + debugger_h - 2, olc::YELLOW);

         drawRegisters();
        //drawInstuctions();

        
    }


    void drawFrame()
    {
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                uint8_t byte_to_draw = chip8.screen[i][j];
                    
                //draw on screen based on bytes
                for (int k = 0; k < 8; k++)
                {
                    if (((byte_to_draw & 0x80) >> 7) == 1)
                    {
                        Draw(j * 8 + k, i);  
                    }
                   
                    byte_to_draw <<= 1;
                }
            }
        }
    }

    void printCPUState()
    {
        //print registers
        for (uint8_t i = 0; i <= 0xF; i++)
        {
            printf("V%X: 0x%X  ", i, chip8.V[i]);
            if ((i + 1) % 4 == 0)
                printf("\n");
        }
        printf("I: 0x%X ", chip8.I);
        printf("SP: 0x%X ", chip8.SP);
        printf("ST: 0x%X ", chip8.ST);
        printf("DT: 0x%X ", chip8.DT);
        printf("PC: 0x%X ", chip8.PC);
        printf("\n\n");

        //print instructions
        for (uint16_t i = chip8.PC - 2 * 5; i <= chip8.PC + 2 * 5; i += 2)
        {
            if (i == chip8.PC)
                std::cout << "--> ";
            Chip8::instruction instr;
            instr.bytes[1] = chip8.mem[i];
            instr.bytes[0] = chip8.mem[i  + 1];
            
            std::cout << disassembler.disassembleInstruction(instr.instr) << std::endl;
        }

    }


};

int main()
{
    
    Chip8Emulator emulator(64, 32, 16, 16, 24, 16);

    int extra_w = 0;
    int extra_h = 0;
    if (DEBUGGER_MODE)
    {
        extra_w = emulator.debugger_w;
        extra_h = emulator.debugger_h;
    }

    if (emulator.Construct(emulator.screen_w + extra_w, emulator.screen_h + extra_h,
        emulator.pixel_w, emulator.pixel_h))
    {
        emulator.Start();
    }
            

    return 0;
}