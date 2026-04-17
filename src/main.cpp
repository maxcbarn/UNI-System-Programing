#include "memory/Memory.hpp"
#include "memory/Registers.hpp"
#include "FunctionalUnit.hpp"

int main( int argc , char const *argv[] ) {
    Registers::GetRegisters()->WriteTo16bRegisterLow( REGISTERS_16b::BC , 64 );
    Registers::GetRegisters()->WriteTo16bRegisterHigh( REGISTERS_16b::BC , 128 );
    FunctionalUnit::GetFunctionalUnit()->PushStack(REGISTERS_16b::BC);
    FunctionalUnit::GetFunctionalUnit()->PopStack( REGISTERS_16b::DE );
    DoubleWord a = Registers::GetRegisters()->ReadFrom16bRegisterLow( REGISTERS_16b::DE );
    DoubleWord b = Registers::GetRegisters()->ReadFrom16bRegisterHigh( REGISTERS_16b::DE );
    DoubleWord c = Registers::GetRegisters()->ReadFrom16bRegisterLow( REGISTERS_16b::BC );
    DoubleWord d = Registers::GetRegisters()->ReadFrom16bRegisterHigh( REGISTERS_16b::BC );
    return 0;
}

