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
            return 2; 
        case LDREGTOMEM:
        case LDMEMTOREG:
            return 3; 
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
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
            break;
        case LDREGTOMEM:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
            break;
        case LDMEMTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
            break;
        case PUSH:
        case POP:
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->registers16b[ 0 ] >> 8 ) );
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
