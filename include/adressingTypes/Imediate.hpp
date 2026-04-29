#ifndef IMEDIATE_HPP
#define IMEDIATE_HPP

#include "adressingTypes/AdressingTypes.hpp"

using namespace std;

class Imediate : public AdressingTypes
{
private:
    
public:
    Imediate();
    ~Imediate();
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
    vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
    DecodedInstruction DecodeInstruction( Word instruction ) override;  
    InputAdressingTypes * MakeInput( DecodedInstruction * instruction ) override;
    size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif
