#include "assembler/Lexer.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// ---------------------------------------------------------------
// Utilitarios
// ---------------------------------------------------------------

string Lexer::ToUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

string Lexer::StripComment(const string& line) {
    size_t pos = line.find(';');
    if (pos != string::npos)
        return line.substr(0, pos);
    return line;
}

string Lexer::Trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

// ---------------------------------------------------------------
// Classificacao de palavras
// ---------------------------------------------------------------

bool Lexer::IsMnemonic(const string& word) {
    static const vector<string> mnemonics = {
        "NOP", "HLT",
        "ADD", "SUB", "AND", "OR", "XOR", "CP",
        "INC", "DEC",
        "PUSH", "POP",
        "JP", "JPOFFSET", "CALL", "RET",
        "LD"
    };
    for (const auto& m : mnemonics)
        if (word == m) return true;
    return false;
}

bool Lexer::IsRegister8b(const string& word) {
    static const vector<string> regs = { "A", "B", "C", "D", "E", "H", "L" };
    for (const auto& r : regs)
        if (word == r) return true;
    return false;
}

bool Lexer::IsRegister16b(const string& word) {
    static const vector<string> regs = { "AF", "BC", "DE", "HL" };
    for (const auto& r : regs)
        if (word == r) return true;
    return false;
}

bool Lexer::IsRegisterEsp(const string& word) {
    static const vector<string> regs = { "IX", "IY", "SP", "PC" };
    for (const auto& r : regs)
        if (word == r) return true;
    return false;
}

bool Lexer::IsNumber(const string& word) {
    if (word.empty()) return false;
    if (word.size() > 2 && word[0] == '0' && (word[1] == 'x' || word[1] == 'X'))
        return all_of(word.begin() + 2, word.end(), ::isxdigit);
    if (word.size() > 2 && word[0] == '0' && (word[1] == 'b' || word[1] == 'B'))
        return all_of(word.begin() + 2, word.end(), [](char c){ return c == '0' || c == '1'; });
    return all_of(word.begin(), word.end(), ::isdigit);
}

// ---------------------------------------------------------------
// NOVO: reconhece diretivas de ligacao
// ---------------------------------------------------------------
bool Lexer::IsDirective(const string& word) {
    return word == "ENTRY" || word == "EXTRN";
}

// ---------------------------------------------------------------
// Classifica uma palavra isolada em um Token
// ---------------------------------------------------------------
Token Lexer::ClassifyWord(const string& word, int lineNumber) {
    string upper = ToUpper(word);

    // Diretivas ENTRY / EXTRN devem ser verificadas ANTES de SYMBOL
    if (IsDirective(upper))    return { TokenType::DIRECTIVE,    upper, lineNumber };

    if (IsMnemonic(upper))     return { TokenType::MNEMONIC,     upper, lineNumber };
    if (IsRegister16b(upper))  return { TokenType::REGISTER_16B, upper, lineNumber };
    if (IsRegisterEsp(upper))  return { TokenType::REGISTER_ESP, upper, lineNumber };
    if (IsRegister8b(upper))   return { TokenType::REGISTER_8B,  upper, lineNumber };
    if (IsNumber(word))        return { TokenType::NUMBER,        word,  lineNumber };

    // Palavra desconhecida e um simbolo (rotulo referenciado ou nome externo)
    return { TokenType::SYMBOL, upper, lineNumber };
}

// ---------------------------------------------------------------
// Tokenizacao de uma linha
// ---------------------------------------------------------------
vector<Token> Lexer::TokenizeLine(const string& rawLine, int lineNumber) {
    vector<Token> tokens;

    string line = Trim(StripComment(rawLine));
    if (line.empty()) return tokens;

    // Substitui virgulas por " , " para facilitar o split
    string processed;
    for (char c : line) {
        if (c == ',') processed += " , ";
        else          processed += c;
    }

    istringstream stream(processed);
    string word;

    while (stream >> word) {
        if (word == ",") {
            tokens.push_back({ TokenType::COMMA, ",", lineNumber });
            continue;
        }

        if (word.back() == ':') {
            string label = word.substr(0, word.size() - 1);
            tokens.push_back({ TokenType::LABEL, ToUpper(label), lineNumber });
            continue;
        }

        if (word.front() == '(') {
            string indirect = word;
            while (indirect.back() != ')' && stream >> word)
                indirect += word;
            tokens.push_back({ TokenType::INDIRECT, ToUpper(indirect), lineNumber });
            continue;
        }

        tokens.push_back(ClassifyWord(word, lineNumber));
    }

    return tokens;
}

// ---------------------------------------------------------------
// Entrada principal: le o arquivo e tokeniza linha a linha
// ---------------------------------------------------------------
vector<Token> Lexer::Tokenize(const string& filepath) {
    vector<Token> allTokens;

    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "[LEXER ERROR] Nao foi possivel abrir o arquivo: " << filepath << endl;
        return allTokens;
    }

    string line;
    int lineNumber = 1;
    while (getline(file, line)) {
        vector<Token> lineTokens = TokenizeLine(line, lineNumber);
        allTokens.insert(allTokens.end(), lineTokens.begin(), lineTokens.end());
        lineNumber++;
    }

    allTokens.push_back({ TokenType::END_OF_FILE, "", lineNumber });
    return allTokens;
}