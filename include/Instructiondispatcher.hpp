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
};

#endif