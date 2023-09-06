/**
 * This contains the data structure/memory of the emulator
*/

struct Chip8 {
  unsigned short opcode; // 2 byte long 
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


  // Graphics of 2048 pixels
  unsigned char gfx[64 * 32];

  unsigned char delay_timer; // 60 Hz, timer register that counts down to 0
  unsigned char sound_timer; // 60 Hz, timer register that counts down to 0

  unsigned short stack[16]; // stores program counter when opcode jumps
  unsigned short sp; // stack pointer

  unsigned char key[16]; // keypad
};