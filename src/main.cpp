#include "memory/Memory.hpp"
#include "memory/Registers.hpp"
#include "FunctionalUnit.hpp"
#include "Instructiondispatcher.hpp"

int main( int argc , char const *argv[] ) {
    Registers::GetRegisters()->WriteTo8bRegister( REGISTERS_8b::B , 64 );
    Instructiondispatcher::GetInstructionDispatcher()->Add( REGISTERS_8b::B );
    return 0;
}

