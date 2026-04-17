#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <vector>
#include <cstdint>
#include "SystemArchitecture.hpp"


using namespace std;

class Registers {
    private:
        Adress startStack = MEM_SIZE_BYTES, endStack = MEM_SIZE_BYTES - 512; 
        vector< Word > generalUse8b;
        vector< DoubleWord > generalUse16b;
        Word accumulator;
        Word flag;
        DoubleWord stackPtr = MEM_SIZE_BYTES;
        Registers();
        inline static Registers * registers = nullptr;
    public:
        ~Registers();
        static Registers * GetRegisters();
        void WriteTo8bRegister( REGISTERS_8b registerEnum , Word data );
        Word ReadFrom16bRegisterLow( REGISTERS_16b registerEnum );
        Word ReadFrom16bRegisterHigh( REGISTERS_16b registerEnum );
        void WriteToAccumulator( Word data );
        Word ReadToAccumulator();
        Word ReadFrom8bRegister( REGISTERS_8b registerEnum );
        void WriteTo16bRegister( REGISTERS_16b registerEnum , DoubleWord data );
        void WriteTo16bRegisterLow( REGISTERS_16b registerEnum , Word data );
        void WriteTo16bRegisterHigh( REGISTERS_16b registerEnum , Word data );
        Adress GetStackPtr();
        bool IncreaseStackPtr();
        bool DecreaseStackPtr();
};



#endif