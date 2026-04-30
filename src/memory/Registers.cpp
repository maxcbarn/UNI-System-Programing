#include "memory/Registers.hpp"
#include "SystemArchitecture.hpp"
#include "memory/Memory.hpp"
#include <iostream>

using namespace std;

Registers::Registers() {
    flag = 0;
    generalUse8b = vector< Word >( QUANTITY_8b_REGISTERS , 0 );
}
 
Registers::~Registers() {

}

Adress Registers::GetStartMemory() {
    return startMemory;
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

DoubleWord Registers::ReadFrom16bRegister( REGISTERS_16b registerEnum ) {
    switch ( registerEnum ) {
        case REGISTERS_16b::AF:
            return ( ( DoubleWord )generalUse8b[( int )A] << 8 ) | ( DoubleWord )flag;
            break;
        case REGISTERS_16b::BC:
            return ( ( DoubleWord )generalUse8b[( int )B] << 8 ) | ( DoubleWord )generalUse8b[( int )C];
            break;
        case REGISTERS_16b::DE:
            return ( ( DoubleWord )generalUse8b[( int )D] << 8  ) | ( DoubleWord )generalUse8b[( int )E];
            break;
        case REGISTERS_16b::Hl:
            return ( ( DoubleWord )generalUse8b[( int )H] << 8  ) | ( DoubleWord )generalUse8b[( int )L];
            break;
        default:
            return 0;
            break;
    }
}

void Registers::WriteTo16bRegister( REGISTERS_16b registerEnum, DoubleWord data ) {
    switch ( registerEnum ) {
        case REGISTERS_16b::AF:
            return;
            break;
        case REGISTERS_16b::BC:
            WriteTo8bRegister( B, ( Word )( data >> 8 ) );
            WriteTo8bRegister( C, ( Word )( data & 0xFF ) );
            break;
        case REGISTERS_16b::DE:
            WriteTo8bRegister( D, ( Word )( data >> 8 ) );
            WriteTo8bRegister( E, ( Word )( data & 0xFF ) );
            break;
        case REGISTERS_16b::Hl:
            WriteTo8bRegister( H, ( Word )( data >> 8 ) );
            WriteTo8bRegister( L, ( Word )( data & 0xFF ) );
            break;
    }
}

void Registers::WriteToAccumulator( Word data ) {
    WriteTo8bRegister( A, data );
}

Word Registers::ReadFromAccumulator() {
    return ReadFrom8bRegister( A );
}

Word Registers::ReadFrom8bRegister( REGISTERS_8b registerEnum ) {
    return generalUse8b[( size_t )registerEnum];
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