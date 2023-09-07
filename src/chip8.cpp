/**
 * This file implements the methods of the chip8 class in
 * chip8.h
*/
#include "chip8.h"

void Chip8::initialize()
{
  pc= 0x200;
  opcode= 0;
  I= 0;
  sp= 0;

  // clear memory
  fill(begin(memory), end(memory), 0);

  // clear stack
  fill(begin(stack), end(stack), 0);

  // clear registers
  fill(begin(V), end(V), 0);

  // clear display
  fill(begin(gfx), end(gfx), 0);

  // init the font size from 0x50 to 0x09F
  for (int i= 0; i < 80; ++i) {
    memory[0x50+i]= font[i];
  }

  delay_timer= 0;
  sound_timer= 0;
}

void Chip8::emulateCycle()
{
  // fetching the opcode
  opcode= memory[pc] << 8 | memory[pc + 1];

  // decode opcode

  // get the most significant 4 bits for instruction
  switch (opcode & 0xF000) {
    // set index register to be 12 last bit address
    case 0xA000:
      I= opcode & 0x0FFF;
      pc+= 2;
      break;
    default:
      cout << "Unknown opcode: " << opcode << endl;
      break;
  }


  // update timers
}