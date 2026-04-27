#ifndef ADRESSINGTYPES_HPP
#define ADRESSINGTYPES_HPP

#include "SystemArchitecture.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"

class AdressingTypes {
    private:
    
    public: 
        AdressingTypes();
        virtual ~AdressingTypes() = default;
        virtual void Add( InputAdressingTypes * input );
        virtual void Sub( InputAdressingTypes * input );
        virtual void Inc( InputAdressingTypes * input );
        virtual void Dec( InputAdressingTypes * input );
        virtual void And( InputAdressingTypes * input );
        virtual void Or( InputAdressingTypes * input );
        virtual void Xor( InputAdressingTypes * input );
        virtual void Cp( InputAdressingTypes * input );
        virtual void Nop();
        virtual void PushStack( InputAdressingTypes * input );
        virtual void PopStack( InputAdressingTypes * input );
};

#endif