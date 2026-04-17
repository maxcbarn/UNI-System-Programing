#ifndef MEMORY_HPP
#define MEMORY_HPP



#include <vector>
#include "SystemArchitecture.hpp"


using namespace std;

class Memory {
    private:
        vector< vector< Word > > memory; 
        Memory();
        static inline Memory * memoryPtr = nullptr; 
        size_t sqrMatrixSize;
    public:
        ~Memory();
        static Memory * GetMemory();
        Word ReadMemory( Adress adress );
        void ModifyMemory( Adress adress , Word word );
};





#endif