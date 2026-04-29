#include "Processor.hpp"
#include "adressingTypes/AdressingTypesFactory.hpp"
#include "memory/Memory.hpp"
#include "memory/Registers.hpp"
#include <iostream>

using namespace std;

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
    if( halt ) {
        cout << "PROCESSOR HALTED" << endl;
        return false;
    }
    Registers * regs = Registers::GetRegisters();
    regs->IncreaseProgramCounter();
    Adress pc = regs->GetProgramCounter();
    Word instruction = Memory::GetMemory()->ReadMemory( pc );
    DecodedInstruction decodedInstruction = adressingTypes->DecodeInstruction( instruction );
    InputAdressingTypes * input = adressingTypes->MakeInput( &decodedInstruction );
    switch ( decodedInstruction.instruction ) {
        case NOP:
            adressingTypes->Nop();
            break;
        case HLT:
            adressingTypes->Halt();
            break;
        case ADD:
            adressingTypes->Add( input );
            break;
        case SUB:
            adressingTypes->Sub( input );
            break;
        case AND:
            adressingTypes->And( input );
            break;
        case OR:
            adressingTypes->Or( input );
            break;
        case XOR:
            adressingTypes->Xor( input );
            break;
        case CP:
            adressingTypes->Cp( input );
            break;
        case INC:
            adressingTypes->Inc( input );
            break;
        case DEC:
            adressingTypes->Dec( input );
            break;
        case PUSH:
            adressingTypes->PushStack( input );
            break;
        case POP:
            adressingTypes->PopStack( input );
            break;
        default:
            cout << "INSTRUCTION NOT FOUND" << endl;
            break;
    }
    return true;
}

vector<Word> Processor::ReadParameter( size_t wordQuantity ) {
    Registers * regs = Registers::GetRegisters();
    Memory * memory = Memory::GetMemory();
    vector<Word> parameters;
    for ( size_t index = 0 ; index < wordQuantity ; index++ ) {
        regs->IncreaseProgramCounter();
        Adress pc = regs->GetProgramCounter();
        parameters.push_back( memory->ReadMemory( pc ) );
    }
    return parameters;
}

void Processor::LoadProgram( vector<Word> program ) {
    Memory * memory = Memory::GetMemory();
    Registers * regs = Registers::GetRegisters();
    for( size_t index = 0 ; index < program.size() ; index++ ) {
        memory->ModifyMemory( index , program[index] );
    }
    regs->SetProgramSize( program.size() - 1 );
}
