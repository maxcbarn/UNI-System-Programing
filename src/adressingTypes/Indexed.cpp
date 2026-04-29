#include "adressingTypes/Indexed.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"

Indexed::Indexed() : AdressingTypes() {

}

Indexed::~Indexed() {

}

size_t Indexed::GetInstructionWordQuantity( INSTRUCTIONS instruction ) {
    return 0;
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
    return DecodedInstruction();
}  

InputAdressingTypes * Indexed::MakeInput( DecodedInstruction * instruction ) {
    return ( InputAdressingTypes * ) new InputIndexed{};
}