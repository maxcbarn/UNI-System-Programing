#include "processor/adressingTypes/InputAdressingTypes.hpp"
#include "instructions/DirectInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include <iostream>

using namespace std;

DirectInstructions::DirectInstructions() : AdressingTypesInstructions() {

}   

DirectInstructions::~DirectInstructions() {

}

size_t DirectInstructions::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    switch ( instruction ) {
        case ADD:
        case SUB:
        case AND:
        case OR:
        case XOR:
        case CP:
        case INC:
        case DEC:
            return 3; 
        case JP:
        case JPOFFSET:
        case CALL:
            return 3;
        case RET:
        case NOP:
        case HLT:
            return 1;
        case LDREGTOREG:
            return 3;
        case LDVALTOREG:
            return 3;
        case LDREGTOMEM:
            return 4;
        case LDMEMTOREG:
            return 4;
        case PUSH:
        case POP:
            return 2;
        default:
            return 0;
    }
}


vector<Word> DirectInstructions::EncodeInstruction( DecodedInstruction * instruction ) {
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
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] >> 8 ) );
            break;
        case JP:
        case JPOFFSET:
        case CALL:
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] >> 8 ) );
            break;
        case RET:
        case NOP:
        case HLT:
            break;
        case LDREGTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->registers[ 1 ] );
            break;
        case LDVALTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers[ 0 ] );
            encodedInstruction.push_back( ( Word ) instruction->imediateValue[ 0 ] );
            break;
        case LDREGTOMEM:
            encodedInstruction.push_back( ( Word ) instruction->registers[ 0 ] );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] >> 8 ) );
            break;
        case LDMEMTOREG:
            encodedInstruction.push_back( ( Word ) instruction->registers[ 0 ] );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] & 0x00FF ) );
            encodedInstruction.push_back( ( Word )( instruction->adresses[ 0 ] >> 8 ) );
            break;
        case PUSH:
        case POP:
            encodedInstruction.push_back( ( Word )instruction->registers[ 0 ] );
            break;
        default:
            break;
    }
    return encodedInstruction;
}
