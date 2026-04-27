#include "adressingTypes/Direct.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "adressingTypes/AdressingTypes.hpp"


Direct::Direct() : AdressingTypes() {

}   

Direct::~Direct() {

}


void Direct::Add( InputAdressingTypes * input ) {

}

void Direct::Sub( InputAdressingTypes * input ) {

}

void Direct::Inc( InputAdressingTypes * input ) {

}

void Direct::Dec( InputAdressingTypes * input ) {

}

void Direct::And( InputAdressingTypes * input ) {

}

void Direct::Or( InputAdressingTypes * input ) {

}

void Direct::Xor( InputAdressingTypes * input ) {

}

void Direct::Cp( InputAdressingTypes * input ) {

}

void Direct::Nop() {

}

void Direct::PushStack( InputAdressingTypes * input ) {

}

void Direct::PopStack( InputAdressingTypes * input ) {

}

Word Direct::EncodeInstruction( DecodedInstruction * instruction ) {
    return 0;
}

DecodedInstruction Direct::DecodeInstruction( Word instruction ) {
    return DecodedInstruction();
}  
