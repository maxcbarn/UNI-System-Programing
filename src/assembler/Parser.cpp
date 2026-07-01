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
    if (tok.value == "A")  return A;
    if (tok.value == "B")  return B;
    if (tok.value == "C")  return C;
    if (tok.value == "D")  return D;
    if (tok.value == "E")  return E;
    if (tok.value == "H")  return H;
    if (tok.value == "L")  return L;
    if (tok.value == "AF") return AF;
    if (tok.value == "BC") return BC;
    if (tok.value == "DE") return DE;
    if (tok.value == "HL") return HL;
    if (tok.value == "IX") return IX;
    if (tok.value == "IY") return IY;
    if (tok.value == "SP") return SP;
    if (tok.value == "PC") return PC;
    cerr << "[PARSER ERROR] linha " << tok.line
         << ": registrador desconhecido: " << tok.value << endl;
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
        cerr << "[PARSER ERROR] linha " << tok.line
             << ": valor invalido: " << s << endl;
        return 0;
    }
}

// ---------------------------------------------------------------
// Analisa um token INDIRECT
// ---------------------------------------------------------------

Parser::IndirectResult Parser::ParseIndirect(const string& s) {
    string inner = s.substr(1, s.size() - 2);

    if (inner == "HL")
        return { IndirectResult::Type::HL, 0, 0 };

    if (inner.size() >= 2 && inner.substr(0, 2) == "IX") {
        int8_t off = 0;
        if (inner.size() > 2)
            off = (int8_t)stoi(inner.substr(2));
        return { IndirectResult::Type::INDEXED_IX, off, 0 };
    }

    if (inner.size() >= 2 && inner.substr(0, 2) == "IY") {
        int8_t off = 0;
        if (inner.size() > 2)
            off = (int8_t)stoi(inner.substr(2));
        return { IndirectResult::Type::INDEXED_IY, off, 0 };
    }

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
// Tamanho de cada instrucao em bytes
// ---------------------------------------------------------------

int Parser::GetInstructionSize(const ParsedLine& line) {
    // Diretivas nao geram codigo
    if (line.isDirective) return 0;

    AdressingTypesFactoryInstructions* factory =
        AdressingTypesFactoryInstructions::GetAdressingTypesFactoryInstructions();
    AdressingTypesInstructions* encoder =
        factory->GetAdressingTypeInstructions(line.addrMode);
    int size = (int)encoder->GetInstructionWordQuantity(line.instruction) + 1;
    delete encoder;
    return size;
}

// ---------------------------------------------------------------
// NOVO: analisa uma diretiva ENTRY ou EXTRN
//
//   ENTRY  sym1, sym2, ...
//   EXTRN  sym1, sym2, ...
//
// Os simbolos que seguem a diretiva podem ter TokenType SYMBOL
// (o mais comum), mas aceitamos qualquer token nao-COMMA como nome.
// ---------------------------------------------------------------

ParsedLine Parser::ParseDirective(const vector<Token>& toks, int startIdx,
                                  DirectiveType dtype,
                                  const string& label, int lineNum) {
    ParsedLine pline{};
    pline.label          = label;
    pline.lineNumber     = lineNum;
    pline.isDirective    = true;
    pline.directiveType  = dtype;

    for (int i = startIdx; i < (int)toks.size(); ++i) {
        if (toks[i].type == TokenType::COMMA) continue;
        // Qualquer token nao-virgula e tratado como nome de simbolo
        pline.directiveSymbols.push_back(toks[i].value);
    }

    if (pline.directiveSymbols.empty()) {
        cerr << "[PARSER ERROR] linha " << lineNum
             << ": diretiva "
             << (dtype == DirectiveType::ENTRY ? "ENTRY" : "EXTRN")
             << " sem simbolos\n";
    }

    return pline;
}

// ---------------------------------------------------------------
// LD (muitos formatos)
// ---------------------------------------------------------------

ParsedLine Parser::ParseLD(const vector<Token>& ops, const string& label, int lineNum) {
    ParsedLine pline{};
    pline.label      = label;
    pline.lineNumber = lineNum;

    if (ops.size() < 2) {
        cerr << "[PARSER ERROR] linha " << lineNum
             << ": LD precisa de dois operandos\n";
        return pline;
    }

    const Token& dest = ops[0];
    const Token& src  = ops[1];

    if (dest.type == TokenType::REGISTER_8B && src.type == TokenType::REGISTER_8B) {
        pline.instruction = LDREGTOREG;
        pline.addrMode    = IMPLICIT;
        pline.reg8b_dest  = ToReg(dest);
        pline.reg8b_src   = ToReg(src);
        return pline;
    }

    if (dest.type == TokenType::REGISTER_8B && src.type == TokenType::NUMBER) {
        pline.instruction = LDVALTOREG;
        pline.addrMode    = IMEDIATE;
        pline.reg8b_dest  = ToReg(dest);
        pline.value       = ToValue(src);
        return pline;
    }

    if (dest.type == TokenType::REGISTER_8B && src.type == TokenType::INDIRECT) {
        pline.reg8b_dest  = ToReg(dest);
        pline.instruction = LDMEMTOREG;
        auto ind = ParseIndirect(src.value);
        if (ind.type == IndirectResult::Type::HL) {
            pline.addrMode = INDIRECT_REGISTER;
        } else if (ind.type == IndirectResult::Type::INDEXED_IX) {
            pline.addrMode = INDEXED; pline.regEsp = IX; pline.offset = ind.offset;
        } else if (ind.type == IndirectResult::Type::INDEXED_IY) {
            pline.addrMode = INDEXED; pline.regEsp = IY; pline.offset = ind.offset;
        } else {
            pline.addrMode = DIRECT; pline.value = ind.addr;
        }
        return pline;
    }

    if (dest.type == TokenType::INDIRECT && src.type == TokenType::REGISTER_8B) {
        pline.reg8b_src   = ToReg(src);
        pline.instruction = LDREGTOMEM;
        auto ind = ParseIndirect(dest.value);
        if (ind.type == IndirectResult::Type::HL) {
            pline.addrMode = INDIRECT_REGISTER;
        } else if (ind.type == IndirectResult::Type::INDEXED_IX) {
            pline.addrMode = INDEXED; pline.regEsp = IX; pline.offset = ind.offset;
        } else if (ind.type == IndirectResult::Type::INDEXED_IY) {
            pline.addrMode = INDEXED; pline.regEsp = IY; pline.offset = ind.offset;
        } else {
            pline.addrMode = DIRECT; pline.value = ind.addr;
        }
        return pline;
    }

    cerr << "[PARSER ERROR] linha " << lineNum
         << ": formato de LD desconhecido\n";
    return pline;
}

// ---------------------------------------------------------------
// ParseLine — ponto de entrada para uma linha
// ---------------------------------------------------------------

ParsedLine Parser::ParseLine(const vector<Token>& toks) {
    ParsedLine pline{};
    if (toks.empty()) return pline;

    int i = 0;

    // Rotulo opcional
    if (toks[i].type == TokenType::LABEL) {
        pline.label = toks[i].value;
        i++;
    }

    if (i >= (int)toks.size()) return pline;

    // -------------------------------------------------------
    // NOVO: diretiva de ligacao (ENTRY / EXTRN)
    // -------------------------------------------------------
    if (toks[i].type == TokenType::DIRECTIVE) {
        DirectiveType dtype = (toks[i].value == "ENTRY")
                              ? DirectiveType::ENTRY
                              : DirectiveType::EXTRN;
        return ParseDirective(toks, i + 1, dtype, pline.label, toks[i].line);
    }

    // Mnemônico obrigatorio a partir daqui
    if (toks[i].type != TokenType::MNEMONIC) {
        cerr << "[PARSER ERROR] linha " << toks[i].line
             << ": esperado mnemônico, encontrado: " << toks[i].value << endl;
        return pline;
    }

    string mnem      = toks[i].value;
    pline.lineNumber = toks[i].line;
    i++;

    // Coleta operandos (remove virgulas)
    vector<Token> ops;
    for (; i < (int)toks.size(); i++)
        if (toks[i].type != TokenType::COMMA)
            ops.push_back(toks[i]);

    if (mnem == "LD")
        return ParseLD(ops, pline.label, pline.lineNumber);

    if (mnem == "NOP") { pline.instruction = NOP; pline.addrMode = IMPLICIT; return pline; }
    if (mnem == "HLT") { pline.instruction = HLT; pline.addrMode = IMPLICIT; return pline; }
    if (mnem == "RET") { pline.instruction = RET; pline.addrMode = IMPLICIT; return pline; }

    if (mnem == "PUSH" || mnem == "POP") {
        pline.instruction = (mnem == "PUSH") ? PUSH : POP;
        pline.addrMode    = IMPLICIT;
        if (!ops.empty() && ops[0].type == TokenType::REGISTER_16B)
            pline.reg16b = ToReg(ops[0]);
        else
            cerr << "[PARSER ERROR] linha " << pline.lineNumber
                 << ": " << mnem << " exige registrador 16b\n";
        return pline;
    }

    if (mnem == "ADD" || mnem == "SUB" || mnem == "AND" || mnem == "OR"  ||
        mnem == "XOR" || mnem == "CP"  || mnem == "INC" || mnem == "DEC") {
        static const map<string, INSTRUCTIONS> opMap = {
            {"ADD", ADD}, {"SUB", SUB}, {"AND", AND}, {"OR",  OR},
            {"XOR", XOR}, {"CP",  CP},  {"INC", INC}, {"DEC", DEC}
        };
        pline.instruction = opMap.at(mnem);
        pline.addrMode    = IMPLICIT;
        if (ops.size() >= 2 &&
            ops[0].type == TokenType::REGISTER_8B &&
            ops[1].type == TokenType::REGISTER_8B) {
            pline.reg8b_src = ToReg(ops[1]);
        } else if (ops.size() == 1 && ops[0].type == TokenType::REGISTER_8B) {
            pline.reg8b_src = ToReg(ops[0]);
        } else {
            cerr << "[PARSER ERROR] linha " << pline.lineNumber
                 << ": " << mnem << " exige registrador 8b\n";
        }
        return pline;
    }

    if (mnem == "JP" || mnem == "JPOFFSET" || mnem == "CALL") {
        static const map<string, INSTRUCTIONS> opMap = {
            {"JP", JP}, {"JPOFFSET", JPOFFSET}, {"CALL", CALL}
        };
        pline.instruction = opMap.at(mnem);
        pline.addrMode    = DIRECT;
        if (!ops.empty()) {
            if (ops[0].type == TokenType::SYMBOL) {
                pline.hasSymbol = true;
                pline.symbolRef = ops[0].value;
            } else if (ops[0].type == TokenType::NUMBER) {
                pline.value = ToValue(ops[0]);
            }
        }
        return pline;
    }

    cerr << "[PARSER ERROR] linha " << pline.lineNumber
         << ": mnemônico desconhecido: " << mnem << endl;
    return pline;
}

// ---------------------------------------------------------------
// Parse — agrupa tokens por linha e interpreta cada uma
// ---------------------------------------------------------------

vector<ParsedLine> Parser::Parse(const vector<Token>& tokens) {
    vector<ParsedLine> result;

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

    for (const auto& lineTokens : byLine) {
        ParsedLine pline = ParseLine(lineTokens);
        if (!pline.label.empty() || pline.lineNumber > 0 || pline.isDirective)
            result.push_back(pline);
    }

    return result;
}