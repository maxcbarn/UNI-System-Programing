#pragma once
#include "assembler/AssemblerObj.hpp"
#include <iostream>
#include <iomanip>
#include <string>

static void PrintSection(const std::string& title, char fill = '-', int width = 52) {
    int pad = (width - (int)title.size() - 2);
    int left  = pad / 2;
    int right = pad - left;
    std::cout << std::string(left, fill)
              << " " << title << " "
              << std::string(right, fill) << "\n";
}

static void HexDump(const std::vector<uint8_t>& data, uint16_t baseAddr = 0) {
    if (data.empty()) {
        std::cout << "    (vazio)\n";
        return;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 16 == 0) {
            std::cout << "    "
                      << std::hex << std::setw(4) << std::setfill('0')
                      << (baseAddr + i) << ": ";
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << (int)data[i] << " ";
        if (i % 16 == 15 || i == data.size() - 1)
            std::cout << "\n";
    }
    std::cout << std::dec;
}

inline void PrintObjectModule(const ObjectModule& mod) {
    const int W = 52;
    std::string bar(W, '=');

    std::cout << "\n" << bar << "\n";
    std::cout << "  OBJECT MODULE: " << mod.name << "\n";
    std::cout << bar << "\n";

    // ── Cabecalho ─────────────────────────────────────────────
    PrintSection("HEADER");
    std::cout << "  Nome    : " << mod.name   << "\n";
    std::cout << "  Tamanho : " << mod.length << " bytes\n";
    if (mod.hasEntryPoint)
        std::cout << "  Entry   : 0x"
                  << std::hex << std::setw(4) << std::setfill('0')
                  << mod.entryPoint << std::dec << "\n";
    else
        std::cout << "  Entry   : (nenhum)\n";

    PrintSection("ENTRY TABLE");
    if (mod.entryTable.empty()) {
        std::cout << "  (nenhum simbolo exportado)\n";
    } else {
        std::cout << "  " << std::left << std::setw(20) << "Simbolo"
                  << "Offset local\n";
        std::cout << "  " << std::string(30, '-') << "\n";
        for (const EntryRecord& er : mod.entryTable) {
            std::cout << "  " << std::left  << std::setw(20) << er.name
                      << "0x" << std::hex << std::setw(4) << std::setfill('0')
                      << er.localOffset << std::dec << "\n";
        }
    }

    PrintSection("EXTERN TABLE");
    if (mod.externTable.empty()) {
        std::cout << "  (nenhum simbolo importado)\n";
    } else {
        for (const ExternRecord& ext : mod.externTable) {
            std::cout << "  " << ext.name << "\n";
            if (ext.useOffsets.empty()) {
                std::cout << "    (sem usos registrados)\n";
            } else {
                std::cout << "    Usos (offsets): ";
                for (size_t i = 0; i < ext.useOffsets.size(); ++i) {
                    if (i) std::cout << ", ";
                    std::cout << "0x" << std::hex << std::setw(4)
                              << std::setfill('0') << ext.useOffsets[i] << std::dec;
                }
                std::cout << "\n";
            }
        }
    }

    PrintSection("RELOC TABLE");
    if (mod.relocTable.empty()) {
        std::cout << "  (nenhuma entrada)\n";
    } else {
        std::cout << "  Offsets que precisam de +base no link:\n  ";
        for (size_t i = 0; i < mod.relocTable.size(); ++i) {
            if (i && i % 8 == 0) std::cout << "\n  ";
            std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0')
                      << mod.relocTable[i] << std::dec << "  ";
        }
        std::cout << "\n";
    }

    PrintSection("TEXT SEGMENT");
    std::cout << "  " << mod.text.size() << " bytes (base = 0x0000)\n";
    HexDump(mod.text, 0x0000);

    PrintSection("ERRORS");
    if (mod.errors.empty()) {
        std::cout << "  (nenhum)\n";
    } else {
        for (const std::string& e : mod.errors)
            std::cout << "  [!] " << e << "\n";
    }

    std::cout << bar << "\n\n";
}

inline void PrintLinkedModule(const LinkedModule& lm) {
    const int W = 52;
    std::string bar(W, '=');

    std::cout << "\n" << bar << "\n";
    std::cout << "  LINKED MODULE\n";
    std::cout << bar << "\n";

    PrintSection("HEADER");
    std::cout << "  Tamanho       : " << lm.image.size() << " bytes\n";
    std::cout << "  Load address  : 0x"
              << std::hex << std::setw(4) << std::setfill('0')
              << lm.loadAddress << std::dec << "\n";
    if (lm.hasEntry)
        std::cout << "  Entry point   : 0x"
                  << std::hex << std::setw(4) << std::setfill('0')
                  << lm.entryPoint << std::dec << "\n";
    else
        std::cout << "  Entry point   : (nenhum)\n";

    PrintSection("RELOC TABLE");
    if (lm.relocTable.empty()) {
        std::cout << "  (vazia — imagem completamente resolvida)\n";
    } else {
        std::cout << "  " << lm.relocTable.size()
                  << " offsets pendentes (loader soma loadAddress a cada um):\n  ";
        for (size_t i = 0; i < lm.relocTable.size(); ++i) {
            if (i && i % 8 == 0) std::cout << "\n  ";
            std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0')
                      << lm.relocTable[i] << std::dec << "  ";
        }
        std::cout << "\n";
    }

    PrintSection("IMAGE");
    HexDump(lm.image, lm.loadAddress);

    PrintSection("ERRORS");
    if (lm.errors.empty()) {
        std::cout << "  (nenhum)\n";
    } else {
        for (const std::string& e : lm.errors)
            std::cout << "  [!] " << e << "\n";
    }

    std::cout << bar << "\n\n";
}
