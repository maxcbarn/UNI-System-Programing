#include "instructions/IndexedInstructions.hpp"
#include "processor/adressingTypes/InputAdressingTypes.hpp"

using namespace std;

IndexedInstructions::IndexedInstructions() : AdressingTypesInstructions() {

}

IndexedInstructions::~IndexedInstructions() {

}

size_t IndexedInstructions::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch ( instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            return 3; // 3 bytes: Opcode + IX/IY + Offset
        case LDMEMTOREG:
        case LDREGTOMEM:
            return 4; // 4 bytes: Opcode + Reg8b + IX/IY + Offset
        case PUSH:
        case POP:
            return 2; // opcode + IX/IY
        case RET:
        case NOP:
        case HLT:
            return 1;
        default:
            return 0;
    }
}

vector<Word> IndexedInstructions::EncodeInstruction( DecodedInstruction * instruction ) {
    vector<Word> encodedInstruction;
    encodedInstruction.push_back( ( Word ) instruction->instruction );
    
    switch ( instruction->instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            encodedInstruction.push_back( ( Word ) instruction->registersEsp[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;

        case LDMEMTOREG:
        case LDREGTOMEM:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->registersEsp[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;

        case RET:
        case NOP:
        case HLT:
            break;
            
        default:
            break;
    }
    
    return encodedInstruction;
}