#include "adressingTypes/AdressingTypes.hpp"

#include <iostream>
#include "Processor.hpp"
#include "FunctionalUnit.hpp"


using namespace std;

void AdressingTypes::Nop() {
    cout << "NOP INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Nop();
}

void AdressingTypes::Halt() {
    cout << "HLT INSTRUCTION" << endl;
    Processor::GetProcessor()->Halt();
}
