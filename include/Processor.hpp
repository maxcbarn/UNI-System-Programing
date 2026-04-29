#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "adressingTypes/AdressingTypes.hpp"
#include "adressingTypes/InputAdressingTypes.hpp"

using namespace std;

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
        bool NextInstruction();
        void LoadProgram( vector<Word> program );
        vector<Word> ReadParameter( size_t wordQuantity );
};






#endif