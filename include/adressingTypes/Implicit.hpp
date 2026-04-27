#ifndef IMPLITCIT_HPP
#define IMPLITCIT_HPP

#include "SystemArchitecture.hpp"
#include "AdressingTypes.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"

class Implicit : public AdressingTypes {
    private:
    public: 
        Implicit();
        ~Implicit();
        void Add( InputAdressingTypes * input ) override;
        void Sub( InputAdressingTypes * input ) override;
        void Inc( InputAdressingTypes * input ) override;
        void Dec( InputAdressingTypes * input ) override;
        void And( InputAdressingTypes * input ) override;
        void Or( InputAdressingTypes * input ) override;
        void Xor( InputAdressingTypes * input ) override;
        void Cp( InputAdressingTypes * input ) override;
        void Nop() override;
        void PushStack( InputAdressingTypes * input ) override;
        void PopStack( InputAdressingTypes * input ) override;
        Word EncodeInstruction( DecodedInstruction * instruction ) override;
        DecodedInstruction DecodeInstruction( Word instruction ) override;
};

#endif