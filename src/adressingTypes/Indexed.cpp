#include "adressingTypes/Indexed.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include "memory/Memory.hpp"
#include <iostream>

using namespace std;

Indexed::Indexed() : AdressingTypes() {

}

Indexed::~Indexed() {

}

size_t Indexed::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch ( instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            return 3; // 3 bytes: Opcode + IX/IY + Offset
        case LDMEMTOREG:
        case LDREGTOMEM:
            return 4; // 4 bytes: Opcode + Reg8b + IX/IY + Offset
        case PUSH:
        case POP:
            return 2; // opcode + IX/IY
        case RET:
        case NOP:
        case HLT:
            return 1;
        default:
            return 0;
    }
}

void Indexed::Jump( InputAdressingTypes * input ) {
    // not applicable in indexed mode
}

void Indexed::JumpOffset( InputAdressingTypes * input ) {
    // not applicable in indexed mode
}

void Indexed::Call( InputAdressingTypes * input ) {
    // not applicable in indexed mode
}

void Indexed::Return( InputAdressingTypes * input ) {
    cout << "RET INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Return();
}

void Indexed::LoadRegisterToResgister( InputAdressingTypes * input ) {
    // not applicable in indexed mode
}

void Indexed::LoadValueToRegister( InputAdressingTypes * input ) {
    // not applicable in indexed mode
}

void Indexed::LoadRegisterToMemory( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    
    Word value = regs->ReadFrom8bRegister( idxInput->register8b );
    
    cout << "LD (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << "), r INSTRUCTION (Indexed)" << endl;
    cout << "SOURCE REGISTER VALUE: " << TwoComplementViwer( value ) << endl;
    cout << "TARGET ADDRESS: 0x" << hex << targetAddress << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Store( targetAddress, value );
    cout << "STORED: " << TwoComplementViwer( value ) << endl;
}

void Indexed::LoadMemoryToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    
    cout << "LD r, (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "SOURCE ADDRESS: 0x" << hex << targetAddress << dec << endl;
    Word value = FunctionalUnit::GetFunctionalUnit()->Load( targetAddress );
    regs->WriteTo8bRegister( idxInput->register8b, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( idxInput->register8b ) ) << endl;
}

void Indexed::Add( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "ADD A, (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Add( regs->ReadFromAccumulator(), memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Indexed::Sub( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "SUB (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Sub( regs->ReadFromAccumulator(), memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Indexed::Inc( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "INC (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Inc( memValue );
    Memory::GetMemory()->ModifyMemory( targetAddress, result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Indexed::Dec( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "DEC (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Dec( memValue );
    Memory::GetMemory()->ModifyMemory( targetAddress, result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Indexed::And( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "AND (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->And( regs->ReadFromAccumulator(), memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Indexed::Or( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "OR (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Or( regs->ReadFromAccumulator(), memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}


void Indexed::Xor( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "XOR (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Xor( regs->ReadFromAccumulator(), memValue );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}

void Indexed::Cp( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    DoubleWord baseAddress = regs->ReadFromEspRegister( idxInput->registerEsp );
    int8_t signedOffset = (int8_t) idxInput->offset;
    Adress targetAddress = baseAddress + signedOffset;
    Word memValue = Memory::GetMemory()->ReadMemory( targetAddress );
    
    cout << "CP (" << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " + " << ( int )signedOffset << ") INSTRUCTION (Indexed)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | MEMORY[" << targetAddress << "]: " << TwoComplementViwer( memValue ) << endl;
    FunctionalUnit::GetFunctionalUnit()->Cp( regs->ReadFromAccumulator(), memValue );
    cout << regs->FlagsToTerminal();
}

void Indexed::PushStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = (InputIndexed *)input;
    cout << "PUSH " << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " INSTRUCTION (Indexed)" << endl;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFromEspRegister( idxInput->registerEsp ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->PushStack( idxInput->registerEsp );
}

void Indexed::PopStack( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    InputIndexed * idxInput = ( InputIndexed * ) input;
    FunctionalUnit::GetFunctionalUnit()->PopStack( idxInput->registerEsp );
    cout << "POP " << ( idxInput->registerEsp == IX ? "IX" : "IY" ) << " INSTRUCTION (Indexed)" << endl;
    cout << "RESULT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFromEspRegister( idxInput->registerEsp ) ) << endl;
}

vector<Word> Indexed::EncodeInstruction( DecodedInstruction * instruction ) {
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
            encodedInstruction.push_back( ( Word ) instruction->registersEsp[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;

        case LDMEMTOREG:
        case LDREGTOMEM:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->registersEsp[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
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

DecodedInstruction Indexed::DecodeInstruction( Word instruction ) {
    DecodedInstruction decodedInstruction;
    decodedInstruction.instruction = static_cast<INSTRUCTIONS>( instruction );
    Registers * regs = Registers::GetRegisters();
    Memory * mem = Memory::GetMemory();

    switch ( decodedInstruction.instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            decodedInstruction.registersEsp.push_back( (REGISTERS_ESP) mem->ReadMemory( regs->GetProgramCounter() ) );
            regs->IncreaseProgramCounter();
            
            decodedInstruction.imediateValue.push_back( mem->ReadMemory( regs->GetProgramCounter() ) );
            regs->IncreaseProgramCounter();
            break;

        case LDMEMTOREG:
        case LDREGTOMEM:
            decodedInstruction.registers8b.push_back( (REGISTERS_8b) mem->ReadMemory( regs->GetProgramCounter() ) );
            regs->IncreaseProgramCounter();
            
            decodedInstruction.registersEsp.push_back( (REGISTERS_ESP) mem->ReadMemory( regs->GetProgramCounter() ) );
            regs->IncreaseProgramCounter();
            
            decodedInstruction.imediateValue.push_back( mem->ReadMemory( regs->GetProgramCounter() ) );
            regs->IncreaseProgramCounter();
            break;

        case PUSH:
        case POP:
            decodedInstruction.registersEsp.push_back( (REGISTERS_ESP) mem->ReadMemory( regs->GetProgramCounter() ) );
            regs->IncreaseProgramCounter();
            break;
            
        case RET:
        case NOP:
        case HLT:
            // No additional bytes needed
            break;

        default:
            break;
    }
    return decodedInstruction;
} 

InputAdressingTypes * Indexed::MakeInput( DecodedInstruction * instruction ) {
    InputIndexed * input = new InputIndexed();
    switch (instruction->instruction) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            input->registerEsp = instruction->registersEsp[0];
            input->offset = instruction->imediateValue[0];
            break;
        case LDMEMTOREG:
        case LDREGTOMEM:
            input->register8b = instruction->registers8b[0];
            input->registerEsp = instruction->registersEsp[0];
            input->offset = instruction->imediateValue[0];
            break;
        case PUSH:
        case POP:
            input->registerEsp = instruction->registersEsp[0];
            break;
        default:
            break;
    }
    return ( InputAdressingTypes * ) input;
}
