#include <fstream>
#include "Chip8.h"
#include <chrono>
#include <random>
#include <cstring>
#include <cstdint>

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_ADDRESS = 0x200;

uint8_t fontset[FONTSET_SIZE] = {
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

//CHIP 8 Functions
Chip8::Chip8():randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    programCounter = START_ADDRESS;
    
    //load font
    for(unsigned int i = 0; i < FONTSET_SIZE; ++i){
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }
    
    //Init rng
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
    
    //Set up function pointer table
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::op_1NNN;
	table[0x2] = &Chip8::op_2NNN;
	table[0x3] = &Chip8::op_3XKK;
	table[0x4] = &Chip8::op_4XKK;
	table[0x5] = &Chip8::op_5XY0;
	table[0x6] = &Chip8::op_6XKK;
	table[0x7] = &Chip8::op_7XKK;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::op_9XY0;
	table[0xA] = &Chip8::op_ANNN;
	table[0xB] = &Chip8::op_BNNN;
	table[0xC] = &Chip8::op_CXKK;
	table[0xD] = &Chip8::op_DXYN;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

    for (size_t i = 0; i <= 0xE; i++)
		{
			table0[i] = &Chip8::op_NULL;
			table8[i] = &Chip8::op_NULL;
			tableE[i] = &Chip8::op_NULL;
		}

	table0[0x0] = &Chip8::op_00E0;
	table0[0xE] = &Chip8::op_00EE;

	table8[0x0] = &Chip8::op_8XY0;
	table8[0x1] = &Chip8::op_8XY1;
	table8[0x2] = &Chip8::op_8XY2;
	table8[0x3] = &Chip8::op_8XY3;
	table8[0x4] = &Chip8::op_8XY4;
	table8[0x5] = &Chip8::op_8XY5;
	table8[0x6] = &Chip8::op_8XY6;
	table8[0x7] = &Chip8::op_8XY7;
	table8[0xE] = &Chip8::op_8XYE;

    for (size_t i = 0; i <= 0x65; i++)
    {
        tableF[i] = &Chip8::op_NULL;
    }

	tableE[0x1] = &Chip8::op_EXA1;
	tableE[0xE] = &Chip8::op_EX9E;

	tableF[0x07] = &Chip8::op_FX07;
	tableF[0x0A] = &Chip8::op_FX0A;
	tableF[0x15] = &Chip8::op_FX15;
	tableF[0x18] = &Chip8::op_FX18;
	tableF[0x1E] = &Chip8::op_FX1E;
	tableF[0x29] = &Chip8::op_FX29;
	tableF[0x33] = &Chip8::op_FX33;
	tableF[0x55] = &Chip8::op_FX55;
	tableF[0x65] = &Chip8::op_FX65;
}
//Loads Rom into mem
void Chip8::LoadRom(const char* filename){
    std::ifstream file(filename, std::ios::binary |std::ios::ate);

    if(file.is_open()){

        std::streampos size = file.tellg();
        char * buffer = new char[size];

        file.seekg(0,std::ios::beg);
        file.read(buffer,size);
        file.close();
        
        for(long i =0;i < size; ++i){
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;

    }
}
// Fetch Decode Execute
void Chip8::Cycle(){
    //Fetch
    opcode = (memory[programCounter] << 8u ) | memory[programCounter + 1];
    programCounter+=2;

    //Decode
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    if(delayTimer > 0){
        --delayTimer;
    }
    if(soundTimer > 0){
        --soundTimer;
    }
}

//Op Code Instructions
void Chip8::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8()
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE()
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

void Chip8::op_NULL() {
    // Ignored
}

void Chip8::op_00E0() {
    std::memset(display, 0, sizeof(display));
}

void Chip8::op_00EE() {
    --stackPointer; 
    programCounter = stack[stackPointer];
}

void Chip8::op_1NNN() {
    programCounter = getNNN();
}

void Chip8::op_2NNN() {
    stack[stackPointer] = programCounter;
    ++stackPointer;
    programCounter = getNNN();
}

void Chip8::op_3XKK() {
    if(vRegisters[getVx()] == getKK()){
        programCounter += 2;
    }
}

void Chip8::op_4XKK() {
    if(vRegisters[getVx()] != getKK()){
        programCounter += 2;
    }
}

void Chip8::op_5XY0() {
    if(vRegisters[getVx()] == vRegisters[getVy()]){
        programCounter += 2;
    }
}

void Chip8::op_6XKK() {
    vRegisters[getVx()] = getKK();
}

void Chip8::op_7XKK() {
    vRegisters[getVx()] += getKK();
}

void Chip8::op_8XY0() {
    vRegisters[getVx()] = vRegisters[getVy()];
}

void Chip8::op_8XY1() {
    vRegisters[getVx()] |= vRegisters[getVy()];
}

void Chip8::op_8XY2() {
    vRegisters[getVx()] &= vRegisters[getVy()];
}

void Chip8::op_8XY3() {
    vRegisters[getVx()] ^= vRegisters[getVy()];
}

void Chip8::op_8XY4() {
    uint16_t sum = vRegisters[getVx()] + vRegisters[getVy()];
    vRegisters[0xF] = (sum > 0xFF) ? 1 : 0;
    vRegisters[getVx()] = sum & 0xFFu;
}

void Chip8::op_8XY5() {
    vRegisters[0xF] = (vRegisters[getVx()] > vRegisters[getVy()]) ? 1 : 0;
    vRegisters[getVx()] -= vRegisters[getVy()];
}

void Chip8::op_8XY6() {
    vRegisters[0xF] = vRegisters[getVx()] & 0x1u;
    vRegisters[getVx()] >>= 1;
}

void Chip8::op_8XY7() {
    vRegisters[0xF] = (vRegisters[getVy()] > vRegisters[getVx()]) ? 1 : 0;
    vRegisters[getVx()] = vRegisters[getVy()] - vRegisters[getVx()];
}

void Chip8::op_8XYE() {
    vRegisters[0xF] = (vRegisters[getVx()] & 0x80u) >> 7u;
    vRegisters[getVx()] <<= 1;
}

void Chip8::op_9XY0() {
    if(vRegisters[getVx()] != vRegisters[getVy()]){
        programCounter += 2;
    }
}

void Chip8::op_ANNN() {
    indexRegister = getNNN();
}

void Chip8::op_BNNN() {
    programCounter = getNNN();
}

void Chip8::op_CXKK() {
    vRegisters[getVx()] = randByte(randGen) & getKK();
}

void Chip8::op_DXYN() {
    uint8_t Vx = getVx();
    uint8_t Vy = getVy();
    uint8_t height = opcode & 0x000Fu;

    uint8_t xPos = vRegisters[Vx] % 64;
    uint8_t yPos = vRegisters[Vy] % 32;
    vRegisters[0xF] = 0;

    for(unsigned int row = 0; row < height; ++row){
        uint8_t spriteByte = memory[indexRegister + row];

        for(unsigned int col = 0; col < 8; col++){
            uint8_t spritePixel = spriteByte & (0x80u >> col);
            uint32_t* screenPixel = &display[(yPos + row) * 64 + (xPos + col)];

            if(spritePixel){
                if(*screenPixel == 0xFFFFFFFF){
                    vRegisters[0xF] = 1;
                }
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::op_EX9E() {
    if(keypad[vRegisters[getVx()]]){
        programCounter += 2;
    }
}

void Chip8::op_EXA1() {
    if(!keypad[vRegisters[getVx()]]){
        programCounter += 2;
    }
}

void Chip8::op_FX07() {
    vRegisters[getVx()] = delayTimer;
}

void Chip8::op_FX0A() {
    uint8_t Vx = getVx();
    bool keyPressed = false;
    for(uint8_t i = 0; i < 16; ++i) {
        if(keypad[i]) {
            vRegisters[Vx] = i;
            keyPressed = true;
            break;
        }
    }
    if(!keyPressed) programCounter -= 2;
}

void Chip8::op_FX15() {
    delayTimer = vRegisters[getVx()];
}

void Chip8::op_FX18() {
    soundTimer = vRegisters[getVx()];
}

void Chip8::op_FX1E() {
    indexRegister += vRegisters[getVx()];
}

void Chip8::op_FX29() {
    uint8_t digit = vRegisters[getVx()];
    indexRegister = FONTSET_START_ADDRESS + (5 * digit);
}

void Chip8::op_FX33() {
    uint8_t Vx = getVx();
    uint8_t val = vRegisters[Vx];
    memory[indexRegister + 2] = val % 10;
    val /= 10;
    memory[indexRegister + 1] = val % 10;
    val /= 10;
    memory[indexRegister] = val % 10;
}

void Chip8::op_FX55() {
    for(uint8_t i = 0; i <= getVx(); i++){
        memory[indexRegister + i] = vRegisters[i];
    }
}

void Chip8::op_FX65() {
    for(uint8_t i = 0; i <= getVx(); i++){
        vRegisters[i] = memory[indexRegister + i];
    }
}


//extract  opcode Vx
uint8_t Chip8::getVx(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    return Vx;
}

//extract opcode Vy
uint8_t Chip8::getVy(){
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    return Vy;
}

//extract kk
uint8_t Chip8::getKK(){
    uint8_t kk = (opcode & 0x00FFu);
    return kk;
}
// extract n
uint16_t Chip8::getN(){
    uint16_t n = opcode & 0x000Fu;
    return n;
}
//extract nn
uint16_t Chip8::getNN(){
    uint16_t nn = opcode & 0x00FFu;
    return nn;
}
//extract nnn
uint16_t Chip8::getNNN(){
    uint16_t nnn = opcode & 0x0FFFu;
    return nnn;
}

