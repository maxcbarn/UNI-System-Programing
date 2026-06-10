#include "assembler/Parser.hpp"
#include "instructions/AdressingTypesFactoryInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include <iostream>
#include <map>
#include <stdexcept>

// ---------------------------------------------------------------
// Conversores de Token para tipos da arquitetura
// ---------------------------------------------------------------

REGISTERS Parser::ToReg(const Token& tok) {
    if (tok.value == "A") return A;
    if (tok.value == "B") return B;
    if (tok.value == "C") return C;
    if (tok.value == "D") return D;
    if (tok.value == "E") return E;
    if (tok.value == "H") return H;
    if (tok.value == "L") return L;
    if (tok.value == "AF") return AF;
    if (tok.value == "BC") return BC;
    if (tok.value == "DE") return DE;
    if (tok.value == "HL") return HL;
    if (tok.value == "IX") return IX;
    if (tok.value == "IY") return IY;
    if (tok.value == "SP") return SP;
    if (tok.value == "PC") return PC;
    cerr << "[PARSER ERROR] linha " << tok.line << ": registrador desconhecido: " << tok.value << endl;
    return A;
}


uint16_t Parser::ToValue(const Token& tok) {
    const string& s = tok.value;
    try {
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
            return (uint16_t)stoul(s, nullptr, 16);
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'b' || s[1] == 'B'))
            return (uint16_t)stoul(s.substr(2), nullptr, 2);
        return (uint16_t)stoul(s, nullptr, 10);
    } catch (...) {
        cerr << "[PARSER ERROR] linha " << tok.line << ": valor invalido: " << s << endl;
        return 0;
    }
}

// ---------------------------------------------------------------
// Analisa um token INDIRECT: "(HL)", "(IX+5)", "(IY-3)", "(1234)"
// ---------------------------------------------------------------

Parser::IndirectResult Parser::ParseIndirect(const string& s) {
    // Remove parenteses
    string inner = s.substr(1, s.size() - 2);

    if (inner == "HL")
        return { IndirectResult::Type::HL, 0, 0 };

    if (inner.size() >= 2 && inner.substr(0, 2) == "IX") {
        int8_t off = 0;
        if (inner.size() > 2)
            off = (int8_t)stoi(inner.substr(2)); // captura "+5" ou "-3"
        return { IndirectResult::Type::INDEXED_IX, off, 0 };
    }

    if (inner.size() >= 2 && inner.substr(0, 2) == "IY") {
        int8_t off = 0;
        if (inner.size() > 2)
            off = (int8_t)stoi(inner.substr(2));
        return { IndirectResult::Type::INDEXED_IY, off, 0 };
    }

    // Endereco direto: "(1234)" ou "(0xFF)"
    uint16_t addr = 0;
    try {
        if (inner.size() > 2 && inner[0] == '0' && (inner[1] == 'x' || inner[1] == 'X'))
            addr = (uint16_t)stoul(inner, nullptr, 16);
        else
            addr = (uint16_t)stoul(inner, nullptr, 10);
    } catch (...) {
        cerr << "[PARSER ERROR] endereco indireto invalido: " << s << endl;
    }
    return { IndirectResult::Type::DIRECT_ADDR, 0, addr };
}

// ---------------------------------------------------------------
// Pega o tamanho em bytes de cada instrução
// ---------------------------------------------------------------

int Parser::GetInstructionSize(const ParsedLine& line) {
    AdressingTypesFactoryInstructions* factory =
        AdressingTypesFactoryInstructions::GetAdressingTypesFactoryInstructions();
    AdressingTypesInstructions* encoder =
        factory->GetAdressingTypeInstructions(line.addrMode);
    int size = (int)encoder->GetInstructionWordQuantity(line.instruction) + 1;
    delete encoder;
    return size;
}

// ---------------------------------------------------------------
// Parser da instrucao LD (muitos formatos possiveis)
// ---------------------------------------------------------------

