#ifndef SYSTEMARCHITECTURE_HPP
#define SYSTEMARCHITECTURE_HPP

#include <cstdint>
#include <limits>
#include <vector>

#define MEM_SIZE_BYTES numeric_limits<uint16_t>::max()

using namespace std;

using Word = uint8_t;
using DoubleWord = uint16_t;
using Adress = uint16_t;

#define QUANTITY_8b_REGISTERS 7
enum REGISTERS_8b {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    H = 5,
    L = 6
};

#define QUANTITY_16b_REGISTERS 4
enum REGISTERS_16b {
    AF = 0b00000001,
    BC = 0b00000010,
    DE = 0b00000100,
    Hl = 0b00001000
};

#define QUANTITY_ESP_REGISTERS 4
enum REGISTERS_ESP {
    PC,
    SP,
    IX,
    IY
};

enum FLAGS {
    CARRY = 0b00000001,
    ADD_SUBTRACT = 0b00000010,
    PARITY_OVERFLOW = 0b00000100,
    X = 0b00001000,
    HALF_CARRY = 0b00010000,
    Y = 0b00100000,
    ZERO = 0b01000000,
    SIGN = 0b10000000
};

enum INSTRUCTIONS {
    NOP = 0b00000000,
    HLT = 0b00000001,
    ADD = 0b00000010,
    SUB = 0b00000011,
    AND = 0b00000100,
    OR = 0b00000101,
    XOR = 0b00000110,
    CP = 0b00000111,
    INC = 0b00001000,
    DEC = 0b00001001,
    PUSH = 0b00001010,
    POP = 0b00001011,
    JP = 0b00001100,
    JPOFFSET = 0b00001101,
    CALL = 0b00001110,
    RET = 0b00001111,
    LDREGTOREG = 0b00010000,
    LDVALTOREG = 0b00010001,
    LDREGTOMEM = 0b00010010, // In reality is a store, strange architecture
    LDMEMTOREG = 0b00010011, // real load from memory
};


struct DecodedInstruction {
    INSTRUCTIONS instruction;
    vector< REGISTERS_8b > registers8b; // first one is the first on the instruction and so on
    vector< REGISTERS_16b > registers16b; // first one is the first on the instruction and so on
    vector< Word > imediateValue; // first one is the first on the instruction and so on
    vector< Adress > adresses; // first one is the first on the instruction and so on
};


int TwoComplementViwer( Word data );
int TwoComplementViwer( DoubleWord data );
bool EvenParity( Word value );

#endif