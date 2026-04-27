#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "adressingTypes/AdressingTypes.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"


class Processor {
    private:
        Processor();
        bool halt = false;
        inline static Processor * processor = nullptr;
        AdressingTypes * adressingTypes;
        INPUTADRESSINGTYPES adressingTypesUsed;
    public:
        ~Processor();
        static Processor * GetProcessor();
        void Halt();
        void Initialize( INPUTADRESSINGTYPES adressingTypes );
        void DecodeInstruction();
        void EncodeInstruction();
        bool NextInstruction();
        void LoadProgram();
};






#endif