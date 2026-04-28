#include "Processor.hpp"
#include "adressingTypes/AdressingTypesFactory.hpp"
#include "memory/Memory.hpp"
#include "memory/Registers.hpp"

Processor::Processor() {

}

Processor::~Processor() {

}

Processor * Processor::GetProcessor() {
    if( processor == nullptr ) {
        processor = new Processor();
    }
    return processor;
}

void Processor::Halt() {
    halt = true;
}

void Processor::Initialize( INPUTADRESSINGTYPES adressingTypes ) {
    this->adressingTypes = AdressingTypesFactory::GetAdressingTypesFactory()->GetAdressingType( adressingTypes );
    this->adressingTypesUsed = adressingTypes;
    halt = false;
    Registers::ResetRegisters();
    Memory::ResetMemory();
}

bool Processor::NextInstruction() {
   if (halt) return false;

    Registers* regs = Registers::GetRegisters();
    Memory* mem = Memory::GetMemory();
    
    Word opcode = mem->ReadMemory(regs->GetPC());
    
    this->adressingTypes->Execute(opcode); 

    regs->IncrementPC(); 
    return true;
}

void Processor::LoadProgram(const std::vector<Word>& program, Adress startAdress) {
    Memory* mem = Memory::GetMemory();
    for (size_t i = 0; i < program.size(); ++i) {
        mem->ModifyMemory(startAdress + i, program[i]);
    }
}
