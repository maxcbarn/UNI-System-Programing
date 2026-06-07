#include "processor/adressingTypes/Direct.hpp"
#include "processor/adressingTypes/InputAdressingTypes.hpp"
#include "processor/adressingTypes/AdressingTypes.hpp"
#include "processor/FunctionalUnit.hpp"
#include "processor/memory/Registers.hpp"
#include "processor/memory/Memory.hpp"
#include <iostream>

using namespace std;

Direct::Direct() : AdressingTypes() {

}   

Direct::~Direct() {

}

void Direct::Jump( InputAdressingTypes * input ) {
    Adress address = ( ( InputDirect * ) input )->address;
    cout << "JP nn INSTRUCTION (Direct)" << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Jump( address );
}

void Direct::JumpOffset( InputAdressingTypes * input ) {
    Adress offset = ( ( InputDirect * ) input )->address;
    cout << "JR nn INSTRUCTION (Direct)" << endl;
    cout << "OFFSET: " << TwoComplementViwer( offset ) << endl;
    FunctionalUnit::GetFunctionalUnit()->JumpOffset( offset );
}

void Direct::Call( InputAdressingTypes * input ) {
    Adress address = ( ( InputDirect * ) input )->address;
    cout << "CALL nn INSTRUCTION (Direct)" << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Call( address );
}

void Direct::Return( InputAdressingTypes * input ) {
    cout << "RET INSTRUCTION (Direct)" << endl;
    FunctionalUnit::GetFunctionalUnit()->Return();
}

void Direct::LoadRegisterToResgister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b src = ( ( InputDirect * ) input )->register8b;
    REGISTERS_8b dest = ( ( InputDirect * ) input )->register8b_dest;
    cout << "LD r, r' INSTRUCTION (Direct)" << endl;
    cout << "SOURCE VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( src ) ) << " DESTINATION VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
    Word value = regs->ReadFrom8bRegister( src );
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT (REGISTER): " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void Direct::LoadValueToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b dest = ( ( InputDirect * ) input )->register8b_dest;
    Word value = ( ( InputDirect * ) input )->imediate;
    cout << "LD r, n INSTRUCTION (Direct)" << endl;
    cout << "IMMEDIATE VALUE: " << TwoComplementViwer( value ) << endl;
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

void Direct::LoadRegisterToMemory( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b src = ( ( InputDirect * ) input )->register8b;
    Adress address = ( ( InputDirect * ) input )->address;
    Word value = regs->ReadFrom8bRegister( src );
    cout << "LD (nn), r INSTRUCTION (Direct)" << endl;
    cout << "SOURCE REGISTER VALUE: " << TwoComplementViwer( value ) << endl;
    cout << "TARGET ADDRESS: 0x" << hex << address << dec << endl;
    FunctionalUnit::GetFunctionalUnit()->Store( address, value );
    cout << "STORED: " << TwoComplementViwer( value ) << endl;
}

void Direct::LoadMemoryToRegister( InputAdressingTypes * input ) {
    Registers * regs = Registers::GetRegisters();
    REGISTERS_8b dest = ( ( InputDirect * ) input )->register8b_dest;
    Adress address = ( ( InputDirect * ) input )->address;
    cout << "LD r, (nn) INSTRUCTION (Direct)" << endl;
    cout << "SOURCE ADDRESS: 0x" << hex << address << dec << endl;
    Word value = FunctionalUnit::GetFunctionalUnit()->Load( address );
    regs->WriteTo8bRegister( dest, value );
    cout << "RESULT: " << TwoComplementViwer( regs->ReadFrom8bRegister( dest ) ) << endl;
}

InputAdressingTypes * Direct::MakeInput( DecodedInstruction * instruction ) {
    InputDirect * input = new InputDirect{};
    switch ( instruction->instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            input->address = instruction->adresses[ 0 ];
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            input->address = instruction->adresses[ 0 ];
            break;
        case RET:
        case NOP:
        case HLT:
            break;
        case LDREGTOREG:
            input->register8b = instruction->registers8b[ 0 ];
            input->register8b_dest = instruction->registers8b[ 1 ];
            break;
        case LDVALTOREG:
            input->register8b_dest = instruction->registers8b[ 0 ];
            input->imediate = instruction->imediateValue[ 0 ];
            break;
        case LDREGTOMEM:
            input->register8b = instruction->registers8b[ 0 ];
            input->address = instruction->adresses[ 0 ];
            break;
        case LDMEMTOREG:
            input->register8b_dest = instruction->registers8b[ 0 ];
            input->address = instruction->adresses[ 0 ];
            break;
    }
    return ( InputAdressingTypes * ) input;
}

