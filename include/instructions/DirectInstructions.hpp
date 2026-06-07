#ifndef DIRECTINSTRUCTIONS_HPP
#define DIRECTINSTRUCTIONS_HPP

#include "instructions/AdressingTypesInstructions.hpp"

using namespace std;

class DirectInstructions : public AdressingTypesInstructions
{
private:
    
public:
    DirectInstructions();
    ~DirectInstructions();
    vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
    size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif
