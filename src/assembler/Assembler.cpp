#include "assembler/Assembler.hpp"
#include "instructions/AdressingTypesFactoryInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include <iostream>

// ---------------------------------------------------------------
// PASSO 1 — Constroi a tabela de simbolos
//
// Percorre todas as linhas calculando o contador de localizacao (LC).
// Cada vez que encontra um rotulo, registra: nome -> endereco atual.
// ---------------------------------------------------------------

void Assembler::PassOne(const vector<ParsedLine>& lines) {
    uint16_t locationCounter = 0;

    cout << "\n=== PASSO 1 — Tabela de Simbolos ===" << endl;

    for (const auto& line : lines) {
        // Se esta linha define um rotulo, registra o endereco atual
        if (!line.label.empty()) {
            if (symbolTable.count(line.label)) {
                errors.push_back("Rotulo duplicado: " + line.label +
                                 " na linha " + to_string(line.lineNumber));
                cerr << "[ASSEMBLER ERROR] " << errors.back() << endl;
            } else {
                symbolTable[line.label] = locationCounter;
                cout << "  " << line.label << " -> 0x"
                     << hex << locationCounter << dec << endl;
            }
        }

        // Avanca o contador pelo tamanho da instrucao desta linha
        int size = Parser::GetInstructionSize(line);
        locationCounter += size;
    }
    
    cout << "====================================\n" << endl;
}

// ---------------------------------------------------------------
// PASSO 2 — Resolve simbolos e gera DecodedInstructions
//
// Percorre as linhas novamente. Onde hasSymbol=true, busca o
// endereco na tabela de simbolos construida no Passo 1.
// ---------------------------------------------------------------

vector<DecodedInstruction> Assembler::PassTwo(const vector<ParsedLine>& lines) {
    vector<DecodedInstruction> decoded;

    cout << "=== PASSO 2 — Geracao de Instrucoes ===" << endl;

    for (const auto& line : lines) {
        // Linha que so tem rotulo (sem instrucao) nao gera codigo
        if (line.lineNumber == 0) continue;

        ParsedLine resolved = line;

        // Resolve referencia a simbolo
        if (line.hasSymbol) {
            auto it = symbolTable.find(line.symbolRef);
            if (it == symbolTable.end()) {
                errors.push_back("Simbolo nao definido: " + line.symbolRef +
                                 " na linha " + to_string(line.lineNumber));
                cerr << "[ASSEMBLER ERROR] " << errors.back() << endl;
                resolved.value = 0;
            } else {
                resolved.value = it->second;
                cout << "  Resolvido: " << line.symbolRef
                     << " -> 0x" << hex << resolved.value << dec << endl;
            }
        }

        decoded.push_back(ToDecodedInstruction(resolved));
    }

    cout << "========================================\n" << endl;
    return decoded;
}

// ---------------------------------------------------------------
// Converte ParsedLine -> DecodedInstruction
// (estrutura que o EncodeInstruction da etapa 1 ja sabe usar)
// ---------------------------------------------------------------

DecodedInstruction Assembler::ToDecodedInstruction(const ParsedLine& line) {
    DecodedInstruction d{};
    d.instruction = line.instruction;
    d.addrMode    = line.addrMode; 

    switch (line.addrMode) {
        case IMPLICIT:
            switch (line.instruction) {
                case LDREGTOREG:
                    d.registers.push_back(line.reg8b_src);
                    d.registers.push_back(line.reg8b_dest);
                    break;
                case PUSH:
                case POP:
                    d.registers.push_back(line.reg16b);
                    break;
                default:
                    // ADD, SUB, AND, OR, XOR, CP, INC, DEC — operando e reg8b_src
                    d.registers.push_back(line.reg8b_src);
                    break;
            }
            break;

        case IMEDIATE:
            // LDVALTOREG: destino + valor
            d.registers.push_back(line.reg8b_dest);
            d.imediateValue.push_back((Word)line.value);
            break;

        case DIRECT:
            // JP, CALL, JPOFFSET -> endereco
            // LDMEMTOREG / LDREGTOMEM -> registrador + endereco
            if (line.instruction == LDMEMTOREG)
                d.registers.push_back(line.reg8b_dest);
            else if (line.instruction == LDREGTOMEM)
                d.registers.push_back(line.reg8b_src);
            d.adresses.push_back(line.value);
            break;

        case INDIRECT_REGISTER:
            // LD A, (HL) ou LD (HL), B
            if (line.instruction == LDMEMTOREG)
                d.registers.push_back(line.reg8b_dest);
            else if (line.instruction == LDREGTOMEM)
                d.registers.push_back(line.reg8b_src);
            d.registers.push_back(HL); 
            break;

        case INDEXED:
            // LD A, (IX+5) ou LD (IX+5), B
            if (line.instruction == LDMEMTOREG)
                d.registers.push_back(line.reg8b_dest);
            else if (line.instruction == LDREGTOMEM)
                d.registers.push_back(line.reg8b_src);
            d.registers.push_back(line.regEsp);
            d.imediateValue.push_back((Word)line.offset);
            break;

        default:
            break;
    }

    return d;
}

// ---------------------------------------------------------------
// Entrada principal: Lexer -> Parser -> Passo1 -> Passo2 -> bytes
// ---------------------------------------------------------------

vector<Word> Assembler::Assemble(const string& filepath) {
    errors.clear();
    symbolTable.clear();

    // Lexer: texto -> tokens
    Lexer lexer;    
    vector<Token> tokens = lexer.Tokenize(filepath);

    if (tokens.empty() || tokens[0].type == TokenType::END_OF_FILE) {
        errors.push_back("Arquivo vazio ou nao encontrado: " + filepath);
        return {};
    }

    // Parser: tokens -> ParsedLines
    Parser parser;
    vector<ParsedLine> lines = parser.Parse(tokens);

    // Passo 1: monta tabela de simbolos
    PassOne(lines);

    // Passo 2: resolve simbolos e gera DecodedInstructions
    vector<DecodedInstruction> decoded = PassTwo(lines);

    // Encoding: DecodedInstruction -> bytes (usa infra da etapa 1)
    vector<Word> program;
    AdressingTypesFactoryInstructions* factory =
        AdressingTypesFactoryInstructions::GetAdressingTypesFactoryInstructions();

    for (auto& instr : decoded) {
        AdressingTypesInstructions* encoder =
            factory->GetAdressingTypeInstructions(instr.addrMode);

        program.push_back( (Word) instr.addrMode );
        
        vector<Word> encoded = encoder->EncodeInstruction(&instr);
        program.insert(program.end(), encoded.begin(), encoded.end());
        delete encoder;
    }

    cout << "Montagem concluida: " << program.size() << " bytes gerados." << endl;

    if (!errors.empty()) {
        cout << "\nErros encontrados:" << endl;
        for (const auto& e : errors)
            cout << "  - " << e << endl;
    }

    return program;
}
