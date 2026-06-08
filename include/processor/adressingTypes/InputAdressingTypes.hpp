#ifndef INPUADRESSINGTYPES_HPP
#define INPUADRESSINGTYPES_HPP

#include "SystemArchitecture.hpp"

struct InputAdressingTypes {
    
};

struct InputImplicit : public InputAdressingTypes {
    REGISTERS register8b;
    REGISTERS register8b_dest;
    REGISTERS register16b;
    Adress addres;
    Word imediate;
};

struct InputDirect : public InputAdressingTypes {
    REGISTERS register8b;
    REGISTERS register8b_dest;
    REGISTERS register16b;
    Adress address;
    Word imediate;
};

struct InputImediate : public InputAdressingTypes {
    Word value;
    REGISTERS register8b_dest;
    REGISTERS register16b;
    Adress address;
};

struct InputIndirectRegister : public InputAdressingTypes {
    REGISTERS register16b;
    REGISTERS register8b;
    REGISTERS register8b_dest;
};

struct InputIndexed : public InputAdressingTypes {
    REGISTERS registerEsp;
    Word offset;
    REGISTERS register8b;
};









#endif