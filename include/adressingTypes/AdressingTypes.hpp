#ifndef ADRESSINGTYPES_HPP
#define ADRESSINGTYPES_HPP

#include "SystemArchitecture.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"

class AdressingTypes {
    private:
    
    public: 
        AdressingTypes() = default;
        virtual ~AdressingTypes() = default;
        virtual void Add( InputAdressingTypes * input ) = 0;
        virtual void Sub( InputAdressingTypes * input ) = 0;
        virtual void Inc( InputAdressingTypes * input ) = 0;
        virtual void Dec( InputAdressingTypes * input ) = 0;
        virtual void And( InputAdressingTypes * input ) = 0;
        virtual void Or( InputAdressingTypes * input ) = 0;
        virtual void Xor( InputAdressingTypes * input ) = 0;
        virtual void Cp( InputAdressingTypes * input ) = 0;
        virtual void Nop() = 0;
        virtual void PushStack( InputAdressingTypes * input ) = 0;
        virtual void PopStack( InputAdressingTypes * input ) = 0;
        virtual Word EncodeInstruction( DecodedInstruction * instruction ) = 0;
        virtual DecodedInstruction DecodeInstruction( Word instruction ) = 0;
};

#endif