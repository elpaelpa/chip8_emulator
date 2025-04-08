#include <cstdint>
#include <chrono>
#include <random>
class Chip8{

    public:
        //keys array
        uint8_t keypad[16];
          //Display 64 x 32
        uint32_t display[64 * 32]{};

        Chip8();
        void LoadRom(char const *); //Loads rom from file
        void Cycle();
        
    private:
    void LoadFont();
    
    std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

    //Memory & Stack
    uint8_t memory[4096]{}; //4kb
    uint16_t stack[16]{}; 
    uint8_t stackPointer{};

    //Registers
    uint16_t programCounter{};
    uint16_t indexRegister{};
    uint8_t vRegisters[16]{}; //Registers V0 - VF (0 - 15)

    //Timers
    uint8_t delayTimer{};
    uint8_t soundTimer{};

    //Current opcode
    uint16_t opcode{};

    
    //Opcodes

    void Table0();
	void Table8();
	void TableE();
	void TableF();


    void op_NULL();
    void op_00E0();
    void op_00EE();
    void op_1NNN();
    void op_2NNN();
    void op_3XKK();
    void op_4XKK();
    void op_5XY0();
    void op_6XKK();
    void op_7XKK();
    void op_8XY0();
    void op_8XY1();
    void op_8XY2();
    void op_8XY3();
    void op_8XY4();
    void op_8XY5();
    void op_8XY6();
    void op_8XY7();
    void op_8XYE();
    void op_9XY0();
    void op_ANNN();
    void op_BNNN();
    void op_CXKK();
    void op_DXYN();
    void op_EX9E();
    void op_EXA1();
    void op_FX07();
    void op_FX0A();
    void op_FX15();
    void op_FX18();
    void op_FX1E();
    void op_FX29();
    void op_FX33();
    void op_FX55();
    void op_FX65();
    //Decode helpers

    uint8_t getVx();
    uint8_t getVy();
    uint8_t getKK();
    uint16_t getN();
    uint16_t getNN();   
    uint16_t getNNN();

    typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1]{&Chip8::op_NULL};
	Chip8Func table0[0xE + 1]{&Chip8::op_NULL};
	Chip8Func table8[0xE + 1]{&Chip8::op_NULL};
	Chip8Func tableE[0xE + 1]{&Chip8::op_NULL};
	Chip8Func tableF[0x65 + 1]{&Chip8::op_NULL};
};