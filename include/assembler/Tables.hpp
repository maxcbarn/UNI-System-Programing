#ifndef TABLES_HPP
#define TABLES_HPP

#include "SystemArchitecture.hpp"
#include <vector>
#include <string>
#include <bitset>

using namespace std;


struct OpcodeTableEntry {
    INSTRUCTIONS instruction;
    Word opcode;
    size_t wordQuantity;
    string name;
    string Show() {
        return name + " (opcode: 0b" + bitset<8>( opcode ).to_string() + ", word quantity: " + to_string( wordQuantity ) + ")";
    }
};

struct RegisterTableEntry {
    REGISTERS registerEnum;
    Word code;
    string name;
    string Show() {
        return name + " (code: 0b" + bitset<8>( code ).to_string() + ")";
    }
};



class Tables
{
    private:
        Tables();
        inline static Tables * tables = nullptr;
        vector<OpcodeTableEntry> opcodeTable;
        vector<RegisterTableEntry> registerTable;
    public:
        ~Tables();
        static Tables * GetTables();
        void ShowOpcodeTable();
        void ShowRegisterTable();
        void CreateTables( INPUTADRESSINGTYPES addresingType );
};


#endif