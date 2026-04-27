// make print input, instruction, data, result and flags
#include "Instructiondispatcher.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include <iostream>

using namespace std;

Instructiondispatcher::Instructiondispatcher() {

}

Instructiondispatcher::~Instructiondispatcher() {

}

Instructiondispatcher * Instructiondispatcher::GetInstructionDispatcher() {
    if( instructiondispatcher != nullptr ) {
        return instructiondispatcher;
    }
    instructiondispatcher = new Instructiondispatcher();
    return instructiondispatcher;
}

void Instructiondispatcher::Add( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "ADD INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Add( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Sub( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "SUB INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Sub( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Inc( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "INC INSTRUCTION" << endl;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Inc( regs->ReadFrom8bRegister( register8b ) );
    regs->WriteTo8bRegister( register8b , result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Dec( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "DEC INSTRUCTION" << endl;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Dec( regs->ReadFrom8bRegister( register8b ) );
    regs->WriteTo8bRegister( register8b , result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::And( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "AND INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->And( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Or( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "OR INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Or( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Xor( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "XOR INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    Word result = FunctionalUnit::GetFunctionalUnit()->Xor( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    regs->WriteToAccumulator( result );
    cout << "RESULT: " << TwoComplementViwer( result ) << endl;
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Cp( REGISTERS_8b register8b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "CP INSTRUCTION" << endl;
    cout << "ACCUMULATOR VALUE: " << TwoComplementViwer( regs->ReadFromAccumulator() ) << " | INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom8bRegister( register8b ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->Cp( regs->ReadFromAccumulator() , regs->ReadFrom8bRegister( register8b ) );
    cout << regs->FlagsToTerminal();
}
 
void Instructiondispatcher::Nop() {
    cout << "NOP INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Nop();
}
 
void Instructiondispatcher::PushStack( REGISTERS_16b register16b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "PUSH INSTRUCTION" << endl;
    cout << "INPUT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
    FunctionalUnit::GetFunctionalUnit()->PushStack( register16b );
}
 
void Instructiondispatcher::PopStack( REGISTERS_16b register16b ) {
    Registers * regs = Registers::GetRegisters();
    cout << "POP INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->PopStack( register16b );
    cout << "RESULT REGISTER VALUE: " << TwoComplementViwer( regs->ReadFrom16bRegister( register16b ) ) << endl;
}