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
        void PushStack( DoubleWord data );
        void PushStack( REGISTERS_16b registerEnum );
        void PopStack();
        void PopStack( REGISTERS_16b registerEnum );
        void PushStack( REGISTERS_ESP registerEnum );
        void PopStack( REGISTERS_ESP registerEnum );
        Word Load( Adress address );
        void Store( Adress address , Word data );
        void Jump( Adress address );
        void JumpOffset( Adress offset );
        void Call( Adress address );
        void Return();
}; 




#endif