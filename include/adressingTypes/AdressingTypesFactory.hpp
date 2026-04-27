#ifndef ADRESSINGTYPESFACTORY_HPP
#define ADRESSINGTYPESFACTORY_HPP

#include "adressingTypes/AdressingTypes.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"

class AdressingTypesFactory
{
private:
    AdressingTypesFactory();
    inline static AdressingTypesFactory * adressingTypesFactory = nullptr;
public:
    static AdressingTypesFactory * GetAdressingTypesFactory();
    AdressingTypes * GetAdressingType( INPUTADRESSINGTYPES adressingType );
    ~AdressingTypesFactory();
};


#endif



