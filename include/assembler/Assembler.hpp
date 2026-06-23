#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "assembler/Lexer.hpp"
#include "assembler/Parser.hpp"
#include "assembler/MacroProcessor.hpp"
#include "SystemArchitecture.hpp"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Assembler {
public:
    // Ponto de entrada: recebe caminho do .asm e retorna bytes prontos para a VM
    vector<Word> Assemble(const string& filepath);

    // Tabela de simbolos publica (para exibir na GUI ou no terminal)
    unordered_map<string, uint16_t> symbolTable;

    // Lista de erros encontrados durante a montagem
    vector<string> errors;

private:
    // Passo 1: calcula o endereco de cada rotulo e preenche symbolTable
    void PassOne(const vector<ParsedLine>& lines);

    // Passo 2: resolve simbolos e gera os DecodedInstructions
    vector<DecodedInstruction> PassTwo(const vector<ParsedLine>& lines);

    // Converte um ParsedLine (ja com simbolos resolvidos) em DecodedInstruction
    DecodedInstruction ToDecodedInstruction(const ParsedLine& line);
};

#endif
