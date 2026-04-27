#include "adressingTypes/Imediate.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"

Imediate::Imediate() : AdressingTypes() {

}

Imediate::~Imediate() {

}

void Imediate::Add( InputAdressingTypes * input ) {

}

void Imediate::Sub( InputAdressingTypes * input ) {

}

void Imediate::Inc( InputAdressingTypes * input ) {

}

void Imediate::Dec( InputAdressingTypes * input ) {

}

void Imediate::And( InputAdressingTypes * input ) {

}

void Imediate::Or( InputAdressingTypes * input ) {

}

void Imediate::Xor( InputAdressingTypes * input ) {

}

void Imediate::Cp( InputAdressingTypes * input ) {

}

void Imediate::Nop() {

}

void Imediate::PushStack( InputAdressingTypes * input ) {

}

void Imediate::PopStack( InputAdressingTypes * input ) {

}

Word Imediate::EncodeInstruction( DecodedInstruction * instruction ) {
    return 0;
}

DecodedInstruction Imediate::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  