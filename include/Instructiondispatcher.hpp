#ifndef INSTRUCTIONDISPATCHER_HPP
#define INSTRUCTIONDISPATCHER_HPP

#include "SystemArchitecture.hpp"

class Instructiondispatcher {
    private:
        Instructiondispatcher();
        static inline Instructiondispatcher * instructiondispatcher = nullptr;
    public: 
        ~Instructiondispatcher();
        static Instructiondispatcher * GetInstructionDispatcher();
        void Add( REGISTERS_8b register8b );
        void Sub( REGISTERS_8b register8b );
        void Inc( REGISTERS_8b register8b );
        void Dec( REGISTERS_8b register8b );
        void And( REGISTERS_8b register8b );
        void Or( REGISTERS_8b register8b );
        void Xor( REGISTERS_8b register8b );
        void Cp( REGISTERS_8b register8b );
        void Nop();
        void PushStack( REGISTERS_16b register16b );
        void PopStack( REGISTERS_16b register16b );
};

#endif