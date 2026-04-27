// make print input, instruction, data, result and flags
#include "Implicit.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include <iostream>

using namespace std;

Implicit::Implicit() {

}

Implicit::~Implicit() {

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
 
void Implicit::Nop() {
    cout << "NOP INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Nop();
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