#ifndef SYSTEMARCHITECTURE_HPP
#define SYSTEMARCHITECTURE_HPP

#include <cstdint>
#include <limits>

#define MEM_SIZE_BYTES numeric_limits<uint16_t>::max()

using namespace std;

using Word = uint8_t;
using DoubleWord = uint16_t;
using Adress = uint16_t;

#define QUANTITY_8b_REGISTERS 6
enum REGISTERS_8b {
    B = 0,
    C = 1, 
    D = 2,
    E = 3,
    H = 4,
    L = 5
};

#define QUANTITY_16b_REGISTERS 4
enum REGISTERS_16b {
    AF,
    BC,
    DE,
    Hl
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

int TwoComplementViwer( Word data );
int TwoComplementViwer( DoubleWord data );
bool EvenParity( Word value );

#endif