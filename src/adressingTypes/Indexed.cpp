#include "adressingTypes/Indexed.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"
#include "memory/Registers.hpp"
#include "memory/Memory.hpp"


Indexed::Indexed() : AdressingTypes() {

}

Indexed::~Indexed() {

}
size_t Indexed::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch ( instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
        case LDMEMTOREG:
        case LDREGTOMEM:
            return 2; 
        case JP:
        case JPOFFSET:
        case CALL:
            return 2; 

        case RET:
        case NOP:
        case HLT:
            return 1;
        default:
            return 0;
    }
}

void Indexed::Jump( InputAdressingTypes * input ) {

}

void Indexed::JumpOffset( InputAdressingTypes * input ) {

}

void Indexed::Call( InputAdressingTypes * input ) {

}

void Indexed::Return( InputAdressingTypes * input ) {

}

void Indexed::LoadRegisterToResgister( InputAdressingTypes * input ) {

}

void Indexed::LoadValueToRegister( InputAdressingTypes * input ) {

}

void Indexed::LoadRegisterToMemory( InputAdressingTypes * input ) {

}

void Indexed::LoadMemoryToRegister( InputAdressingTypes * input ) {

}

void Indexed::Add( InputAdressingTypes * input ) {

}

void Indexed::Sub( InputAdressingTypes * input ) {

}

void Indexed::Inc( InputAdressingTypes * input ) {

}

void Indexed::Dec( InputAdressingTypes * input ) {

}

void Indexed::And( InputAdressingTypes * input ) {

}

void Indexed::Or( InputAdressingTypes * input ) {

}

void Indexed::Xor( InputAdressingTypes * input ) {

}

void Indexed::Cp( InputAdressingTypes * input ) {

}

void Indexed::PushStack( InputAdressingTypes * input ) {

}

void Indexed::PopStack( InputAdressingTypes * input ) {

}

vector<Word> Indexed::EncodeInstruction( DecodedInstruction * instruction ) {
    return {};
}

DecodedInstruction Indexed::DecodeInstruction( Word instruction ) {
    DecodedInstruction decodedInstruction;
    decodedInstruction.instruction = static_cast<INSTRUCTIONS>( instruction );

    switch ( decodedInstruction.instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
        case LDMEMTOREG:
        case LDREGTOMEM:
        case JPOFFSET: 

            decodedInstruction.imediateValue.push_back( Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetProgramCounter() ) );
            Registers::GetRegisters()->IncreaseProgramCounter();
            decodedInstruction.registers16b.push_back( IX ); 

            break;

        case JP:
        case CALL:
            decodedInstruction.registers16b.push_back( IX );
            break;

        case RET:
        case NOP:
        case HLT:
        
            break;

        default:
            break;
    }

    return decodedInstruction;
} 

InputAdressingTypes * Indexed::MakeInput( DecodedInstruction * instruction ) {
    return ( InputAdressingTypes * ) new InputIndexed{};
}