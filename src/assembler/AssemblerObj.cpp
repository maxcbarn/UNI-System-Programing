#include "assembler/AssemblerObj.hpp"
#include "instructions/AdressingTypesFactoryInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include <iostream>
#include <algorithm>

// ---------------------------------------------------------------
// Utilitario: verifica se sym foi declarado EXTRN neste modulo
// ---------------------------------------------------------------

bool AssemblerObj::IsExtern(const string& sym) const {
    return find(externSymbols.begin(), externSymbols.end(), sym) != externSymbols.end();
}

// ---------------------------------------------------------------
// PASSO 1
//
// Objetivos:
//   1. Percorrer todas as linhas calculando o LC (location counter).
//   2. Registrar cada label na symbolTable local.
//   3. Registrar ENTRY  → lista entrySymbols (simbolos exportados).
//   4. Registrar EXTRN  → lista externSymbols (simbolos importados).
//   5. Preencher mod.entryTable e mod.externTable com os dados
//      que o Linker vai precisar.  Os useOffsets de extern so
//      ficam disponiveis no Passo 2 — aqui so registramos os nomes.
// ---------------------------------------------------------------

void AssemblerObj::PassOne(const vector<ParsedLine>& lines, ObjectModule& mod) {
    uint16_t lc = 0;

    cout << "\n=== PASSO 1 — Tabela de Simbolos ===" << endl;

    for (const auto& line : lines) {

        // --- Diretivas de ligacao ---
        if (line.isDirective) {
            if (line.directiveType == DirectiveType::ENTRY) {
                for (const string& sym : line.directiveSymbols) {
                    // Verificamos se ja foi registrado para evitar duplicatas
                    bool already = false;
                    for (auto& ep : entrySymbols)
                        if (ep.first == sym) { already = true; break; }
                    if (!already) {
                        // O offset real so e conhecido quando encontrarmos
                        // o label correspondente; guardamos para resolver
                        // no final do Passo 1.
                        entrySymbols.push_back({ sym, sym });
                        cout << "  [ENTRY] " << sym << " (offset a resolver)" << endl;
                    }
                }
            } else if (line.directiveType == DirectiveType::EXTRN) {
                for (const string& sym : line.directiveSymbols) {
                    bool already = find(externSymbols.begin(),
                                        externSymbols.end(), sym) != externSymbols.end();
                    if (!already) {
                        externSymbols.push_back(sym);
                        cout << "  [EXTRN] " << sym << endl;
                    }
                }
            }
            // Diretivas nao geram codigo → LC nao avanca
            continue;
        }

        // --- Label ---
        if (!line.label.empty()) {
            if (symbolTable.count(line.label)) {
                errors.push_back("Rotulo duplicado: " + line.label +
                                 " na linha " + to_string(line.lineNumber));
                cerr << "[ASSEMBLER ERROR] " << errors.back() << endl;
            } else {
                symbolTable[line.label] = lc;
                cout << "  " << line.label << " -> 0x"
                     << hex << lc << dec << endl;
            }
        }

        lc += Parser::GetInstructionSize(line);
    }

    mod.length = lc;
    cout << "  Tamanho do modulo: " << lc << " bytes" << endl;

    // Resolve offsets dos simbolos ENTRY usando a symbolTable
    for (auto& ep : entrySymbols) {
        auto it = symbolTable.find(ep.second); // ep.second == labelName
        if (it == symbolTable.end()) {
            errors.push_back("ENTRY: simbolo '" + ep.first +
                             "' nao definido como label neste modulo");
            cerr << "[ASSEMBLER ERROR] " << errors.back() << endl;
            continue;
        }
        EntryRecord rec;
        rec.name        = ep.first;
        rec.localOffset = it->second;
        mod.entryTable.push_back(rec);
        cout << "  [ENTRY resolvido] " << rec.name
             << " -> offset 0x" << hex << rec.localOffset << dec << endl;
    }

    // Prepara entradas na externTable (useOffsets preenchidos no Passo 2)
    for (const string& sym : externSymbols) {
        ExternRecord rec;
        rec.name = sym;
        mod.externTable.push_back(rec);
    }

    cout << "====================================\n" << endl;
}

// ---------------------------------------------------------------
// PASSO 2
//
// Objetivos:
//   1. Gerar os bytes de cada instrucao (LC avanca de novo).
//   2. Para referencias a labels LOCAIS (hasSymbol + nao-extern):
//      • resolver pelo symbolTable.
//      • o endereco e interno → adicionar o offset a mod.relocTable.
//   3. Para referencias a simbolos EXTERNOS (hasSymbol + extern):
//      • colocar 0x0000 provisorio nos bytes.
//      • registrar o offset no ExternRecord correspondente.
//   4. Enderecos de dados diretos internos (ex: LD A,(1234)):
//      • sao internos → registrar em relocTable.
// ---------------------------------------------------------------

