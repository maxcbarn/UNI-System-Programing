#include "assembler/Linker.hpp"
#include <iostream>
#include <iomanip>



uint16_t Linker::ReadU16(const vector<uint8_t>& text, uint16_t offset) {
    if (offset + 1 >= (uint16_t)text.size()) return 0;
    return (uint16_t)(text[offset]) | ((uint16_t)(text[offset + 1]) << 8);
}

void Linker::WriteU16(vector<uint8_t>& image, uint16_t offset, uint16_t value) {
    if (offset + 1 >= (uint16_t)image.size()) return;
    image[offset]     = (uint8_t)(value & 0xFF);
    image[offset + 1] = (uint8_t)((value >> 8) & 0xFF);
}

void Linker::PrintGST() const {
    cout << "  Tabela Global de Simbolos (GST):" << endl;
    for (const auto& entry : gst) {
        cout << "    " << entry.first
             << " -> 0x" << hex << setw(4) << setfill('0') << entry.second
             << dec << endl;
    }
}


void Linker::PassOne(const vector<ObjectModule>& modules,
                     uint16_t startAddr) {
    gst.clear();
    bases.clear();

    uint16_t lc = startAddr;

    cout << "\n=== LINKER PASSO 1 — Atribuicao de Bases ===" << endl;

    for (size_t i = 0; i < modules.size(); ++i) {
        const ObjectModule& mod = modules[i];
        bases.push_back(lc);

        cout << "  Modulo [" << mod.name << "]"
             << "  base=0x" << hex << setw(4) << setfill('0') << lc
             << "  length=" << dec << mod.length << endl;

        // Registra os simbolos exportados na GST
        for (const EntryRecord& er : mod.entryTable) {
            uint16_t globalAddr = lc + er.localOffset;

            if (gst.count(er.name)) {
                errors.push_back("Definicao duplicada do simbolo '" +
                                 er.name + "' (modulos multiplos)");
                cerr << "[LINKER ERROR] " << errors.back() << endl;
                continue;
            }

            gst[er.name] = globalAddr;
            cout << "    ENTRY " << er.name
                 << " (local 0x" << hex << er.localOffset << ")"
                 << " -> global 0x" << globalAddr << dec << endl;
        }

        lc += mod.length;
    }

    cout << "  Tamanho total combinado: " << (lc - startAddr) << " bytes" << endl;
    PrintGST();
    cout << "=============================================\n" << endl;
}


void Linker::PassTwo(const vector<ObjectModule>& modules,uint16_t startAddr,Mode mode,LinkedModule& out) {

    // Calcula tamanho total e aloca a imagem
    uint16_t totalSize = 0;
    for (const auto& mod : modules)
        totalSize += mod.length;

    out.image.assign(totalSize, 0x00);
    out.relocTable.clear();

    cout << "=== LINKER PASSO 2 — Relocacao e Resolucao ===" << endl;

    for (size_t i = 0; i < modules.size(); ++i) {
        const ObjectModule& mod = modules[i];
        uint16_t base    = bases[i];
        uint16_t imgBase = base - startAddr; // offset dentro de out.image

        cout << "  Modulo [" << mod.name << "]"
             << "  base=0x" << hex << setw(4) << setfill('0') << base
             << dec << endl;

        // ── a) Copia o segmento de codigo ──────────────────────
        for (size_t b = 0; b < mod.text.size(); ++b)
            out.image[imgBase + b] = mod.text[b];

        // ── b) Relocacao interna ────────────────────────────────
        for (uint16_t relocOff : mod.relocTable) {
            uint16_t imageOff = imgBase + relocOff;

            // Le o valor atual (modulo-relativo)
            uint16_t oldVal = ReadU16(out.image, imageOff);

            // Soma a base do modulo para tornar combinado-relativo
            // (em ABSOLUTE isso ja e o endereco final)
            uint16_t newVal = oldVal + base;
            WriteU16(out.image, imageOff, newVal);

            cout << "    [reloc interno] offset 0x" << hex << relocOff
                 << "  " << oldVal << " -> " << newVal << dec << endl;

            // Em modo RELOCATABLE: o loader ainda precisa somar loadAddr
            if (mode == Mode::RELOCATABLE)
                out.relocTable.push_back(imageOff);
        }

        // ── c) Resolucao de externos ────────────────────────────
        for (const ExternRecord& ext : mod.externTable) {
            auto it = gst.find(ext.name);
            if (it == gst.end()) {
                errors.push_back("Simbolo externo nao definido: '" +
                                 ext.name + "' (referenciado por " +
                                 mod.name + ")");
                cerr << "[LINKER ERROR] " << errors.back() << endl;
                continue;
            }

            uint16_t resolvedAddr = it->second;

            for (uint16_t useOff : ext.useOffsets) {
                uint16_t imageOff = imgBase + useOff;
                WriteU16(out.image, imageOff, resolvedAddr);

                cout << "    [extern] " << ext.name
                     << " @ modulo-offset 0x" << hex << useOff
                     << " -> 0x" << resolvedAddr << dec << endl;

                // Em modo RELOCATABLE: resolvedAddr e combinado-relativo,
                // portanto o loader ainda precisa somar loadAddr
                if (mode == Mode::RELOCATABLE)
                    out.relocTable.push_back(imageOff);
            }
        }

        // Ponto de entrada (primeiro modulo com entryPoint vence)
        if (mod.hasEntryPoint && !out.hasEntry) {
            out.hasEntry   = true;
            out.entryPoint = base + mod.entryPoint;
            cout << "    [entry point] 0x" << hex << out.entryPoint << dec << endl;
        }
    }

    out.loadAddress = startAddr;

    cout << "  Imagem gerada: " << out.image.size() << " bytes" << endl;
    if (mode == Mode::RELOCATABLE)
        cout << "  Entradas na tabela de relocacao de saida: "
             << out.relocTable.size() << endl;
    else
        cout << "  Modo ABSOLUTE: sem tabela de relocacao de saida." << endl;

    cout << "================================================\n" << endl;
}

