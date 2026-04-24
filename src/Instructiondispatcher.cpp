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