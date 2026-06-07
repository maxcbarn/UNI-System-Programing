#ifndef IMEDIATEINSTRUCTIONS_HPP
#define IMEDIATEINSTRUCTIONS_HPP

#include "instructions/AdressingTypesInstructions.hpp"

using namespace std;

class ImediateInstructions : public AdressingTypesInstructions
{
private:
    
public:
    ImediateInstructions();
    ~ImediateInstructions();
    vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
    size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif
