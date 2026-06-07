#ifndef ADRESSINGTYPESINSTRUCTIONS_HPP
#define ADRESSINGTYPESINSTRUCTIONS_HPP

#include "SystemArchitecture.hpp"
#include "processor/adressingTypes/InputAdressingTypes.hpp"

using namespace std;

class AdressingTypesInstructions {
    private:
    
    public: 
        AdressingTypesInstructions() = default;
        virtual ~AdressingTypesInstructions() = default;
        virtual vector<Word> EncodeInstruction( DecodedInstruction * instruction ) = 0;
        virtual size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) = 0;
};

#endif