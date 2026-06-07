#ifndef INDEXEDINSTRUCTIONS_HPP
#define INDEXEDINSTRUCTIONS_HPP

#include "instructions/AdressingTypesInstructions.hpp"

using namespace std;

class IndexedInstructions : public AdressingTypesInstructions
{
private:
    
public:
    IndexedInstructions();
    ~IndexedInstructions();
    vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
    size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif
