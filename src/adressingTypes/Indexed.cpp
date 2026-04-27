#include "adressingTypes/Indexed.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"

Indexed::Indexed() : AdressingTypes() {

}

Indexed::~Indexed() {

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

void Indexed::Nop() {

}

void Indexed::PushStack( InputAdressingTypes * input ) {

}

void Indexed::PopStack( InputAdressingTypes * input ) {

}

Word Indexed::EncodeInstruction( DecodedInstruction * instruction ) {
    return 0;
}

DecodedInstruction Indexed::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  