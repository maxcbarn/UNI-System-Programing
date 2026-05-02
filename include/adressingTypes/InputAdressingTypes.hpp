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
    REGISTERS_8b register8b_dest;
    REGISTERS_16b register16b;
    Adress addres;
    Word imediate;
};

struct InputDirect : public InputAdressingTypes {
    REGISTERS_8b register8b;
    REGISTERS_8b register8b_dest;
    REGISTERS_16b register16b;
    Adress address;
    Word imediate;
};

struct InputImediate : public InputAdressingTypes {
    Word value;
    REGISTERS_8b register8b_dest;
    REGISTERS_16b register16b;
    Adress address;
};

struct InputIndirectRegister : public InputAdressingTypes {
    REGISTERS_16b register16b;
    REGISTERS_8b register8b;
    REGISTERS_8b register8b_dest;
};

struct InputIndexed : public InputAdressingTypes {
    REGISTERS_ESP registerEsp;
    Word offset;
    REGISTERS_8b register8b;
};









#endif