ParsedLine Parser::ParseLD(const vector<Token>& ops, const string& label, int lineNum) {
    ParsedLine pline{};
    pline.label      = label;
    pline.lineNumber = lineNum;

    if (ops.size() < 2) {
        cerr << "[PARSER ERROR] linha " << lineNum << ": LD precisa de dois operandos\n";
        return pline;
    }

    const Token& dest = ops[0];
    const Token& src  = ops[1];

    // LD reg8b, reg8b  →  LDREGTOREG, IMPLICIT
    if (dest.type == TokenType::REGISTER_8B && src.type == TokenType::REGISTER_8B) {
        pline.instruction = LDREGTOREG;
        pline.addrMode    = IMPLICIT;
        pline.reg8b_dest  = ToReg(dest);
        pline.reg8b_src   = ToReg(src);
        return pline;
    }

    // LD reg8b, numero  →  LDVALTOREG, IMEDIATE
    if (dest.type == TokenType::REGISTER_8B && src.type == TokenType::NUMBER) {
        pline.instruction = LDVALTOREG;
        pline.addrMode    = IMEDIATE;
        pline.reg8b_dest  = ToReg(dest);
        pline.value       = ToValue(src);
        return pline;
    }

    // LD reg8b, (...)  →  LDMEMTOREG com modo dependendo do tipo de indireto
    if (dest.type == TokenType::REGISTER_8B && src.type == TokenType::INDIRECT) {
        pline.reg8b_dest = ToReg(dest);
        pline.instruction = LDMEMTOREG;
        auto ind = ParseIndirect(src.value);

        if (ind.type == IndirectResult::Type::HL) {
            pline.addrMode = INDIRECT_REGISTER;
        } else if (ind.type == IndirectResult::Type::INDEXED_IX) {
            pline.addrMode = INDEXED;
            pline.regEsp   = IX;
            pline.offset   = ind.offset;
        } else if (ind.type == IndirectResult::Type::INDEXED_IY) {
            pline.addrMode = INDEXED;
            pline.regEsp   = IY;
            pline.offset   = ind.offset;
        } else { // DIRECT_ADDR
            pline.addrMode = DIRECT;
            pline.value    = ind.addr;
        }
        return pline;
    }

    // LD (...), reg8b  →  LDREGTOMEM com modo dependendo do tipo de indireto
    if (dest.type == TokenType::INDIRECT && src.type == TokenType::REGISTER_8B) {
        pline.reg8b_src = ToReg(src);
        pline.instruction = LDREGTOMEM;
        auto ind = ParseIndirect(dest.value);

        if (ind.type == IndirectResult::Type::HL) {
            pline.addrMode = INDIRECT_REGISTER;
        } else if (ind.type == IndirectResult::Type::INDEXED_IX) {
            pline.addrMode = INDEXED;
            pline.regEsp   = IX;
            pline.offset   = ind.offset;
        } else if (ind.type == IndirectResult::Type::INDEXED_IY) {
            pline.addrMode = INDEXED;
            pline.regEsp   = IY;
            pline.offset   = ind.offset;
        } else { // DIRECT_ADDR
            pline.addrMode = DIRECT;
            pline.value    = ind.addr;
        }
        return pline;
    }

    cerr << "[PARSER ERROR] linha " << lineNum << ": formato de LD desconhecido\n";
    return pline;
}

// ---------------------------------------------------------------
// Interpreta um conjunto de tokens de uma linha
// ---------------------------------------------------------------