void Direct::Add( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    Adress address = ((InputDirect*)input)->address;
    Word memValue = Memory::GetMemory()->ReadMemory(address);

    cout << "ADD INSTRUCTION (Direct)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer(regs->ReadFromAccumulator())
         << " | MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->Add(regs->ReadFromAccumulator(), memValue);
    regs->WriteToAccumulator(result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << regs->FlagsToTerminal();
}

void Direct::Sub( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    Adress address = ((InputDirect*)input)->address;
    Word memValue = Memory::GetMemory()->ReadMemory(address);

    cout << "SUB INSTRUCTION (Direct)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer(regs->ReadFromAccumulator())
         << " | MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->Sub(regs->ReadFromAccumulator(), memValue);
    regs->WriteToAccumulator(result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << regs->FlagsToTerminal();
}

void Direct::Inc( InputAdressingTypes * input ) {
    Adress address = ((InputDirect*)input)->address;
    Memory* mem = Memory::GetMemory();
    Word memValue = mem->ReadMemory(address);

    cout << "INC INSTRUCTION (Direct)" << endl;
    cout << "MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->Inc(memValue);
    mem->ModifyMemory(address, result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << Registers::GetRegisters()->FlagsToTerminal();
}

void Direct::Dec( InputAdressingTypes * input ) {
    Adress address = ((InputDirect*)input)->address;
    Memory* mem = Memory::GetMemory();
    Word memValue = mem->ReadMemory(address);

    cout << "DEC INSTRUCTION (Direct)" << endl;
    cout << "MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->Dec(memValue);
    mem->ModifyMemory(address, result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << Registers::GetRegisters()->FlagsToTerminal();
}

void Direct::And( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    Adress address = ((InputDirect*)input)->address;
    Word memValue = Memory::GetMemory()->ReadMemory(address);

    cout << "AND INSTRUCTION (Direct)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer(regs->ReadFromAccumulator())
         << " | MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->And(regs->ReadFromAccumulator(), memValue);
    regs->WriteToAccumulator(result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << regs->FlagsToTerminal();
}

void Direct::Or( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    Adress address = ((InputDirect*)input)->address;
    Word memValue = Memory::GetMemory()->ReadMemory(address);

    cout << "OR INSTRUCTION (Direct)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer(regs->ReadFromAccumulator())
         << " | MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->Or(regs->ReadFromAccumulator(), memValue);
    regs->WriteToAccumulator(result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << regs->FlagsToTerminal();
}

void Direct::Xor( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    Adress address = ((InputDirect*)input)->address;
    Word memValue = Memory::GetMemory()->ReadMemory(address);

    cout << "XOR INSTRUCTION (Direct)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer(regs->ReadFromAccumulator())
         << " | MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    Word result = FunctionalUnit::GetFunctionalUnit()->Xor(regs->ReadFromAccumulator(), memValue);
    regs->WriteToAccumulator(result);

    cout << "RESULT: " << TwoComplementViwer(result) << endl;
    cout << regs->FlagsToTerminal();
}

void Direct::Cp( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    Adress address = ((InputDirect*)input)->address;
    Word memValue = Memory::GetMemory()->ReadMemory(address);

    cout << "CP INSTRUCTION (Direct)" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer(regs->ReadFromAccumulator())
         << " | MEMORY[" << address << "]: " << TwoComplementViwer(memValue) << endl;

    FunctionalUnit::GetFunctionalUnit()->Cp(regs->ReadFromAccumulator(), memValue);

    cout << regs->FlagsToTerminal();
}

void Direct::PushStack( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ((InputDirect*)input)->register16b;

    cout << "PUSH INSTRUCTION (Direct)" << endl;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer(regs->ReadFrom16bRegister(register16b)) << endl;

    FunctionalUnit::GetFunctionalUnit()->PushStack(register16b);
}

void Direct::PopStack( InputAdressingTypes * input ) {
    Registers* regs = Registers::GetRegisters();
    REGISTERS_16b register16b = ((InputDirect*)input)->register16b;

    FunctionalUnit::GetFunctionalUnit()->PopStack(register16b);

    cout << "POP INSTRUCTION (Direct)" << endl;
    cout << "RESULT REGISTER VALUE: " << TwoComplementViwer(regs->ReadFrom16bRegister(register16b)) << endl;
}

DecodedInstruction Direct::DecodeInstruction( Word instruction ) {
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
            decodedInstruction.adresses.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses[ 0 ] |= ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 );
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
        case RET:
        case NOP:
        case HLT:
            break;
        case LDREGTOREG:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case LDVALTOREG:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.imediateValue.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case LDREGTOMEM:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses[ 0 ] |= ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        case LDMEMTOREG:
            decodedInstruction.registers8b.push_back( ( REGISTERS_8b ) Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.adresses[ 0 ] |= ( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) << 8 );
            Registers::GetRegisters()->IncreaseProgramCounter();
            break;
        default:
            break;
    }
    return decodedInstruction;
}  
