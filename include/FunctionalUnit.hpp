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
        Word Add( Word data1 , Word data2 );
        void PushStack( REGISTERS_16b registerEnum );
        void PopStack( REGISTERS_16b registerEnum );
};




#endif