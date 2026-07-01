#pragma once

#include "assembler/MacroProcessor.hpp"
#include "assembler/Lexer.hpp"
#include "assembler/Parser.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;


struct EntryRecord {
    string name;       
    uint16_t localOffset;  
};
 

struct ExternRecord {
    string name;        
    vector<uint16_t>  useOffsets;  
};

struct ObjectModule {
    string name; 
    uint16_t length = 0;
    vector<EntryRecord>  entryTable;
    vector<ExternRecord> externTable;
    vector<uint8_t> text; 
    vector<uint16_t> relocTable;
    bool hasEntryPoint = false;
    uint16_t entryPoint = 0;    
    vector<string> errors;
};
 
// ---------------------------------------------------------------
// Modulo de saida do Linker.
//
// Modo ABSOLUTE  (Ligador-Relocador):
//   - image[] contem o programa completamente resolvido.
//   - relocTable esta vazia (Carregador Absoluto nao precisa).
//   - loadAddress tem o endereco exato onde deve ser carregado.
//
// Modo RELOCATABLE (Ligador apenas):
//   - image[] contem o programa com enderecos relativos ao inicio.
//   - relocTable lista offsets ainda pendentes de relocacao.
//   - O Carregador Relocador soma loadAddress a cada entrada.
// ---------------------------------------------------------------
struct LinkedModule {
    vector<uint8_t> image;        // bytes do programa combinado
    vector<uint16_t> relocTable;   // offsets ainda relocaveis (modo RELOCATABLE)
    uint16_t loadAddress = 0;
    bool hasEntry = false;
    uint16_t entryPoint = 0;
    vector<string> errors;
};

class AssemblerObj {
public:
    ObjectModule Assemble(const string& filepath);
    vector<string> errors;
private:
    map<string, uint16_t> symbolTable;
    vector<string> externSymbols;
    vector<pair<string,string>> entrySymbols;
    void PassOne(const vector<ParsedLine>& lines, ObjectModule& mod);
    void PassTwo(const vector<ParsedLine>& lines, ObjectModule& mod);
    vector<uint8_t> EncodeInstruction(const ParsedLine& line);
    bool IsExtern(const string& sym) const;
    static DecodedInstruction ToDecodedInstruction(const ParsedLine& line);
};
