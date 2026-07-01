#ifndef PARSER_HPP
#define PARSER_HPP

#include "assembler/Lexer.hpp"
#include "SystemArchitecture.hpp"
#include <string>
#include <vector>

using namespace std;

enum class DirectiveType {
    NONE,
    ENTRY,   
    EXTRN  
};

// Representa uma linha do .asm ja interpretada pelo parser
struct ParsedLine {
    string              label;          // rotulo definido nesta linha (vazio se nenhum)
    INSTRUCTIONS        instruction;
    INPUTADRESSINGTYPES addrMode;

    // Operandos (preenchidos conforme o modo de enderecamento)
    REGISTERS       reg8b_dest = A;    // registrador destino (LD A, B -> A)
    REGISTERS       reg8b_src  = A;    // registrador fonte   (LD A, B -> B)
    REGISTERS       reg16b     = AF;   // registrador 16 bits (PUSH BC -> BC)
    REGISTERS       regEsp     = IX;   // IX ou IY (para indexado)
    int8_t          offset     = 0;    // deslocamento indexado (IX+5 -> 5)
    uint16_t        value      = 0;    // valor imediato ou endereco direto

    string          symbolRef;         // nome do rotulo referenciado (JP LOOP -> "LOOP")
    bool            hasSymbol  = false; // true se symbolRef precisa ser resolvido no passo 2

    int             lineNumber = 0;
    bool            isDirective     = false;
    DirectiveType   directiveType   = DirectiveType::NONE;
    vector<string>  directiveSymbols; 
};



class Parser {
public:
    // Recebe os tokens do Lexer e retorna as linhas interpretadas
    vector<ParsedLine> Parse(const vector<Token>& tokens);

    // Retorna o tamanho em bytes de uma instrucao (usado no Passo 1)
    static int GetInstructionSize(const ParsedLine& line);

private:
    ParsedLine ParseLine(const vector<Token>& lineTokens);
    ParsedLine ParseLD(const vector<Token>& ops, const string& label, int lineNum);
    ParsedLine ParseDirective(const vector<Token>& toks, int startIdx, DirectiveType dtype, const string& label, int lineNum);

    static REGISTERS ToReg(const Token& tok);
    static uint16_t      ToValue(const Token& tok);

    // Resultado de analisar um token INDIRECT como "(HL)", "(IX+5)", "(1234)"
    struct IndirectResult {
        enum class Type { HL, INDEXED_IX, INDEXED_IY, DIRECT_ADDR } type;
        int8_t   offset = 0;
        uint16_t addr   = 0;
    };
    static IndirectResult ParseIndirect(const string& s);
};

#endif
