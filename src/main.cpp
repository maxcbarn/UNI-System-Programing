/*
 * ETAPA 2 — Montador de Dois Passos
 *
 * Ponto de entrada do sistema. Recebe um arquivo .asm pela linha de
 * comando, passa pelo Assembler (Lexer -> Parser -> Passo1 -> Passo2)
 * para gerar os bytes, e entrega para a VM executar instrucao a instrucao.
 *
 * Como executar: ./App <arquivo.asm>
 */

#include "processor/Processor.hpp"
#include "assembler/Assembler.hpp"
#include "SystemArchitecture.hpp"
#include <iostream>

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        cerr << "Uso: ./App <arquivo.asm>" << endl;
        return 1;
    }

    // Lexer + Parser + Passo1 (tabela de simbolos) + Passo2 (resolucao)
    // Retorna os bytes prontos para carregar na VM
    Assembler assembler;
    vector<Word> program = assembler.Assemble(argv[1]);

    if (program.empty()) {
        cerr << "Falha na montagem. Verifique os erros acima." << endl;
        return 1;
    }

    // Carrega os bytes na memoria da VM e executa instrucao a instrucao
    Processor* processor = Processor::GetProcessor();
    processor->Initialize(IMPLICIT);
    processor->LoadProgram(program);

    cout << "\nPressione Enter para executar a proxima instrucao..." << endl;
    cin.get();
    while (processor->NextInstruction()) {
        cout << "Pressione Enter para executar a proxima instrucao..." << endl;
        cin.get();
    }

    return 0;
}
