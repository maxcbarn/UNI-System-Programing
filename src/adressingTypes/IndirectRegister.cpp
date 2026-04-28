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

void IndirectRegister::Nop() {
    cout << "NOP INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Nop();
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

Word IndirectRegister::EncodeInstruction( DecodedInstruction * instruction ) {
    return 0;
}

DecodedInstruction IndirectRegister::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  