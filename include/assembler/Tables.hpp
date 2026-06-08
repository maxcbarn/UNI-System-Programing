#ifndef TABLES_HPP
#define TABLES_HPP

#include "SystemArchitecture.hpp"
#include <vector>
#include <string>

using namespace std;


struct OpcodeTableEntry {
    INSTRUCTIONS instruction;
    Word opcode;
    size_t wordQuantity;
    string name;
};

struct RegisterTableEntry {
    REGISTERS registerEnum;
    Word code;
    string name;
};



class Tables
{
    private:
        Tables();
        inline static Tables * tables = nullptr;
        vector<OpcodeTableEntry> opcodeTable;
    public:
        ~Tables();
        static Tables * GetTables();
};


#endif