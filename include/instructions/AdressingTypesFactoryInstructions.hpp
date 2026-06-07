#ifndef ADRESSINGTYPESFACTORYINSTRUCTIONS_HPP
#define ADRESSINGTYPESFACTORYINSTRUCTIONS_HPP

#include "instructions/AdressingTypesInstructions.hpp"
#include "processor/adressingTypes/InputAdressingTypes.hpp"

class AdressingTypesFactoryInstructions
{
private:
    AdressingTypesFactoryInstructions();
    inline static AdressingTypesFactoryInstructions * adressingTypesFactoryInstructions = nullptr;
public:
    static AdressingTypesFactoryInstructions * GetAdressingTypesFactoryInstructions();
    AdressingTypesInstructions * GetAdressingTypeInstructions( INPUTADRESSINGTYPES adressingType );
    ~AdressingTypesFactoryInstructions();
};


#endif



