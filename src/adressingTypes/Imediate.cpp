#include "adressingTypes/Imediate.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include <iostream>
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"

using namespace std;


Imediate::Imediate() : AdressingTypes() {

}

Imediate::~Imediate() {

}

InputAdressingTypes * Imediate::MakeInput( DecodedInstruction * instruction ) {
    return ( InputAdressingTypes * ) new InputImediate{};
}

size_t Imediate::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    return 0;
}

void Imediate::Jump( InputAdressingTypes * input ) {

}

void Imediate::JumpOffset( InputAdressingTypes * input ) {

}

void Imediate::Call( InputAdressingTypes * input ) {

}

void Imediate::Return( InputAdressingTypes * input ) {

}

void Imediate::LoadRegisterToResgister( InputAdressingTypes * input ) {

}

void Imediate::LoadValueToRegister( InputAdressingTypes * input ) {

}

void Imediate::LoadRegisterToMemory( InputAdressingTypes * input ) {

}

void Imediate::LoadMemoryToRegister( InputAdressingTypes * input ) {

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
    return {};
}

DecodedInstruction Imediate::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  