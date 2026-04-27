#ifndef INDIRECTREGISTER_HPP
#define INDIRECTREGISTER_HPP

#include "adressingTypes/AdressingTypes.hpp"


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
    void Nop() override;
    void PushStack( InputAdressingTypes * input ) override;
    void PopStack( InputAdressingTypes * input ) override;
    Word EncodeInstruction( DecodedInstruction * instruction ) override;
    DecodedInstruction DecodeInstruction( Word instruction ) override;  
};

#endif
