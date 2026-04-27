#include "adressingTypes/AdressingTypesFactory.hpp"
#include "adressingTypes/Implicit.hpp"
#include "adressingTypes/Imediate.hpp"
#include "adressingTypes/Direct.hpp"
#include "adressingTypes/Indexed.hpp"
#include "adressingTypes/IndirectRegister.hpp"

AdressingTypesFactory::AdressingTypesFactory() {
}

AdressingTypesFactory * AdressingTypesFactory::GetAdressingTypesFactory() {
    if( adressingTypesFactory != nullptr ) {
        return adressingTypesFactory;
    }
    adressingTypesFactory = new AdressingTypesFactory();
    return adressingTypesFactory;
}

AdressingTypes * AdressingTypesFactory::GetAdressingType( INPUTADRESSINGTYPES adressingType ) {
    switch( adressingType ) {
        case IMPLICIT:
            return new Implicit();
        case IMEDIATE:
            return new Imediate();
        case DIRECT:
            return new Direct();
        case INDEXED:
            return new Indexed();
        case INDIRECT_REGISTER:
            return new IndirectRegister();
        default:
            return nullptr;
    }
}

AdressingTypesFactory::~AdressingTypesFactory() {
}