void AssemblerObj::PassTwo(const vector<ParsedLine>& lines, ObjectModule& mod) {
    uint16_t lc = 0;

    cout << "=== PASSO 2 — Geracao de Codigo ===" << endl;

    AdressingTypesFactoryInstructions* factory =
        AdressingTypesFactoryInstructions::GetAdressingTypesFactoryInstructions();

    for (const auto& line : lines) {

        // Diretivas nao geram bytes
        if (line.isDirective) continue;

        ParsedLine resolved = line;

        // --- Resolve referencias a simbolos ---
        if (line.hasSymbol) {
            if (IsExtern(line.symbolRef)) {
                // Referencia externa: valor provisorio 0, registra o uso
                resolved.value = 0x0000;

                // O campo de 16 bits do endereco fica em:
                //   lc + 1  (o primeiro byte e o opcode/addrMode)
                // Isso depende do layout exato do encoder.
                // Usamos lc+1 como convencao; ajuste se necessario.
                uint16_t useOffset = lc + 1;

                for (auto& ext : mod.externTable) {
                    if (ext.name == line.symbolRef) {
                        ext.useOffsets.push_back(useOffset);
                        break;
                    }
                }
                cout << "  [EXTRN uso] " << line.symbolRef
                     << " @ offset 0x" << hex << useOffset << dec << endl;

            } else {
                // Referencia local
                auto it = symbolTable.find(line.symbolRef);
                if (it == symbolTable.end()) {
                    errors.push_back("Simbolo nao definido: " + line.symbolRef +
                                     " na linha " + to_string(line.lineNumber));
                    cerr << "[ASSEMBLER ERROR] " << errors.back() << endl;
                    resolved.value = 0;
                } else {
                    resolved.value = it->second;
                    cout << "  [local] " << line.symbolRef
                         << " -> 0x" << hex << resolved.value << dec << endl;

                    // Endereco interno → precisa de relocacao
                    // O campo fica em lc+1 (apos o byte de modo)
                    mod.relocTable.push_back(lc + 1);
                }
            }
        }

        // --- Converte para DecodedInstruction e codifica ---
        DecodedInstruction decoded = ToDecodedInstruction(resolved);

        AdressingTypesInstructions* encoder =
            factory->GetAdressingTypeInstructions(decoded.addrMode);

        // Byte de modo de enderecamento (convencao do projeto)
        mod.text.push_back((uint8_t)decoded.addrMode);

        vector<Word> encoded = encoder->EncodeInstruction(&decoded);
        for (Word w : encoded)
            mod.text.push_back(w);

        delete encoder;

        lc += Parser::GetInstructionSize(line);
    }

    cout << "  " << mod.text.size() << " bytes gerados." << endl;
    cout << "  " << mod.relocTable.size() << " entradas na tabela de relocacao." << endl;
    cout << "======================================\n" << endl;
}

// ---------------------------------------------------------------
// ToDecodedInstruction (sem alteracao logica)
// ---------------------------------------------------------------

DecodedInstruction AssemblerObj::ToDecodedInstruction(const ParsedLine& line) {
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
                case PUSH: case POP:
                    d.registers.push_back(line.reg16b);
                    break;
                default:
                    d.registers.push_back(line.reg8b_src);
                    break;
            }
            break;
        case IMEDIATE:
            d.registers.push_back(line.reg8b_dest);
            d.imediateValue.push_back((Word)line.value);
            break;
        case DIRECT:
            if (line.instruction == LDMEMTOREG)
                d.registers.push_back(line.reg8b_dest);
            else if (line.instruction == LDREGTOMEM)
                d.registers.push_back(line.reg8b_src);
            d.adresses.push_back(line.value);
            break;
        case INDIRECT_REGISTER:
            if (line.instruction == LDMEMTOREG)
                d.registers.push_back(line.reg8b_dest);
            else if (line.instruction == LDREGTOMEM)
                d.registers.push_back(line.reg8b_src);
            d.registers.push_back(HL);
            break;
        case INDEXED:
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
// Assemble — ponto de entrada principal
// ---------------------------------------------------------------

ObjectModule AssemblerObj::Assemble(const string& filepath) {
    ObjectModule mod;
    errors.clear();
    symbolTable.clear();
    externSymbols.clear();
    entrySymbols.clear();


    size_t slash = filepath.find_last_of("/\\");
    string fname = (slash == string::npos) ? filepath : filepath.substr(slash + 1);
    size_t dot = fname.rfind('.');
    mod.name = (dot == string::npos) ? fname : fname.substr(0, dot);

    MacroProcessor macroProcessor;
    string expandedFile = macroProcessor.Process(filepath);
    if (expandedFile.empty()) {
        for (const auto& e : macroProcessor.errors)
            errors.push_back(e);
        cerr << "Falha no pre-processamento de macros." << endl;
        mod.errors = errors;
        return mod;
    }
    for (const auto& e : macroProcessor.errors)
        errors.push_back(e);

    Lexer lexer;
    vector<Token> tokens = lexer.Tokenize(expandedFile);
    if (tokens.empty() || tokens[0].type == TokenType::END_OF_FILE) {
        errors.push_back("Arquivo vazio ou nao encontrado: " + filepath);
        mod.errors = errors;
        return mod;
    }
    Parser parser;
    vector<ParsedLine> lines = parser.Parse(tokens);
    PassOne(lines, mod);
    PassTwo(lines, mod);

    mod.errors = errors;

    if (!errors.empty()) {
        cout << "\nErros encontrados:" << endl;
        for (const auto& e : errors)
            cout << "  - " << e << endl;
    }

    return mod;
}