ParsedLine Parser::ParseLine(const vector<Token>& toks) {
    ParsedLine pline{};
    if (toks.empty()) return pline;

    int i = 0;

    // Rotulo opcional no inicio da linha
    if (toks[i].type == TokenType::LABEL) {
        pline.label = toks[i].value;
        i++;
    }

    if (i >= (int)toks.size()) return pline; // linha so com rotulo

    if (toks[i].type != TokenType::MNEMONIC) {
        cerr << "[PARSER ERROR] linha " << toks[i].line << ": esperado mnemônico, encontrado: " << toks[i].value << endl;
        return pline;
    }

    string mnem       = toks[i].value;
    pline.lineNumber  = toks[i].line;
    i++;

    // Coleta operandos (remove virgulas)
    vector<Token> ops;
    for (; i < (int)toks.size(); i++)
        if (toks[i].type != TokenType::COMMA)
            ops.push_back(toks[i]);

    // LD tem tratamento especial
    if (mnem == "LD")
        return ParseLD(ops, pline.label, pline.lineNumber);

    // Instrucoes sem operandos
    if (mnem == "NOP") { pline.instruction = NOP; pline.addrMode = IMPLICIT; return pline; }
    if (mnem == "HLT") { pline.instruction = HLT; pline.addrMode = IMPLICIT; return pline; }
    if (mnem == "RET") { pline.instruction = RET; pline.addrMode = IMPLICIT; return pline; }

    // Instrucoes com um registrador 16b (PUSH/POP)
    if (mnem == "PUSH" || mnem == "POP") {
        pline.instruction = (mnem == "PUSH") ? PUSH : POP;
        pline.addrMode    = IMPLICIT;
        if (!ops.empty() && ops[0].type == TokenType::REGISTER_16B)
            pline.reg16b = ToReg(ops[0]);
        else
            cerr << "[PARSER ERROR] linha " << pline.lineNumber << ": " << mnem << " exige registrador 16b\n";
        return pline;
    }

    // Instrucoes com um registrador 8b (ADD, SUB, AND, OR, XOR, CP, INC, DEC)
    if (mnem == "ADD" || mnem == "SUB" || mnem == "AND" || mnem == "OR"  ||
        mnem == "XOR" || mnem == "CP"  || mnem == "INC" || mnem == "DEC") {
        static const map<string, INSTRUCTIONS> opMap = {
            {"ADD", ADD}, {"SUB", SUB}, {"AND", AND}, {"OR",  OR},
            {"XOR", XOR}, {"CP",  CP},  {"INC", INC}, {"DEC", DEC}
        };
        pline.instruction = opMap.at(mnem);
        pline.addrMode    = IMPLICIT;

        if (ops.size() >= 2 && ops[0].type == TokenType::REGISTER_8B && ops[1].type == TokenType::REGISTER_8B) {
            pline.reg8b_src = ToReg(ops[1]); // Usa o segundo operando 
        } else if (ops.size() == 1 && ops[0].type == TokenType::REGISTER_8B) {
            pline.reg8b_src = ToReg(ops[0]); // Usa o primeiro operando 
        } else {
            cerr << "[PARSER ERROR] linha " << pline.lineNumber << ": " << mnem << " exige registrador 8b\n";
        }
        return pline;
    }

    // Instrucoes de salto/chamada: JP, JPOFFSET, CALL
    if (mnem == "JP" || mnem == "JPOFFSET" || mnem == "CALL") {
        static const map<string, INSTRUCTIONS> opMap = {
            {"JP", JP}, {"JPOFFSET", JPOFFSET}, {"CALL", CALL}
        };
        pline.instruction = opMap.at(mnem);
        pline.addrMode    = DIRECT;

        if (!ops.empty()) {
            if (ops[0].type == TokenType::SYMBOL) {
                // Referencia a rotulo — sera resolvida no Passo 2
                pline.hasSymbol = true;
                pline.symbolRef = ops[0].value;
            } else if (ops[0].type == TokenType::NUMBER) {
                pline.value = ToValue(ops[0]);
            }
        }
        return pline;
    }

    cerr << "[PARSER ERROR] linha " << pline.lineNumber << ": mnemônico desconhecido: " << mnem << endl;
    return pline;
}

// ---------------------------------------------------------------
// Entrada principal: agrupa tokens por linha e interpreta cada uma
// ---------------------------------------------------------------

vector<ParsedLine> Parser::Parse(const vector<Token>& tokens) {
    vector<ParsedLine> result;

    // Agrupa tokens pelo numero da linha
    vector<vector<Token>> byLine;
    vector<Token> current;

    int currentLine = tokens.empty() ? 0 : tokens[0].line;

    for (const auto& tok : tokens) {
        if (tok.type == TokenType::END_OF_FILE) {
            if (!current.empty()) byLine.push_back(current);
            break;
        }
        if (tok.line != currentLine) {
            if (!current.empty()) byLine.push_back(current);
            current.clear();
            currentLine = tok.line;
        }
        current.push_back(tok);
    }

    // Interpreta cada linha
    for (const auto& lineTokens : byLine) {
        ParsedLine pline = ParseLine(lineTokens);
        // Ignora linhas vazias (so tinham comentarios)
        if (!pline.label.empty() || pline.lineNumber > 0)
            result.push_back(pline);
    }

    return result;
}
