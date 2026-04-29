#ifndef INDIRECTREGISTER_HPP
#define INDIRECTREGISTER_HPP

#include "adressingTypes/AdressingTypes.hpp"

using namespace std;

class IndirectRegister : public AdressingTypes
{
private:
    
public:
    IndirectRegister();
    ~IndirectRegister();
    void Add( InputAdressingTypes * input ) override;
    void Sub( InputAdressingTypes * input ) override;
    void Inc( InputAdressingTypes * input ) override;
    void Dec( InputAdressingTypes * input ) override;
    void And( InputAdressingTypes * input ) override;
    void Or( InputAdressingTypes * input ) override;
    void Xor( InputAdressingTypes * input ) override;
    void Cp( InputAdressingTypes * input ) override;
    using AdressingTypes::Nop;
    using AdressingTypes::Halt;
    void Jump( InputAdressingTypes * input ) override;
    void JumpOffset( InputAdressingTypes * input ) override;
    void Call( InputAdressingTypes * input ) override;
    void Return( InputAdressingTypes * input ) override;
    void LoadRegisterToResgister( InputAdressingTypes * input ) override;
    void LoadValueToRegister( InputAdressingTypes * input ) override;
    void LoadRegisterToMemory( InputAdressingTypes * input ) override;
    void LoadMemoryToRegister( InputAdressingTypes * input ) override;
    void PushStack( InputAdressingTypes * input ) override;
    void PopStack( InputAdressingTypes * input ) override;
    vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
    DecodedInstruction DecodeInstruction( Word instruction ) override;  
    InputAdressingTypes * MakeInput( DecodedInstruction * instruction ) override;
    size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif
