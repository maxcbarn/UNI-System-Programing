#include "adressingTypes/Imediate.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include <iostream>
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"
#include "memory/Memory.hpp"

using namespace std;


Imediate::Imediate() : AdressingTypes() {

}

Imediate::~Imediate() {

}

InputAdressingTypes * Imediate::MakeInput( DecodedInstruction * instruction ) {
    InputImediate * input = new InputImediate{};
    switch ( instruction->instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            input->value = instruction->imediateValue[ 0 ];
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            input->address = instruction->adresses[ 0 ];
            break;
        case LDVALTOREG:
            input->register8b_dest = instruction->registers8b[ 0 ];
            input->value = instruction->imediateValue[ 0 ];
            break;
        case PUSH:
        case POP:
            input->register16b = instruction->registers16b[ 0 ];
            break;
        default:
            break;
    }
    return ( InputAdressingTypes * ) input;
}

size_t Imediate::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch ( instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            return 2;
        case JP:
        case JPOFFSET:
        case CALL:
            return 3; 
        case RET:
        case NOP:
        case HLT:
            return 1;
        case LDVALTOREG:
            return 3; 
        case PUSH:
        case POP:
            return 3; 
        default:
            return 0;
    }
}

void Imediate::Jump( InputAdressingTypes * input ) {
    Adress address = ( ( InputImediate * ) input )->address;
    cout << "JP nn INSTRUCTION (Imediate)" << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Jump( address );
}

void Imediate::JumpOffset( InputAdressingTypes * input ) {
    Adress offset = ( ( InputImediate * ) input )->address;
    cout << "JR nn INSTRUCTION (Imediate)" << endl;
    cout << "OFFSET: " << TwoComplementViwer( offset ) << endl;
    FunctionalUnit::GetFunctionalUnit()->JumpOffset( offset );
}

void Imediate::Call( InputAdressingTypes * input ) {
    Adress address = ( ( InputImediate * ) input )->address;
    cout << "CALL nn INSTRUCTION (Imediate)" << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Call( address );
}

void Imediate::Return( InputAdressingTypes * input ) {
    cout << "RET INSTRUCTION (Imediate)" << endl;
    FunctionalUnit::GetFunctionalUnit()->Return();
}

void Imediate::LoadRegisterToResgister( InputAdressingTypes * input ) {
    // not applicable in immediate mode
}

void Imediate::LoadValueToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b dest = ( ( InputImediate * ) input )->register8b_dest;
    Word value = ( ( InputImediate * ) input )->value;
    cout << "LD r, n INSTRUCTION (Imediate)" << endl;
    cout << "IMMEDIATE VALUE: " << TwoComplementViwer( value ) << endl;
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void Imediate::LoadRegisterToMemory( InputAdressingTypes * input ) {
    // not applicable in immediate mode
}

void Imediate::LoadMemoryToRegister( InputAdressingTypes * input ) {
    // not applicable in immediate mode
}


void Imediate::Add( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    Word value = ((InputImediate*)input)->value;
    cout << "ADD INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT VALUE: " << TwoComplementViwer( value ) << endl;   
    Word result = FunctionalUnit::GetFunctionalUnit()->Add( regs->ReadFromAccumulator() , value );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Imediate::Sub( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    Word value = ( ( InputImediate * )input )->value;
    cout << "SUB INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Sub( regs->ReadFromAccumulator() , value );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Imediate::Inc( InputAdressingTypes * input ) {
    Word value = ( ( InputImediate * )input )->value;
    cout << "INC INSTRUCTION" << endl;
    cout << "INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Inc( value );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << Registers::GetRegisters()->FlagsToTerminal();
}

void Imediate::Dec( InputAdressingTypes * input ) {
    Word value = ( ( InputImediate * )input )->value;
    cout << "DEC INSTRUCTION" << endl;
    cout << "INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Dec( value );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << Registers::GetRegisters()->FlagsToTerminal();
}

void Imediate::And( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    Word value = ( ( InputImediate * )input )->value;
    cout << "AND INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->And( regs->ReadFromAccumulator() , value );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Imediate::Or( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    Word value = ( ( InputImediate * )input )->value;
    cout << "OR INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Or( regs->ReadFromAccumulator() , value );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Imediate::Xor( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    Word value = ( ( InputImediate * )input )->value;
    cout << "XOR INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Xor( regs->ReadFromAccumulator() , value );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Imediate::Cp( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    Word value = ( ( InputImediate * )input )->value;
    cout << "CP INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT VALUE: " << TwoComplementViwer( value ) << endl;
    FunctionalUnit::GetFunctionalUnit()->Cp( regs->ReadFromAccumulator() , value );
    cout << regs->FlagsToTerminal();
}

void Imediate::PushStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "PUSH INSTRUCTION" << endl;
    REGISTERS_16b register16b = ( ( InputImediate * )input )->register16b;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->PushStack( register16b );
}

void Imediate::PopStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "POP INSTRUCTION" << endl;
    REGISTERS_16b register16b = ( ( InputImediate * )input )->register16b;
    FunctionalUnit::GetFunctionalUnit()->PopStack( register16b );
    cout << "RESULT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
}

vector<Word> Imediate::EncodeInstruction( DecodedInstruction * instruction ) {
    vector<Word> encodedInstruction;
    encodedInstruction.push_back( ( Word ) instruction->instruction );
    switch ( instruction->instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] >> 8 ) );
            break;
        case LDVALTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;
        case PUSH:
        case POP:
            encodedInstruction.push_back( ( Word ) instruction->registers16b[ 0 ] );
            break;
        case RET:
        case NOP:
        case HLT:
            break;
        default:
            break;
    }
    return encodedInstruction;
}

DecodedInstruction Imediate::DecodeInstruction( Word instruction ) {
    DecodedInstruction decodedInstruction;
    decodedInstruction.instruction = static_cast<INSTRUCTIONS>( instruction );
    switch ( ( INSTRUCTIONS ) instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            decodedInstruction.imediateValue.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            decodedInstruction.adresses.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses[ 0 ] |= ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case LDVALTOREG:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.imediateValue.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case PUSH:
        case POP:
            decodedInstruction.registers16b.push_back( ( REGISTERS_16b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b[ 0 ] = ( REGISTERS_16b )( ( int )decodedInstruction.registers16b[ 0 ] | ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
            break;
        case RET:
        case NOP:
        case HLT:
            break;
        default:
            break;
    }
    return decodedInstruction;
}  