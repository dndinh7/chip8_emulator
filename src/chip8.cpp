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
  for (int i= 0; i < 80; i++) {
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
    // 0 leading 4 bit
    case 0x0000:
      switch(opcode & 0x000F) {
        // clears the screen
        case 0x0000:
          fill(begin(gfx), end(gfx), 0);
          pc+= 2;
          break;
        
        // returns from subroutine
        case 0x000E:
          break;

      }
    // jump to address 0x0NNN
    case 0x1000:
      pc= opcode & 0x0FFF;
      break;
    // subroutine call
    case 0x2000:
      stack[sp]= pc;
      sp++;
      pc = opcode & 0x0FFF;
      break;
    // 0x3XNN, if V[X] equals NN, then skip next instruction 
    case 0x3000:
      char X= opcode & 0x0F00 >> 8;
      if (V[X] == opcode & 0x00FF) pc+= 2;
      pc+= 2;
      break;
    // 0x4XNN, if V[X] does not equal NN, then skip next instruction
    case 0x4000:
      char reg= opcode & 0x0F00 >> 8;
      if (V[X] != opcode & 0x00FF) pc+= 2;
      pc+= 2;
      break;
    // 0x5XY0, if V[X] equals V[Y], then skip next instruction
    case 0x5000:
      char X= opcode & 0x0F00 >> 8;
      char Y= opcode & 0x00F0;
      if (V[X] == V[Y]) pc+= 2;
      pc+= 2;
      break;
    // 0x6XNN, V[X]= NN
    case 0x6000:
      char X= opcode & 0x0F00 >> 8;
      V[X]= opcode & 0x00FF;
      pc+= 2;
      break;
    // 0x7XNN, V[X]+= NN (carry flag not affected) 
    case 0x7000:
      char X= opcode & 0x0F00 >> 8;
      V[X]+= opcode & 0x00FF;
      break;
    // 0x8XY*, X and Y are registers V[X] and V[Y]
    case 0x8000:
      switch (opcode & 0x000F) {
        char X= opcode & 0x0F00 >> 8;
        char Y= opcode & 0x00F0 >> 4;

        // set V[X] = V[Y]
        case 0x0000:
          V[X]= V[Y];
          break;
        // set V[X] = V[X] | V[Y]
        case 0x0001:
          V[X]|= V[Y];
          break;
        // set V[X] = V[X] | V[Y]
        case 0x0002:
          V[X]&= V[Y];
          break;
        // set V[X] = V[X] ^ V[Y] 
        case 0x0003:
          V[X]^= V[Y];
          break;
        // set V[X] = V[X] + V[Y]
        case 0x0004:

          // carry if there is overflow
          if (V[X] > 0XFF - V[Y]) {
            V[0xF]= 1;
          } else {
            V[0xF]= 0;
          }
          V[X]+= V[Y];
          break;
        // set V[X] = V[X] - V[Y]
        case 0x0005:
          // if borrow, then set last flag to 0
          if (V[X] < V[Y]) {
            V[0xF]= 0;
          } else {
            V[0xF]= 1;
          }
          V[X]-= V[Y];

          break;
        // set V[X]= V[X] >> 1 
        case 0x0006:
          // store least significant bit into V[15]
          V[0xF]= V[X] & 0x0F;

          V[X]>>= 1;
          break;
        // set V[X]= V[Y] - V[X]
        case 0x0007:
          // if borrow, then set last flag to 0
          if (V[X] > V[Y]) {
            V[0xF]= 0;
          } else {
            V[0xF]= 1;
          }
          V[X]= V[Y] - V[X];

          break;
        // set V[X]= V[X] << 1
        case 0x000E:
          // store least significant bit into V[15]
          V[0xF]= V[X] & 0x80;

          V[X]<<= 1;
          break;
      }
      pc+= 2;
      break;
    // if V[X] != V[Y], skips next instruction
    case 0x9000:
      char X= opcode & 0x0F00 >> 8;
      char Y= opcode & 0x00F0 >> 4;

      if (V[X] != V[Y]) pc+= 2;

      pc+= 2;

      break;
    // set index register to be 12 last bit address
    case 0xA000:
      I= opcode & 0x0FFF;
      pc+= 2;
      break;
    // 0xBNNN, jumps to address NNN + V[0]
    case 0xB000:
      pc= (opcode & 0x0FFF) + V[0]; 
      break;
    // 0xCXNN, sets V[X] equal to random number and NN
    case 0xC000:
      V[X]= rand() & (opcode & 0x00FF);
      pc+= 2;
      break;
    case 0xD000:
      break;
    case 0xE000:
      break;
    case 0xF000:
      break;

    default:
      cout << "Unknown opcode: " << opcode << endl;
      break;


    
  }

  if (delay_timer > 0) --delay_timer;

  if (sound_timer > 0)
  {
    if (sound_timer == 1) cout << "BEEP!" << endl;
    --sound_timer;
  }


  // update timers
}