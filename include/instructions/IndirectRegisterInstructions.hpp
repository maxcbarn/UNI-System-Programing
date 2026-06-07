#ifndef INDIRECTREGISTERINSTRUCTIONS_HPP
#define INDIRECTREGISTERINSTRUCTIONS_HPP

#include "instructions/AdressingTypesInstructions.hpp"

using namespace std;

class IndirectRegisterInstructions : public AdressingTypesInstructions
{
private:
    
public:
    IndirectRegisterInstructions();
    ~IndirectRegisterInstructions();
    vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
    size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif
