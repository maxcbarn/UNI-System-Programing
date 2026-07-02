#include "processor/Processor.hpp"
#include "assembler/Assembler.hpp"
#include "SystemArchitecture.hpp"
#include "assembler/AssemblerObj.hpp"
#include "assembler/PrintObjectModule.hpp"
#include "assembler/Linker.hpp"
#include <iostream>

using namespace std;




int main(int argc, char const* argv[]) {

    // linhas exemplo
    // ./build/App direct programs/directProgram.asm
    // ./build/App implicit programs/moduleA.asm programs/moduleB.asm

    if (argc < 3) {
        cerr << "Uso: ./App <adressing_type> <modulo1.asm>.....<modulon.asm>" << endl;
        return 1;
    }


    Processor* processor = Processor::GetProcessor();
    

    std::string mode = argv[1];

    if (mode == "direct") {
        processor->Initialize(DIRECT);
    }
    else if (mode == "immediate") {
        processor->Initialize(IMEDIATE);
    }
    else if (mode == "implicit") {
        processor->Initialize(IMPLICIT);
    }
    else if (mode == "indirect_register") {
        processor->Initialize(INDIRECT_REGISTER);
    }
    else if (mode == "indexed") {
        processor->Initialize(INDEXED);
    }
    else {
        std::cerr << "Unknown addressing mode: " << mode << '\n';
    }
    

    
    AssemblerObj assemblerObj;
    vector<ObjectModule> modules;

    for (size_t i = 2; i < argc; i++)
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
    uint16_t loadAddress = 0x0010; 

    LinkedModule resultAbsolute = linker.Link(modules, loadAddress,Linker::Mode::ABSOLUTE);

    if (!linker.errors.empty()) {
        cerr << "Erros na ligacao (ABSOLUTE):" << endl;
        for (const auto& e : linker.errors)
            cerr << "  " << e << endl;
    } else {
        cout << "=== Imagem ABSOLUTE (carregavel em 0x" << hex << loadAddress << dec << ") ===" << endl;
        HexDump(resultAbsolute.image, loadAddress);
    }
    
    
    LinkedModule resultRelocatable = linker.Link(modules, loadAddress,Linker::Mode::RELOCATABLE);
    
    if (!linker.errors.empty()) {
        cerr << "Erros na ligacao (RELOCATABLE):" << endl;
        for (const auto& e : linker.errors)
        cerr << "  " << e << endl;
    } else {
        cout << "=== Imagem RELOCATABLE (carregavel em 0x" << hex << loadAddress << dec << ") ===" << endl;
        HexDump(resultRelocatable.image, loadAddress);
    }
    

    // Carrega os bytes na memoria da VM e executa instrucao a instrucao

    cout << "\n=== Press 1 to use absolute | Press 2 to use relocatable ===" << endl;
    int choice;
    cin >> choice;
    if (choice == 1) {
        processor->LoadProgram(resultAbsolute, Linker::Mode::ABSOLUTE);
    } else if (choice == 2) {
        processor->LoadProgram(resultRelocatable, Linker::Mode::RELOCATABLE);
    } else {
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    cout << "\nPressione Enter para executar a proxima instrucao..." << endl;
    cin.get();
    while (processor->NextInstruction()) {
        cout << "Pressione Enter para executar a proxima instrucao..." << endl;
        cin.get();
    }

    return 0;
}
