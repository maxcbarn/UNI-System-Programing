#pragma once
// ================================================================
// Linker.hpp
//
// Ligador de duas passagens que combina multiplos ObjectModules
// relocaveis em um unico LinkedModule.
//
// Dois modos de operacao:
//
//   ABSOLUTE   (Ligador-Relocador + Carregador Absoluto)
//   ─────────────────────────────────────────────────────
//   • O endereco de carga e conhecido em tempo de ligacao.
//   • O Linker resolve TUDO: relocacao interna + referencias
//     externas.  O LinkedModule resultante tem relocTable vazia.
//   • O Carregador Absoluto apenas copia image[] para memoria
//     no endereco loadAddress.
//
//   RELOCATABLE (Ligador + Carregador Relocador)
//   ─────────────────────────────────────────────────────
//   • O endereco de carga NAO e conhecido em tempo de ligacao.
//   • O Linker resolve referencias externas e aplica relocacao
//     parcial (combina os modulos, ajusta enderecos relativos
//     entre modulos), mas deixa a relocacao final para o loader.
//   • O LinkedModule tem uma relocTable com os offsets que o
//     Carregador Relocador deve ajustar somando loadAddress.
//
// Uso:
//   Linker lnk;
//   // Modo absoluto: carga em 0x1000
//   LinkedModule out = lnk.Link(modules, 0x1000, Linker::Mode::ABSOLUTE);
//   // Modo relocavel: loader decide o endereco
//   LinkedModule out = lnk.Link(modules, 0,      Linker::Mode::RELOCATABLE);
// ================================================================

#include "assembler/AssemblerObj.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;

class Linker {
public:
    enum class Mode {
        ABSOLUTE,     // Ligador-Relocador: loader absoluto
        RELOCATABLE   // Ligador apenas: loader relocador
    };
    LinkedModule Link(const vector<ObjectModule>& modules,uint16_t loadAddr,Mode mode);
    vector<string> errors;
private:
    map<string, uint16_t> gst;
    vector<uint16_t> bases;

    // Passo 1: atribui base a cada modulo, preenche GST
    void PassOne(const vector<ObjectModule>& modules, uint16_t startAddr);

    // Passo 2: gera a imagem, aplica relocacao, resolve externos
    void PassTwo(const vector<ObjectModule>& modules,uint16_t startAddr,Mode mode,LinkedModule& out);
    static uint16_t ReadU16(const vector<uint8_t>& text, uint16_t offset);
    static void WriteU16(vector<uint8_t>& image, uint16_t offset, uint16_t value);
    void PrintGST() const;
};
