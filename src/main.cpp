#include "Processor.hpp"
#include "adressingTypes/Implicit.hpp"
#include "adressingTypes/AdressingTypesFactory.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"
#include "SystemArchitecture.hpp"
#include <iostream>


using std::vector;

static DecodedInstruction Make0(INSTRUCTIONS op) {
    DecodedInstruction d{};
    d.instruction = op;
    return d;
}

static DecodedInstruction Make8(INSTRUCTIONS op, REGISTERS_8b r) {
    DecodedInstruction d{};
    d.instruction = op;
    d.registers8b.push_back(r);
    return d;
}

static DecodedInstruction Make16(INSTRUCTIONS op, REGISTERS_16b r) {
    DecodedInstruction d{};
    d.instruction = op;
    d.registers16b.push_back(r);
    return d;
}


vector<DecodedInstruction> BuildProgram() {
    vector<DecodedInstruction> program;

    program.push_back(Make0(NOP));
    program.push_back(Make8(ADD, B));
    program.push_back(Make8(SUB, C));
    program.push_back(Make8(AND, D));
    program.push_back(Make8(OR, E));
    program.push_back(Make8(XOR, H));
    program.push_back(Make8(CP, L));
    program.push_back(Make8(INC, B));
    program.push_back(Make8(DEC, C));
    program.push_back(Make16(PUSH, BC));
    program.push_back(Make16(POP, DE));
    program.push_back(Make0(HLT));

    return program;
}


int main( int argc , char const *argv[] ) {
    Processor * processor = Processor::GetProcessor();  
    INPUTADRESSINGTYPES adressingType = IMPLICIT;
    processor->Initialize( adressingType );
    vector<DecodedInstruction> program = BuildProgram();
    vector<Word> encodedProgram, aux;
    for ( size_t i = 0 ; i < program.size() ; i++) {
        aux = AdressingTypesFactory::GetAdressingTypesFactory()->GetAdressingType( adressingType )->EncodeInstruction( &program[i] );
        encodedProgram.insert( encodedProgram.end(), aux.begin(), aux.end() );
    }
    
    processor->LoadProgram( encodedProgram );


    cout << "Press Enter To Next Instruction...";
    cin.get();
    while ( processor->NextInstruction() ) {
        cout << "Press Enter To Next Instruction...";
        cin.get();
    }

    return 0;
}


