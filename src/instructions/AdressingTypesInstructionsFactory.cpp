#include "instructions/AdressingTypesFactoryInstructions.hpp"
#include "instructions/ImplicitInstructions.hpp"
#include "instructions/ImediateInstructions.hpp"
#include "instructions/DirectInstructions.hpp"
#include "instructions/IndexedInstructions.hpp"
#include "instructions/IndirectRegisterInstructions.hpp"

AdressingTypesFactoryInstructions::AdressingTypesFactoryInstructions() {
}

AdressingTypesFactoryInstructions * AdressingTypesFactoryInstructions::GetAdressingTypesFactoryInstructions() {
    if( adressingTypesFactoryInstructions != nullptr ) {
        return adressingTypesFactoryInstructions;
    }
    adressingTypesFactoryInstructions = new AdressingTypesFactoryInstructions();
    return adressingTypesFactoryInstructions;
}

AdressingTypesInstructions * AdressingTypesFactoryInstructions::GetAdressingTypeInstructions( INPUTADRESSINGTYPES adressingType ) {
    switch( adressingType ) {
        case IMPLICIT:
            return new ImplicitInstructions();
        case IMEDIATE:
            return new ImediateInstructions();
        case DIRECT:
            return new DirectInstructions   ();
        case INDEXED:
            return new IndexedInstructions();
        case INDIRECT_REGISTER:
            return new IndirectRegisterInstructions();
        default:
            return nullptr;
    }
}

AdressingTypesFactoryInstructions::~AdressingTypesFactoryInstructions() {
}


