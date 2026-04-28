#include "adressingTypes/Direct.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"
#include "FunctionalUnit.hpp"
#include "memory/Registers.hpp"
#include "memory/Memory.hpp"
#include <iostream>

using namespace std;

Direct::Direct() : AdressingTypes() {

}   

Direct::~Direct() {

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

void Direct::Nop() {
    cout << "NOP INSTRUCTION (Direct)" << endl;
    FunctionalUnit::GetFunctionalUnit()->Nop();
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

Word Direct::EncodeInstruction( DecodedInstruction * instruction ) {
    return 0;
}

DecodedInstruction Direct::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  
