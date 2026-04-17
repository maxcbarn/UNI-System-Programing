#include "FunctionalUnit.hpp"
#include "memory/Memory.hpp"
#include "memory/Registers.hpp"


FunctionalUnit::FunctionalUnit() {

}

FunctionalUnit::~FunctionalUnit() {

}

FunctionalUnit * FunctionalUnit::GetFunctionalUnit() {
    if( functionalUnit != nullptr ) {
        return functionalUnit;
    }
    functionalUnit = new FunctionalUnit();
    return functionalUnit;
}

void FunctionalUnit::PushStack( REGISTERS_16b registerEnum ) {
    Memory * memory = Memory::GetMemory();
    Registers * registers = Registers::GetRegisters();
    registers->DecreaseStackPtr();
    memory->ModifyMemory( registers->GetStackPtr() , registers->ReadFrom16bRegisterLow( registerEnum ) );
    registers->DecreaseStackPtr();
    memory->ModifyMemory( registers->GetStackPtr() , registers->ReadFrom16bRegisterHigh( registerEnum ) );
    memory = nullptr;
    registers = nullptr;
}

void FunctionalUnit::PopStack( REGISTERS_16b registerEnum ) {
    Memory * memory = Memory::GetMemory();
    Registers * registers = Registers::GetRegisters();
    registers->WriteTo16bRegisterHigh( registerEnum , memory->ReadMemory( registers->GetStackPtr() ) );
    registers->IncreaseStackPtr();
    registers->WriteTo16bRegisterLow( registerEnum , memory->ReadMemory( registers->GetStackPtr() ) );
    registers->IncreaseStackPtr();
    memory = nullptr;
    registers = nullptr;
}   