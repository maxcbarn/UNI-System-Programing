#include "adressingTypes/IndirectRegister.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"


IndirectRegister::IndirectRegister() : AdressingTypes() {

}

IndirectRegister::~IndirectRegister() {

}

void IndirectRegister::Add( InputAdressingTypes * input ) {

}

void IndirectRegister::Sub( InputAdressingTypes * input ) {

}

void IndirectRegister::Inc( InputAdressingTypes * input ) {

}

void IndirectRegister::Dec( InputAdressingTypes * input ) {

}

void IndirectRegister::And( InputAdressingTypes * input ) {

}

void IndirectRegister::Or( InputAdressingTypes * input ) {

}

void IndirectRegister::Xor( InputAdressingTypes * input ) {

}

void IndirectRegister::Cp( InputAdressingTypes * input ) {

}

void IndirectRegister::Nop() {

}

void IndirectRegister::PushStack( InputAdressingTypes * input ) {

}

void IndirectRegister::PopStack( InputAdressingTypes * input ) {

}

Word IndirectRegister::EncodeInstruction( DecodedInstruction * instruction ) {
    return 0;
}

DecodedInstruction IndirectRegister::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  