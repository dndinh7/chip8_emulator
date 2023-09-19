/**
 * This contains the data structure/memory of the emulator
*/

#ifndef _chip8_
#define _chip8_

#include <string>
#include <algorithm>
#include <iostream>
#include <stdio.h>

using namespace std;

unsigned char font[80] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Chip8 {
  unsigned short opcode; // 2 bytes
  unsigned char memory[4096]; // 4k byte memory
  unsigned char V[16]; // 0-E are used as general purpose registers, while the F register used as carry flag

  // Holds value up to 0xFFF (4096 bytes)
  unsigned short I; // index register 
  unsigned short pc; // program counter

  /*
  0x000 - 0x1FF - chip8 interpreter and font data
  0x050 - 0x0A0 - 4x5 pixel font set(0-F)
  0x200 - 0xFFF - Program ROM and work RAM
  */


  // Graphics of 2048 pixels (32 by 64 pixels)
  unsigned char gfx[32 * 64];

  unsigned char delay_timer; // 60 Hz, timer register that counts down to 0
  unsigned char sound_timer; // 60 Hz, timer register that counts down to 0

  unsigned short stack[16]; // stores program counter when opcode jumps
  unsigned short sp; // stack pointer

  unsigned char key[16]; // keypad

  void initialize();
  void loadGame(string game); // uses fopen to load the data in binary mode

  void emulateCycle(); 

};


#endif // chip8