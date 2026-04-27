#ifndef INPUADRESSINGTYPES_HPP
#define INPUADRESSINGTYPES_HPP

#include "SystemArchitecture.hpp"

enum INPUTADRESSINGTYPES {
    IMPLICIT,
    IMEDIATE,
    DIRECT,
    INDEXED,
    INDIRECT_REGISTER
};


struct InputAdressingTypes {
    
};

struct InputImplicit : public InputAdressingTypes {
    REGISTERS_8b register8b;
    REGISTERS_16b register16b;
};














#endif