#include "processor/adressingTypes/AdressingTypesFactory.hpp"
#include "processor/adressingTypes/Implicit.hpp"
#include "processor/adressingTypes/Imediate.hpp"
#include "processor/adressingTypes/Direct.hpp"
#include "processor/adressingTypes/Indexed.hpp"
#include "processor/adressingTypes/IndirectRegister.hpp"

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


