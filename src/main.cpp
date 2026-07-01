#include "processor/Processor.hpp"
#include "assembler/Assembler.hpp"
#include "SystemArchitecture.hpp"
#include "assembler/AssemblerObj.hpp"
#include "assembler/PrintObjectModule.hpp"
#include "assembler/Linker.hpp"
#include <iostream>


int main(int argc, char const* argv[]) {

    if (argc < 2) {
        cerr << "Uso: ./App <arquivo.asm>" << endl;
        return 1;
    }

    
    AssemblerObj assemblerObj;
    vector<ObjectModule> modules;

    for (size_t i = 1; i < argc; i++)
    {
        modules.push_back(assemblerObj.Assemble(argv[i]));  
        if (!modules.back().errors.empty()) {
            cerr << "Erros no modulo " << i << ":" << endl;
            for (const auto& e : modules.back().errors)
                cerr << "  " << e << endl;
            return 1;
        }
        PrintObjectModule(modules.back());  
    }
    

    Linker linker;
    uint16_t loadAddress = 0x0000; 

    LinkedModule result = linker.Link(modules, loadAddress,Linker::Mode::ABSOLUTE);

    if (!linker.errors.empty()) {
        cerr << "Erros na ligacao (ABSOLUTE):" << endl;
        for (const auto& e : linker.errors)
            cerr << "  " << e << endl;
    } else {
        cout << "=== Imagem ABSOLUTE (carregavel em 0x"
                << hex << loadAddress << dec << ") ===" << endl;
        HexDump(result.image, loadAddress);
    }
    
    vector<Word> program = result.image;

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
