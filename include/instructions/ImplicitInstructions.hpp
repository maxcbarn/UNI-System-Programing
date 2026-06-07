#ifndef IMPLITCITINSTRUCTIONS_HPP
#define IMPLITCITINSTRUCTIONS_HPP

#include "SystemArchitecture.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include "processor/adressingTypes/InputAdressingTypes.hpp"

using namespace std;

class ImplicitInstructions : public AdressingTypesInstructions {
    private:
    public: 
        ImplicitInstructions();
        ~ImplicitInstructions();
        vector<Word> EncodeInstruction( DecodedInstruction * instruction ) override;
        size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) override;
};

#endif