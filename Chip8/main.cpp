#include <stdio.h>
#include <stdlib.h>
#include "Chip8.h"
#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

#define TIMER_CLOCK_FREQ 60
#define CHIP8_CLOCK_FREQ 500


class Chip8Emulator : public olc::PixelGameEngine
{
public:
    Chip8Emulator()
    {
        sAppName = "Chip-8 Emulator";
    }

    Chip8 chip8  = Chip8();


    bool OnUserCreate() override
    {
       // chip8.loadROM("roms/test_opcode.ch8");
       // chip8.loadROM("roms/Paddles.ch8");
        //chip8.loadROM("roms/Tetris.ch8");
       // chip8.loadROM("roms/Breakout.ch8");
        chip8.loadROM("roms/Pong.ch8");
        //chip8.loadROM("roms/Random_Number_Test.ch8");
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        frameTotalTime += fElapsedTime;
        timerTotalTime += fElapsedTime;
        //set the state of the keyboard asynch with the clock of the chip
        setKeyboardState();

        if (timerTotalTime >= timerTotalTime / TIMER_CLOCK_FREQ)
        {
            timerTotalTime = 0;
            chip8.decrementTimers();
        }

        if (frameTotalTime >= 1.0 / CHIP8_CLOCK_FREQ)
        {
            frameTotalTime = 0;

            //update CPU state
            chip8.clock();

            //draw a frame
            Clear(olc::BLACK);
            drawFrame();
            //system("cls");
            //chip8.printScreen();
            



            //reset button states (will be set again next emulator frame if needed)
            chip8.keyEvent = NO_KEY_EVENT;
        }

        return true;
    }

private:
    float frameTotalTime = 0;
    float timerTotalTime = 0;

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
                   /* else {
                        Draw(j * 8 + k, i, olc::BLACK);
                    }*/
                    byte_to_draw <<= 1;
                }
            }
        }
    }

};

int main()
{
    Chip8Emulator emulator;
    if(emulator.Construct(64,32,16,16))
        emulator.Start();

    return 0;
}