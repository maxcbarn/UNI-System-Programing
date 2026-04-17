#include "memory/Memory.hpp"
#include <cmath>
#include <tuple>

using namespace std;

auto adressToMem = []( Adress adress , size_t sqrMatrixSize ) {
    return make_pair< size_t , size_t >( adress / sqrMatrixSize , adress % sqrMatrixSize );
};

auto memToAdress = []( size_t row , size_t collmun , size_t sqrMatrixSize ) {
    return ( Adress ) row * sqrMatrixSize + collmun;
};

Memory::Memory() {
    sqrMatrixSize = ( size_t )sqrt( ( float )MEM_SIZE_BYTES ) + 1;
    memory = vector< vector< Word  > >( sqrMatrixSize , vector< Word  >( sqrMatrixSize , 0 ) ) ;
}

Memory::~Memory() {
    memory.clear();
}


Memory * Memory::GetMemory() {
    if( memoryPtr != nullptr ) {
        return memoryPtr; 
    }
    memoryPtr = new Memory();
    return memoryPtr;
}

Word Memory::ReadMemory( Adress adress ) {
    pair< size_t , size_t > matrixPositon = adressToMem( adress , sqrMatrixSize );
    return memory[matrixPositon.first][matrixPositon.second];
}



void Memory::ModifyMemory( Adress adress , Word word ) {
    pair< size_t , size_t > matrixPositon = adressToMem( adress , sqrMatrixSize );
    memory[matrixPositon.first][matrixPositon.second] = word;
}


