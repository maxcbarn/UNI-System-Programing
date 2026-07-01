#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

using namespace std;

// Cada tipo de token que pode aparecer num arquivo .asm
enum class TokenType {
    LABEL,          // ex: "LOOP:"
    MNEMONIC,       // ex: "ADD", "LD", "JP"
    REGISTER_8B,    // A, B, C, D, E, H, L
    REGISTER_16B,   // AF, BC, DE, HL
    REGISTER_ESP,   // IX, IY, SP, PC
    NUMBER,         // ex: 10, 255, 0xFF
    SYMBOL,         // ex: "LOOP", "FIM" — referencia a rotulo ainda nao resolvida
    INDIRECT,       // ex: (HL), (1234), (IX+5), (IY-3)
    COMMA,          // ,
    END_OF_FILE,
    DIRECTIVE
};

struct Token {
    TokenType type;
    string    value;    // valor original do token (ex: "LOOP", "0xFF", "(IX+5)")
    int       line;     // numero da linha no arquivo (para mensagens de erro)
};

class Lexer {
public:
    // Le o arquivo .asm e retorna a lista de tokens
    vector<Token> Tokenize(const string& filepath);

private:
    vector<Token> TokenizeLine(const string& line, int lineNumber);
    Token         ClassifyWord(const string& word, int lineNumber);

    bool IsMnemonic(const string& word);
    bool IsRegister8b(const string& word);
    bool IsRegister16b(const string& word);
    bool IsRegisterEsp(const string& word);
    bool IsNumber(const string& word);
    bool IsDirective(const string& word);

    string ToUpper(const string& str);
    string StripComment(const string& line);
    string Trim(const string& str);
};

#endif
