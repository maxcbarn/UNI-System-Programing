#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include <vector>
#include <cstdint>
#include "SystemArchitecture.hpp"
#include <string>

// Teste de Commit para o GitHub
using namespace std;

class Registers {
    private:
        Adress startStack = MEM_SIZE_BYTES, endStack = MEM_SIZE_BYTES - 512, programCounter = 0, endProgram = 0, startMemory = 0; 
        vector< Word > generalUse8b;
        Word flag = 0;
        DoubleWord stackPtr = MEM_SIZE_BYTES, indexX = 0, indexY = 0;
        Registers();
        inline static Registers * registers = nullptr;
    public:
        ~Registers();
        static Registers * GetRegisters();
        static void ResetRegisters();
        void WriteTo8bRegister( REGISTERS registerEnum , Word data );
        void WriteToAccumulator( Word data );
        Word ReadFromAccumulator();
        Word ReadFrom8bRegister( REGISTERS registerEnum );
        DoubleWord ReadFrom16bRegister( REGISTERS registerEnum );
        void WriteTo16bRegister( REGISTERS registerEnum, DoubleWord data );
        Adress GetStackPtr();
        bool IncreaseStackPtr();
        bool DecreaseStackPtr();
        void IncreaseProgramCounter();
        void SetProgramCounter( Adress adress );
        void SetProgramSize( size_t size );
        Adress GetProgramCounter();
        void AddFlag( FLAGS flag );
        void ClearFlag( FLAGS flag );
        string FlagsToTerminal();
        Adress GetStartMemory();
        DoubleWord ReadFromEspRegister( REGISTERS registerEnum );
        void WriteToEspRegister( REGISTERS registerEnum, DoubleWord data );
};



#endif