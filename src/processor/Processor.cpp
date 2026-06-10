#include "processor/Processor.hpp"
#include "processor/adressingTypes/AdressingTypesFactory.hpp"
#include "processor/memory/Memory.hpp"
#include "processor/memory/Registers.hpp"
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

    Adress pc = regs->GetProgramCounter();
    regs->IncreaseProgramCounter();
    Word modeByte  = Memory::GetMemory()->ReadMemory( pc );

    pc = regs->GetProgramCounter();
    regs->IncreaseProgramCounter();
    Word instrByte = Memory::GetMemory()->ReadMemory( pc );

    AdressingTypes* currentAdressingType =
        AdressingTypesFactory::GetAdressingTypesFactory()->GetAdressingType(
            (INPUTADRESSINGTYPES) modeByte
        );

    DecodedInstruction decodedInstruction = currentAdressingType->DecodeInstruction( instrByte );
    InputAdressingTypes * input = currentAdressingType->MakeInput( &decodedInstruction );

    switch ( decodedInstruction.instruction ) {
        case NOP:
            currentAdressingType->Nop();
            break;
        case HLT:
            currentAdressingType->Halt();
            break;
        case ADD:
            currentAdressingType->Add( input );
            break;
        case SUB:
            currentAdressingType->Sub( input );
            break;
        case AND:
            currentAdressingType->And( input );
            break;
        case OR:
            currentAdressingType->Or( input );
            break;
        case XOR:
            currentAdressingType->Xor( input );
            break;
        case CP:
            currentAdressingType->Cp( input );
            break;
        case INC:
            currentAdressingType->Inc( input );
            break;
        case DEC:
            currentAdressingType->Dec( input );
            break;
        case PUSH:
            currentAdressingType->PushStack( input );
            break;
        case POP:
            currentAdressingType->PopStack( input );
            break;
        case JP:
            currentAdressingType->Jump( input );
            break;
        case JPOFFSET:
            currentAdressingType->JumpOffset( input );
            break;
        case CALL:
            currentAdressingType->Call( input );
            break;
        case RET:
            currentAdressingType->Return( input );
            break;
        case LDREGTOREG:
            currentAdressingType->LoadRegisterToResgister( input );
            break;
        case LDVALTOREG:
            currentAdressingType->LoadValueToRegister( input );
            break;
        case LDREGTOMEM:
            currentAdressingType->LoadRegisterToMemory( input );
            break;
        case LDMEMTOREG:
            currentAdressingType->LoadMemoryToRegister( input );
            break;
        default:
            cout << "INSTRUCTION NOT FOUND" << endl;
            break;
    }
    delete input;
    delete currentAdressingType;
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
