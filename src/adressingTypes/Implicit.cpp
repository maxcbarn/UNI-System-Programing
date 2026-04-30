// make print input, instruction, data, result and flags
#include "adressingTypes/Implicit.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include "memory/Memory.hpp"
#include <iostream>
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"

using namespace std;

Implicit::Implicit() : AdressingTypes() {

}

Implicit::~Implicit() {

}

InputAdressingTypes * Implicit::MakeInput( DecodedInstruction * instruction ) {
    InputImplicit * input = new InputImplicit{};
    switch (instruction->instruction) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            input->register8b = instruction->registers8b[ 0 ];
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            input->addres = instruction->adresses[ 0 ];
            break;
        case RET:
        case NOP:
        case HLT:
            // No additional bytes needed
            break;
        case LDREGTOREG:
            input->register8b = instruction->registers8b[ 0 ];
            input->register8b = instruction->registers8b[ 1 ];
            break;
        case LDVALTOREG:
            input->register8b = instruction->registers8b[ 0 ];
            input->imediate = instruction->imediateValue[ 0 ];
            break;
        case LDREGTOMEM:
           input->register8b = instruction->registers8b[ 0 ];
           break;
        case LDMEMTOREG:
            input->register8b = instruction->registers8b[ 0 ];
            break;
    }
    return ( InputAdressingTypes * ) input;
}

size_t Implicit::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch (instruction) {
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
        case LDREGTOREG:
            return 3;
        case LDVALTOREG:
            return 3;
        case LDREGTOMEM:
           return 2;
        case LDMEMTOREG:
            return 2;
        default:
            return 0;
    }
}

DecodedInstruction Implicit::DecodeInstruction( Word instruction ) {
    DecodedInstruction decodedInstruction;
    decodedInstruction.instruction = ( INSTRUCTIONS )instruction;
    switch (( INSTRUCTIONS )instruction) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b )Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses[0] |= ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 );
            break;
            break;
        case RET:
        case NOP:
        case HLT:
            // No additional bytes needed
            break;
        case LDREGTOREG:
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b )Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b )Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            break;
        case LDVALTOREG:
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b )Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.imediateValue.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            break;
        case LDREGTOMEM:
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b )Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            break;
        case LDMEMTOREG:
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b )Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            break;
        default:
            break;
    }
    return decodedInstruction; 
}
    
vector<Word> Implicit::EncodeInstruction( DecodedInstruction * instruction ) {
    vector<Word> encodedInstruction;
    encodedInstruction.push_back( ( Word )instruction->instruction );
    switch (instruction->instruction) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            encodedInstruction.push_back( ( Word )instruction->registers8b[ 0 ] );
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            encodedInstruction.push_back( ( Word )( instruction->adresses[0] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[0] >> 8 ) );
            break;
        case RET:
        case NOP:
        case HLT:
            // No additional bytes needed
            break;
        case LDREGTOREG:
            encodedInstruction.push_back( ( Word )instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word )instruction->registers8b[ 1 ] );
            break;
        case LDVALTOREG:
            encodedInstruction.push_back( ( Word )instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word )instruction->imediateValue[ 0 ] );
            break;
        case LDREGTOMEM:
            encodedInstruction.push_back( ( Word )instruction->registers8b[ 0 ] );
            break;
        case LDMEMTOREG:
            encodedInstruction.push_back( ( Word )instruction->registers8b[ 0 ] );
            break;
        default:
            break;
    }
    return encodedInstruction;
}   

void Implicit::Jump( InputAdressingTypes * input ) {
    Adress address = ( ( InputImplicit * )input )->addres;
    cout << "JP nn INSTRUCTION" << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Jump( address );
}

void Implicit::JumpOffset( InputAdressingTypes * input ) {
    Adress offset = ( ( InputImplicit * )input )->addres;
    cout << "JR nn INSTRUCTION" << endl;
    cout << "OFFSET: " << TwoComplementViwer( offset ) << endl;
    FunctionalUnit::GetFunctionalUnit()->JumpOffset( offset );
}

void Implicit::Call( InputAdressingTypes * input ) {
    Adress address = ( ( InputImplicit * )input )->addres;
    cout << "CALL nn INSTRUCTION" << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Call( address );
}

void Implicit::Return( InputAdressingTypes * input ) {
    cout << "RET INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Return();
}

void Implicit::LoadRegisterToResgister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b src = ( ( InputImplicit * )input )->register8b;
    REGISTERS_8b dest = ( ( InputImplicit * )input )->register8b_dest;
    cout << "LD r, r' INSTRUCTION" << endl;
    cout << "SOURCE VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( src ) ) << "DESTINATION VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
    Word value = regs->ReadFrom8bRegister( src );
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT (REGISTER): " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void Implicit::LoadValueToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b dest = ( ( InputImplicit * )input )->register8b_dest;
    Word value = ( ( InputImplicit * )input )->imediate;
    cout << "LD r, n INSTRUCTION" << endl;
    cout << "IMMEDIATE VALUE: " << TwoComplementViwer( value ) << endl;
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void Implicit::LoadRegisterToMemory( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b src = ( ( InputImplicit * )input )->register8b;
    Adress address = regs->ReadFrom16bRegister( Hl );
    Word value = regs->ReadFrom8bRegister( src );
    cout << "LD (HL), r INSTRUCTION" << endl;
    cout << "SOURCE REGISTER VALUE: " << TwoComplementViwer( value ) << endl;
    cout << "TARGET ADDRESS (HL): 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Store( address, value );
    cout << "STORED: " << TwoComplementViwer( value ) << endl;
}

void Implicit::LoadMemoryToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b dest = ( ( InputImplicit * )input )->register8b_dest;
    Adress address = regs->ReadFrom16bRegister( Hl );
    cout << "LD r, (HL) INSTRUCTION" << endl;
    cout << "SOURCE ADDRESS (HL): 0x" << hex << address << dec << endl;
    Word value = FunctionalUnit::GetFunctionalUnit()->Load( address );
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void Implicit::Add( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "ADD INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Add( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::Sub( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "SUB INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Sub( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::Inc( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "INC INSTRUCTION" << endl;
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Inc( regs->ReadFrom8bRegister( register8b ) );
    regs->WriteTo8bRegister( register8b , result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::Dec( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "DEC INSTRUCTION" << endl;
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Dec( regs->ReadFrom8bRegister( register8b ) );
    regs->WriteTo8bRegister( register8b , result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::And( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "AND INSTRUCTION" << endl;
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->And( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::Or( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "OR INSTRUCTION" << endl;
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Or( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::Xor( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "XOR INSTRUCTION" << endl;
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Xor( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Implicit::Cp( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "CP INSTRUCTION" << endl;
    REGISTERS_8b register8b = ( ( InputImplicit * )input )->register8b;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->Cp( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    cout << regs->FlagsToTerminal();
}

void Implicit::PushStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "PUSH INSTRUCTION" << endl;
    REGISTERS_16b register16b = ( ( InputImplicit * )input )->register16b;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->PushStack( register16b );
}
 
void Implicit::PopStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    cout << "POP INSTRUCTION" << endl;
    REGISTERS_16b register16b = ( ( InputImplicit * )input )->register16b;
    FunctionalUnit::GetFunctionalUnit()->PopStack( register16b );
    cout << "RESULT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
}