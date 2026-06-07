#include "processor/adressingTypes/AdressingTypes.hpp"
#include "processor/Processor.hpp"
#include "processor/FunctionalUnit.hpp"
#include <iostream>


using namespace std;

void AdressingTypes::Nop() {
    cout << "NOP INSTRUCTION" << endl;
    FunctionalUnit::GetFunctionalUnit()->Nop();
}

void AdressingTypes::Halt() {
    cout << "HLT INSTRUCTION" << endl;
    Processor::GetProcessor()->Halt();
}
