#include "processor/memory/Registers.hpp"
#include "SystemArchitecture.hpp"
#include "processor/memory/Memory.hpp"
#include "processor/Processor.hpp"
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
    if( programCounter > endProgram ) {
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

void Registers::WriteTo8bRegister( REGISTERS registerEnum , Word data ) {
    if ( registerEnum >= REGISTERS::AF ) {
        cout << "INVALID 8b REGISTER" << endl;
        return;
    }
    generalUse8b[( int )registerEnum] = data;
}

DoubleWord Registers::ReadFrom16bRegister( REGISTERS registerEnum ) {
    if ( !(registerEnum >= REGISTERS::AF && registerEnum <= REGISTERS::HL) ) {
        cout << "INVALID 16b REGISTER" << " REGISTER: " << ( int )registerEnum << endl;
        return 0;
    }

    switch ( registerEnum ) {
        case REGISTERS::AF:
            return ( ( DoubleWord )generalUse8b[( int )REGISTERS::A] << 8 ) | ( DoubleWord )flag;
            break;
        case REGISTERS::BC:
            cout << "READING BC REGISTER: " << ( ( DoubleWord )generalUse8b[( int )REGISTERS::B] ) << "    " << ( DoubleWord )generalUse8b[( int )REGISTERS::C] << endl;
            return ( ( DoubleWord )generalUse8b[( int )REGISTERS::B] << 8 ) | ( DoubleWord )generalUse8b[( int )REGISTERS::C];
            break;
        case REGISTERS::DE:
            return ( ( DoubleWord )generalUse8b[( int )REGISTERS::D] << 8  ) | ( DoubleWord )generalUse8b[( int )REGISTERS::E];
            break;
        case REGISTERS::HL:
            return ( ( DoubleWord )generalUse8b[( int )REGISTERS::H] << 8  ) | ( DoubleWord )generalUse8b[( int )REGISTERS::L];
            break;
        default:
            return 0;
            break;
    }
}

void Registers::WriteTo16bRegister( REGISTERS registerEnum, DoubleWord data ) {
    if ( !(registerEnum >= REGISTERS::AF && registerEnum <= REGISTERS::HL) ) {
        cout << "INVALID 16b REGISTER" << " REGISTER: " << ( int )registerEnum << endl;
        return;
    }
    switch ( registerEnum ) {
        case REGISTERS::AF:
            return;
            break;
        case REGISTERS::BC:
            WriteTo8bRegister( REGISTERS::B, ( Word )( data >> 8 ) );
            WriteTo8bRegister( REGISTERS::C, ( Word )( data & 0xFF ) );
            break;
        case REGISTERS::DE:
            WriteTo8bRegister( REGISTERS::D, ( Word )( data >> 8 ) );
            WriteTo8bRegister( REGISTERS::E, ( Word )( data & 0xFF ) );
            break;
        case REGISTERS::HL:
            WriteTo8bRegister( REGISTERS::H, ( Word )( data >> 8 ) );
            WriteTo8bRegister( REGISTERS::L, ( Word )( data & 0xFF ) );
            break;
    }
}

void Registers::WriteToAccumulator( Word data ) {
    WriteTo8bRegister( REGISTERS::A, data );
}

Word Registers::ReadFromAccumulator() {
    return ReadFrom8bRegister( REGISTERS::A );
}

Word Registers::ReadFrom8bRegister( REGISTERS registerEnum ) {
    if ( registerEnum >= REGISTERS::AF ) {
        cout << "INVALID 8b REGISTER" << endl;
        return 0;
    }
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

DoubleWord Registers::ReadFromEspRegister( REGISTERS registerEnum ) {
    if( registerEnum < REGISTERS::PC ) {
        cout << "INVALID ESP REGISTER" << endl;
        return 0; 
    }
    switch ( registerEnum ) {
        case REGISTERS::IX:
            return indexX;
        case REGISTERS::IY:
            return indexY;
        case REGISTERS::SP:
            return stackPtr;
        case REGISTERS::PC:
            return programCounter;
        default:
            return 0;
    }
}

void Registers::WriteToEspRegister( REGISTERS registerEnum, DoubleWord data ) {
    if( registerEnum < REGISTERS::PC ) {
        cout << "INVALID ESP REGISTER" << endl;
        return; 
    }
    switch ( registerEnum ) {
        case REGISTERS::IX:
            indexX = data;
            break;
        case REGISTERS::IY:
            indexY = data;
            break;
        case REGISTERS::SP:
            stackPtr = data;
            break;
        case REGISTERS::PC:
            programCounter = data;
            break;
        default:
            break;
    }
}