#include "processor/adressingTypes/InputAdressingTypes.hpp"
#include "instructions/IndirectRegisterInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"

using namespace std;

IndirectRegisterInstructions::IndirectRegisterInstructions() : AdressingTypesInstructions() {

}

IndirectRegisterInstructions::~IndirectRegisterInstructions() {

}

size_t IndirectRegisterInstructions::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch ( instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            return 1; 
        case LDREGTOMEM:
        case LDMEMTOREG:
            return 2; 
        case PUSH:
        case POP:
            return 2; 
        case RET:
        case NOP:
        case HLT:
            return 1;
        default:
            return 0;
    }
}

vector<Word> IndirectRegisterInstructions::EncodeInstruction( DecodedInstruction * instruction ) {
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
            break;
        case LDREGTOMEM:
        case LDMEMTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers[ 0 ] );
            break;
        case PUSH:
        case POP:
            encodedInstruction.push_back( ( Word ) instruction->registers[ 0 ] );
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
