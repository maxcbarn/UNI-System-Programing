#include "adressingTypes/IndirectRegister.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include "memory/Memory.hpp"
#include <iostream>
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"

using namespace std;

IndirectRegister::IndirectRegister() : AdressingTypes() {

}

IndirectRegister::~IndirectRegister() {

}

void IndirectRegister::Jump( InputAdressingTypes * input ) {
    // not applicable in indirect register mode
}

void IndirectRegister::JumpOffset( InputAdressingTypes * input ) {
    // not applicable in indirect register mode
}

void IndirectRegister::Call( InputAdressingTypes * input ) {
    // not applicable in indirect register mode
}

void IndirectRegister::Return( InputAdressingTypes * input ) {
    // not applicable in indirect register mode
}

void IndirectRegister::LoadRegisterToResgister( InputAdressingTypes * input ) {
    // not applicable in indirect register mode
}

void IndirectRegister::LoadValueToRegister( InputAdressingTypes * input ) {
    // not applicable in indirect register mode
}

void IndirectRegister::LoadRegisterToMemory( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b src = ( ( InputIndirectRegister * ) input )->register8b;
    REGISTERS_16b register16b = ( ( InputIndirectRegister * ) input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word value = regs->ReadFrom8bRegister( src );
    cout << "LD (rr), r INSTRUCTION (IndirectRegister)" << endl;
    cout << "SOURCE REGISTER VALUE: " << TwoComplementViwer( value ) << endl;
    cout << "TARGET ADDRESS (" << register16b << "): 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Store( address, value );
    cout << "STORED: " << TwoComplementViwer( value ) << endl;

}

void IndirectRegister::LoadMemoryToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b dest = ( ( InputIndirectRegister * ) input )->register8b_dest;
    REGISTERS_16b register16b = ( ( InputIndirectRegister * ) input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    cout << "LD r, (rr) INSTRUCTION (IndirectRegister)" << endl;
    cout << "SOURCE ADDRESS (" << register16b << "): 0x" << hex << address << dec << endl;
    Word value = FunctionalUnit::GetFunctionalUnit()->Load( address );
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void IndirectRegister::Add( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "ADD INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Add( regs->ReadFromAccumulator() , memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::Sub( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "SUB INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Sub( regs->ReadFromAccumulator() , memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::Inc( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "INC INSTRUCTION" << endl;
    cout << "MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Inc( memValue );
    Memory::GetMemory()->ModifyMemory( address , result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::Dec( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "DEC INSTRUCTION" << endl;
    cout << "MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Dec( memValue );
    Memory::GetMemory()->ModifyMemory( address , result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::And( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "AND INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->And( regs->ReadFromAccumulator() , memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::Or( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "OR INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Or( regs->ReadFromAccumulator() , memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::Xor( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "XOR INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Xor( regs->ReadFromAccumulator() , memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::Cp( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    Adress address = regs->ReadFrom16bRegister( register16b );
    Word memValue = Memory::GetMemory()->ReadMemory( address );
    cout << "CP INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << address << "]: " << TwoComplementViwer( memValue ) << endl;
    FunctionalUnit::GetFunctionalUnit()->Cp( regs->ReadFromAccumulator() , memValue );
    cout << regs->FlagsToTerminal();
}

void IndirectRegister::PushStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    cout << "PUSH INSTRUCTION" << endl;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->PushStack( register16b );
}

void IndirectRegister::PopStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ( ( InputIndirectRegister * )input )->register16b;
    FunctionalUnit::GetFunctionalUnit()->PopStack( register16b );
    cout << "POP INSTRUCTION" << endl;
    cout << "RESULT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
}

size_t IndirectRegister::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
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
        case LDREGTOMEM:
        case LDMEMTOREG:
            return 3; 
        case PUSH:
        case POP:
            return 2; 
        case RET:
        case NOP:
        case HLT:
            return 1;
        default:
            return 0;
    }
}

vector<Word> IndirectRegister::EncodeInstruction( DecodedInstruction * instruction ) {
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
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
            break;
        case LDREGTOMEM:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
            break;
        case LDMEMTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
            break;
        case PUSH:
        case POP:
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
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

DecodedInstruction IndirectRegister::DecodeInstruction( Word instruction ) {
    DecodedInstruction decodedInstruction;
    decodedInstruction.instruction = ( INSTRUCTIONS ) instruction;
    switch ( ( INSTRUCTIONS ) instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            decodedInstruction.registers16b.push_back( ( REGISTERS_16b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b[ 0 ] = ( REGISTERS_16b )( ( int )decodedInstruction.registers16b[ 0 ] | ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case LDREGTOMEM:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b.push_back( ( REGISTERS_16b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b[ 0 ] = ( REGISTERS_16b )( ( int )decodedInstruction.registers16b[ 0 ] | ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case LDMEMTOREG:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b.push_back( ( REGISTERS_16b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b[ 0 ] = ( REGISTERS_16b )( ( int )decodedInstruction.registers16b[ 0 ] | ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case PUSH:
        case POP:
            decodedInstruction.registers16b.push_back( ( REGISTERS_16b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b[ 0 ] = ( REGISTERS_16b )( ( int )decodedInstruction.registers16b[ 0 ] | ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
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

InputAdressingTypes * IndirectRegister::MakeInput( DecodedInstruction * instruction ) {
    InputIndirectRegister * input = new InputIndirectRegister{};
    switch ( instruction->instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            input->register16b = instruction->registers16b[ 0 ];
            break;
        case LDREGTOMEM:
            input->register8b = instruction->registers8b[ 0 ];
            input->register16b = instruction->registers16b[ 0 ];
            break;
        case LDMEMTOREG:
            input->register8b_dest = instruction->registers8b[ 0 ];
            input->register16b = instruction->registers16b[ 0 ];
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