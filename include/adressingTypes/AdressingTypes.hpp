#ifndef ADRESSINGTYPES_HPP
#define ADRESSINGTYPES_HPP

#include "SystemArchitecture.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"

using namespace std;

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
        void Nop();
        void Halt();
        virtual void Jump( InputAdressingTypes * input ) = 0;
        virtual void JumpOffset( InputAdressingTypes * input ) = 0;
        virtual void Call( InputAdressingTypes * input ) = 0;
        virtual void Return( InputAdressingTypes * input ) = 0;
        virtual void LoadRegisterToResgister( InputAdressingTypes * input ) = 0;
        virtual void LoadValueToRegister( InputAdressingTypes * input ) = 0;
        virtual void LoadRegisterToMemory( InputAdressingTypes * input ) = 0;
        virtual void LoadMemoryToRegister( InputAdressingTypes * input ) = 0;
        virtual void PushStack( InputAdressingTypes * input ) = 0;
        virtual void PopStack( InputAdressingTypes * input ) = 0;
        virtual vector<Word> EncodeInstruction( DecodedInstruction * instruction ) = 0;
        virtual DecodedInstruction DecodeInstruction( Word instruction ) = 0;
        virtual InputAdressingTypes * MakeInput( DecodedInstruction * instruction ) = 0;
        virtual size_t GetInstructionWordQuantity( INSTRUCTIONS instruction ) = 0;
};

#endif