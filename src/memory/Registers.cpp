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

void Registers::SetProgramSize( size_t size ) {
    endProgram = size;
    startMemory = endProgram + 1;
}

void Registers::IncreaseProgramCounter() {
    if( programCounter >= endProgram ) {
        cout << "PROGRAM COUNTER OUT OF BOUNDS" << endl;
    } else {
        programCounter++;
    }
}

void Registers::SetProgramCounter( Adress adress ) {
    programCounter = adress;
}

Adress Registers::GetProgramCounter() {
    return programCounter;
}

Registers * Registers::GetRegisters() {
    if ( registers != nullptr ) {
        return registers;
    }
    registers = new Registers();
    return registers;
}


void Registers::ResetRegisters() {
    delete registers;
    registers = new Registers();
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

DoubleWord Registers::ReadFrom16bRegister( REGISTERS_16b registerEnum ) {
    return ( DoubleWord )generalUse16b[( int )registerEnum];
}

void Registers::WriteToAccumulator( Word data ) {
    accumulator = data;
}

Word Registers::ReadFromAccumulator() {
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

void Registers::AddFlag( FLAGS flag ) {
    this->flag = this->flag | flag;
}

void Registers::ClearFlag( FLAGS flag ) {
    this->flag = this->flag & ~flag;
}

string Registers::FlagsToTerminal() {
    string ans = "FLAGS VALUES\n";

    if( FLAGS::SIGN & flag ) {
        ans += "SIGN-TRUE\n";
    } else {
        ans += "SIGN-FALSE\n";
    }

    if( FLAGS::ZERO & flag ) {
        ans += "ZERO-TRUE\n";
    } else {
        ans += "ZERO-FALSE\n";
    }

    if( FLAGS::HALF_CARRY & flag ) {
        ans += "HALF_CARRY-TRUE\n";
    } else {
        ans += "HALF_CARRY-FALSE\n";
    }

    if( FLAGS::PARITY_OVERFLOW & flag ) {
        ans += "PARITY_OVERFLOW-TRUE\n";
    } else {
        ans += "PARITY_OVERFLOW-FALSE\n";
    }

    if( FLAGS::ADD_SUBTRACT & flag ) {
        ans += "ADD_SUBTRACT-TRUE\n";
    } else {
        ans += "ADD_SUBTRACT-FALSE\n";
    }

    if( FLAGS::CARRY & flag ) {
        ans += "CARRY-TRUE\n";
    } else {
        ans += "CARRY-FALSE\n";
    }

    return ans;
}