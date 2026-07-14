#include "processor/Processor.hpp"
#include "assembler/Assembler.hpp"
#include "SystemArchitecture.hpp"
#include "assembler/AssemblerObj.hpp"
#include "assembler/PrintObjectModule.hpp"
#include "assembler/Linker.hpp"
#include <iostream>

using namespace std;




#include "processor/Processor.hpp"
#include "assembler/Assembler.hpp"
#include "SystemArchitecture.hpp"
#include "gui/Window.hpp"
#include "gui/Debugger.hpp"
#include <iostream>

using namespace std;

int main(int argc, char const* argv[]) {
    try {
        Window window(1280, 720, "Máquina Virtual Z80");
        std::string initialFile = (argc > 1) ? argv[1] : "";
        Debugger debugger(initialFile);

        // Initialize processor with a default addressing type just in case
        Processor::GetProcessor()->Initialize(IMPLICIT);

        while (!window.ShouldClose()) {
            window.BeginFrame();
            
            debugger.Draw();
            
            window.EndFrame();
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
