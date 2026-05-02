#include "memory/Registers.hpp"
#include "SystemArchitecture.hpp"
#include "memory/Memory.hpp"
#include "Processor.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

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

void Registers::SetProgramSize( size_t size ) {;
    endProgram = size;
    startMemory = endProgram + 1;
}

void Registers::IncreaseProgramCounter() {
    if( programCounter >= endProgram ) {
        cout << "PROGRAM COUNTER OUT OF BOUNDS" << endl;
        Processor::GetProcessor()->Halt();  
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
        Processor::GetProcessor()->Halt();  
        return false;
    } else {
        stackPtr += 1;
        return true;
    }
}

bool Registers::DecreaseStackPtr() {
    if( ( stackPtr - 1 ) < endStack ) {
        cout << "STACK OVERFLOW" << endl;
        Processor::GetProcessor()->Halt();  
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
    std::ostringstream oss;

    const int W = 16;

    oss << std::left
        << "|" << std::setw(W) << "SIGN"
        << "|" << std::setw(W) << "ZERO"
        << "|" << std::setw(W) << "HALF_CARRY"
        << "|" << std::setw(W) << "PARITY_OVERFLOW"
        << "|" << std::setw(W) << "ADD_SUBTRACT"
        << "|" << std::setw(W) << "CARRY"
        << "|\n";

    oss << std::string(W * 6 + 7, '-') << "\n";

    oss << "|"
        << std::setw(W) << ((FLAGS::SIGN & flag) ? 1 : 0)
        << "|" << std::setw(W) << ((FLAGS::ZERO & flag) ? 1 : 0)
        << "|" << std::setw(W) << ((FLAGS::HALF_CARRY & flag) ? 1 : 0)
        << "|" << std::setw(W) << ((FLAGS::PARITY_OVERFLOW & flag) ? 1 : 0)
        << "|" << std::setw(W) << ((FLAGS::ADD_SUBTRACT & flag) ? 1 : 0)
        << "|" << std::setw(W) << ((FLAGS::CARRY & flag) ? 1 : 0)
        << "|\n";

    return oss.str();
}

DoubleWord Registers::ReadFromEspRegister( REGISTERS_ESP registerEnum ) {
    switch ( registerEnum ) {
        case REGISTERS_ESP::IX:
            return indexX;
        case REGISTERS_ESP::IY:
            return indexY;
        case REGISTERS_ESP::SP:
            return stackPtr;
        case REGISTERS_ESP::PC:
            return programCounter;
        default:
            return 0;
    }
}

void Registers::WriteToEspRegister( REGISTERS_ESP registerEnum, DoubleWord data ) {
    switch ( registerEnum ) {
        case REGISTERS_ESP::IX:
            indexX = data;
            break;
        case REGISTERS_ESP::IY:
            indexY = data;
            break;
        case REGISTERS_ESP::SP:
            stackPtr = data;
            break;
        case REGISTERS_ESP::PC:
            programCounter = data;
            break;
        default:
            break;
    }
}