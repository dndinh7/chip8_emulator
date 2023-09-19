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
        
        // returns from subroutine, next instruction from stack
        case 0x000E:
          pc= stack[sp];
          sp--;
          break;
      }
    // jump to address 0x0NNN
    case 0x1000:
      pc= opcode & 0x0FFF;
      break;
    // subroutine call, store the next instruction in stack
    case 0x2000:
      stack[sp]= pc+2;
      sp++;
      pc = opcode & 0x0FFF;
      break;
    // 0x3XNN, if V[X] equals NN, then skip next instruction 
    case 0x3000:
      unsigned char X= (opcode & 0x0F00) >> 8;
      if (V[X] == opcode & 0x00FF) pc+= 4;
      else pc+= 2;
      break;
    // 0x4XNN, if V[X] does not equal NN, then skip next instruction
    case 0x4000:
      char reg= (opcode & 0x0F00) >> 8;
      if (V[X] != opcode & 0x00FF) pc+= 4;
      else pc+= 2;
      break;
    // 0x5XY0, if V[X] equals V[Y], then skip next instruction
    case 0x5000:
      unsigned char X= (opcode & 0x0F00) >> 8;
      unsigned char Y= (opcode & 0x00F0);
      if (V[X] == V[Y]) pc+= 4;
      else pc+= 2;
      break;
    // 0x6XNN, V[X]= NN
    case 0x6000:
      unsigned char X= (opcode & 0x0F00) >> 8;
      V[X]= opcode & 0x00FF;
      pc+= 2;
      break;
    // 0x7XNN, V[X]+= NN (carry flag not affected) 
    case 0x7000:
      unsigned char X= (opcode & 0x0F00) >> 8;
      V[X]+= opcode & 0x00FF;
      break;
    // 0x8XY*, X and Y are registers V[X] and V[Y]
    case 0x8000:
      switch (opcode & 0x000F) {
        unsigned char X= (opcode & 0x0F00) >> 8;
        unsigned char Y= (opcode & 0x00F0) >> 4;

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
      unsigned char X= (opcode & 0x0F00) >> 8;
      unsigned char Y= (opcode & 0x00F0) >> 4;

      if (V[X] != V[Y]) pc+= 4;
      else pc+= 2;

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
    // 0xDXYN, displays the sprite
    case 0xD000:
      unsigned char X= (opcode & 0x0F00) >> 8;
      unsigned char Y= (opcode & 0x00F0) >> 4;
      unsigned char height= (opcode & 0x000F);
      unsigned char pixel;

      // reset flag
      V[0xF]= 0;

      for (int i= 0; i < height; i++) {
        pixel= memory[I + i];
        for (int j= 0; j < 8; j++) {
          // if pixel is 0, then the display will stay the same
          if (pixel & (0x80 >> j) == 1) {
            // make sure the pixel is on screen by modulating
            if (gfx[(V[X] + i) % 32 * 64 + (V[Y] + j) % 64] == 1) {
              V[0xF]= 1; // this essentially notes collision
            }
            gfx[(V[X] + i) % 32 * 64 + (V[Y] + j) % 64]^= 1;
          }
        }
      }
      pc+= 2;

      break;
    case 0xE000:
      unsigned char X= (opcode & 0x0F00) >> 8;
      switch (opcode & 0x000F) {
        // 0xEX9E, skips next instruction if key at V[X] is pressed 
        case 0x009E:
          if (key[V[X]] != 0) pc+= 4;
          else pc+= 2;

          break;
        // 0xEXA1, skips next instruction if key at V[X] is not pressed 
        case 0x00A1:
          if (key[V[X]] == 0) pc+= 4;
          else pc+= 2;

          break;
      }
      break;
    case 0xF000:
      switch (opcode & 0x00FF) {
        // 0xFX07, sets V[X] equal to delay timer
        case 0x0007:
          unsigned char X= (opcode & 0x0F00) >> 8;
          V[X]= delay_timer;
          break;
        // 0xFX0A, key press is awaited, then stored in V[X] (all operations stop until key is pressed) 
        case 0x000A:
          break;
        // 0xFX15, sets delay timer to V[X]
        case 0x0015:
          unsigned char X= (opcode & 0x0F00) >> 8;
          delay_timer= V[X];
          break;
        // 0xFX15, sets sound timer to V[X]
        case 0x0018:
          unsigned char X= (opcode & 0x0F00) >> 8;
          sound_timer= V[X];
          break;
        // 0xFX1E, sets I= I + V[X]
        case 0x001E:
          unsigned char X= (opcode & 0x0F00) >> 8;
          I+= V[X];
          break;
        case 0x0029:
          break;
        // 0xFX33, store the binary-coded decimal of V[X] into memory at I (hundreds at I, tens at I+1, and ones at I+2)
        case 0x0033:
          unsigned char X= (opcode & 0x0F00) >> 8;
          memory[I]= V[X] / 100;
          memory[I+1]= V[X] / 10 % 10;
          memory[I+2]= V[X] % 100 % 10;
          pc+= 2;
          break;
        // 0xFX55, stores V[0] to V[X] in memory starting at I
        case 0x0055:
          unsigned char X= (opcode & 0x0F00) >> 8;
          for (char j= 0; j <= X; j++) {
            memory[I+j]= V[j];
          }
          pc+= 2;
          break;
        // 0xFX65, fills V[0] to V[X] with values from memory starting at I
        case 0x0065:
          unsigned char X= (opcode & 0x0F00) >> 8;
          for (char j= 0; j <= X; j++) {
            V[j]= memory[I+j];
          }
          break;
      }
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
}

void Chip8::loadGame(string game) {
  FILE* filePtr;

  filePtr= fopen(game.c_str(),"rb");
  if (filePtr != nullptr) {
    fread(memory + 0x200, 1, 4096 - 0x200, filePtr);
    fclose(filePtr);
  }
}