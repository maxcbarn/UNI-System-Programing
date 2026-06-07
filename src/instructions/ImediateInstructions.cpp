#include "processor/adressingTypes/InputAdressingTypes.hpp"
#include "instructions/ImediateInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include <iostream>

using namespace std;


ImediateInstructions::ImediateInstructions() : AdressingTypesInstructions() {

}

ImediateInstructions::~ImediateInstructions() {

}


size_t ImediateInstructions::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
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
        case JP:
        case JPOFFSET:
        case CALL:
            return 3; 
        case RET:
        case NOP:
        case HLT:
            return 1;
        case LDVALTOREG:
            return 3; 
        case PUSH:
        case POP:
            return 3; 
        default:
            return 0;
    }
}

vector<Word> ImediateInstructions::EncodeInstruction( DecodedInstruction * instruction ) {
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
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] >> 8 ) );
            break;
        case LDVALTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers8b[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;
        case PUSH:
        case POP:
            encodedInstruction.push_back( ( Word ) instruction->registers16b[ 0 ] );
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
