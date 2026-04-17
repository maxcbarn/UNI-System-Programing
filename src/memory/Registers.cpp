#include "memory/Registers.hpp"
#include "SystemArchitecture.hpp"
#include "memory/Memory.hpp"
#include <iostream>

using namespace std;

Registers::Registers() {
    accumulator = 0;
    flag = 0;
    generalUse8b = vector< Word >( QUANTITY_8b_REGISTERS , 0 );
    generalUse16b = vector< DoubleWord >( QUANTITY_16b_REGISTERS , 0 );
}
 
Registers::~Registers() {

}

Registers * Registers::GetRegisters() {
    if ( registers != nullptr ) {
        return registers;
    }
    registers = new Registers();
    return registers;
}

void Registers::WriteTo8bRegister( REGISTERS_8b registerEnum , Word data ) {
    generalUse8b[( int )registerEnum] = data;

}

Word Registers::ReadFrom16bRegisterLow( REGISTERS_16b registerEnum ) {
    return ( Word )(generalUse16b[( int )registerEnum] & 0b0000000011111111);
}

Word Registers::ReadFrom16bRegisterHigh( REGISTERS_16b registerEnum ) {
    DoubleWord a = generalUse16b[( int )registerEnum] >> 8;
    return ( Word )(generalUse16b[( int )registerEnum] >> 8);
}

void Registers::WriteToAccumulator( Word data ) {
    accumulator = data;
}

Word Registers::ReadToAccumulator() {
    return accumulator;
}

Word Registers::ReadFrom8bRegister( REGISTERS_8b registerEnum ) {
    return generalUse8b[( size_t )registerEnum];
}

void Registers::WriteTo16bRegister( REGISTERS_16b registerEnum , DoubleWord data ) {
    generalUse16b[( size_t )registerEnum] = data;
}

void Registers::WriteTo16bRegisterLow( REGISTERS_16b registerEnum , Word data ) {
    DoubleWord lowPart = data;
    DoubleWord highPart = generalUse16b[( int )registerEnum] & 0b1111111100000000 ;
    generalUse16b[( int )registerEnum] = highPart | lowPart;
}

void Registers::WriteTo16bRegisterHigh( REGISTERS_16b registerEnum , Word data ) {
    DoubleWord lowPart = generalUse16b[( int )registerEnum] & 0b0000000011111111;
    DoubleWord highPart = data << 8;
    generalUse16b[( int )registerEnum] = highPart | lowPart;
}

Adress Registers::GetStackPtr() {
    return stackPtr;
}

bool Registers::IncreaseStackPtr() {
   if( ( MEM_SIZE_BYTES - stackPtr ) < 1 ) {
        cout << "STACK UNDERFLOW" << endl;
        return false;
    } else {
        stackPtr += 1;
        return true;
    }
}

bool Registers::DecreaseStackPtr() {
    if( ( stackPtr - 1 ) < endStack ) {
        cout << "STACK OVERFLOW" << endl;
        return false;
    } else {
        stackPtr -= 1;
        return true;
    }
}