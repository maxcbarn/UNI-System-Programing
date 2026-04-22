#ifndef FUNCTIONALUNIT_HPP
#define FUNCTIONALUNIT_HPP

#include "SystemArchitecture.hpp"

class FunctionalUnit {
    private:
        inline static FunctionalUnit * functionalUnit = nullptr;
        FunctionalUnit();
    public:
        static FunctionalUnit * GetFunctionalUnit();
        ~FunctionalUnit();
        void Nop();
        Word Add( Word data1 , Word data2 );
        Word Sub( Word data1 , Word data2 );
        Word And( Word data1 , Word data2 );
        Word Or( Word data1 , Word data2 );
        Word Xor( Word data1 , Word data2 );
        void Cp( Word data1 , Word data2 );
        Word Inc( Word data1 );
        Word Dec( Word data1 );
        void PushStack( REGISTERS_16b registerEnum );
        void PopStack( REGISTERS_16b registerEnum );
};




#endif