// ---------------------------------------------------------------
// Link — ponto de entrada publico
// ---------------------------------------------------------------

LinkedModule Linker::Link(const vector<ObjectModule>& modules,
                          uint16_t loadAddr,
                          Mode mode) {
    errors.clear();
    LinkedModule out;

    if (modules.empty()) {
        errors.push_back("Nenhum modulo fornecido ao ligador.");
        out.errors = errors;
        return out;
    }

    cout << "\n╔══════════════════════════════════════════════╗" << endl;
    cout << "║  LIGADOR DE DUAS PASSAGENS                   ║" << endl;
    cout << "║  Modo: "
         << (mode == Mode::ABSOLUTE ? "ABSOLUTE (Carregador Absoluto) " 
                                    : "RELOCATABLE (Carregador Relocador)")
         << "  ║" << endl;
    if (mode == Mode::ABSOLUTE)
         cout << "║  Endereco de carga: 0x"
              << hex << setw(4) << setfill('0') << loadAddr
              << dec << "                    ║" << endl;
    cout << "╚══════════════════════════════════════════════╝" << endl;

    // Em modo RELOCATABLE o LC comeca em 0 (loader decide a base real)
    uint16_t startAddr = (mode == Mode::ABSOLUTE) ? loadAddr : 0;

    // ── Passo 1 ──────────────────────────────────────────────
    PassOne(modules, startAddr);

    if (!errors.empty()) {
        out.errors = errors;
        return out;  // Erros no Passo 1 impedem continuacao
    }

    // ── Passo 2 ──────────────────────────────────────────────
    PassTwo(modules, startAddr, mode, out);

    out.errors = errors;

    // ── Resumo final ─────────────────────────────────────────
    cout << "=== RESULTADO DA LIGACAO ===" << endl;
    cout << "  Tamanho da imagem    : " << out.image.size() << " bytes" << endl;
    cout << "  Endereco de carga    : 0x"
         << hex << setw(4) << setfill('0') << out.loadAddress << dec << endl;
    if (out.hasEntry)
        cout << "  Ponto de entrada     : 0x"
             << hex << setw(4) << setfill('0') << out.entryPoint << dec << endl;
    if (!out.relocTable.empty()) {
        cout << "  Tabela de relocacao  : " << out.relocTable.size()
             << " entradas" << endl;
        cout << "  (O Carregador Relocador deve somar loadAddress a cada uma)" << endl;
    }
    if (!errors.empty()) {
        cout << "  Erros:" << endl;
        for (const auto& e : errors)
            cout << "    - " << e << endl;
    } else {
        cout << "  Ligacao concluida SEM erros." << endl;
    }
    cout << "============================\n" << endl;

    return out;
}
