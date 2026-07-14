#include "gui/Debugger.hpp"
#include "imgui.h"
#include "processor/memory/Registers.hpp"
#include "processor/memory/Memory.hpp"
#include "processor/Processor.hpp"
#include "assembler/AssemblerObj.hpp"
#include "assembler/Linker.hpp"
#include <iostream>
#include <string>
#include <cstring>
Debugger::Debugger(const std::string& initialFilePath) {
    memset(m_FilePathBuffer, 0, sizeof(m_FilePathBuffer));
    if (!initialFilePath.empty()) {
        strncpy(m_FilePathBuffer, initialFilePath.c_str(), sizeof(m_FilePathBuffer) - 1);
    }
}

Debugger::~Debugger() {}

void Debugger::Draw() {
    if (m_AutoRun) {
        m_AutoRunTimer -= ImGui::GetIO().DeltaTime;
        if (m_AutoRunTimer <= 0.0f) {
            m_AutoRunTimer = m_AutoRunSpeed;
            if (!Processor::GetProcessor()->NextInstruction()) {
                m_AutoRun = false;
            }
        }
    }

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
    
    ImGui::Begin("Z80 Simulator Main", nullptr, window_flags);
    ImGui::PopStyleVar(3);
    
    if (ImGui::BeginTable("MainTable", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthFixed, 350.0f);
        ImGui::TableSetupColumn("Center", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Right", ImGuiTableColumnFlags_WidthFixed, 520.0f);
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        DrawControlPanel();
        DrawRegisters();
        
        ImGui::TableSetColumnIndex(1);
        DrawCode();
        
        ImGui::TableSetColumnIndex(2);
        DrawMemory();
        
        ImGui::EndTable();
    }
    
    ImGui::End();
}

void Debugger::DrawControlPanel() {
    ImGui::BeginChild("Controls", ImVec2(0, 320), true);

    ImGui::InputText("Program Path", m_FilePathBuffer, sizeof(m_FilePathBuffer));
    
    // We assume implicit addressing for UI for simplicity, or we could let the user choose
    static int current_addr_mode = 0;
    const char* addr_modes[] = { "Implicit", "Immediate", "Direct", "Indexed", "Indirect (Register)" };
    ImGui::Combo("Addressing Mode", &current_addr_mode, addr_modes, IM_ARRAYSIZE(addr_modes));
    
    static int link_mode = 0;
    ImGui::RadioButton("Absolute", &link_mode, 0); ImGui::SameLine();
    ImGui::RadioButton("Relocatable", &link_mode, 1);

    if (ImGui::Button("Load Program")) {
        Processor* proc = Processor::GetProcessor();
        switch (current_addr_mode) {
            case 0: proc->Initialize(IMPLICIT); break;
            case 1: proc->Initialize(IMEDIATE); break;
            case 2: proc->Initialize(DIRECT); break;
            case 3: proc->Initialize(INDEXED); break;
            case 4: proc->Initialize(INDIRECT_REGISTER); break;
        }

        AssemblerObj assembler;
        ObjectModule mod = assembler.Assemble(m_FilePathBuffer);
        
        if (mod.errors.empty()) {
            std::vector<ObjectModule> modules = { mod };
            Linker linker;
            uint16_t loadAddress = 0x0010;
            
            Linker::Mode lMode = link_mode == 0 ? Linker::Mode::ABSOLUTE : Linker::Mode::RELOCATABLE;
            LinkedModule linked = linker.Link(modules, loadAddress, lMode);
            
            if (linker.errors.empty()) {
                proc->LoadProgram(linked, lMode);
                std::cout << "Program loaded successfully!\n";
            } else {
                for (const auto& e : linker.errors) std::cerr << "Linker Error: " << e << "\n";
            }
        } else {
            for (const auto& e : mod.errors) std::cerr << "Assembler Error: " << e << "\n";
        }
    }

    ImGui::Separator();

    if (ImGui::Button("Step")) {
        Processor::GetProcessor()->NextInstruction();
    }
    
    ImGui::SameLine();
    
    if (m_AutoRun) {
        if (ImGui::Button("Pause")) m_AutoRun = false;
    } else {
        if (ImGui::Button("Auto Run")) m_AutoRun = true;
    }
    
    ImGui::SliderFloat("Speed (s/inst)", &m_AutoRunSpeed, 0.01f, 2.0f);

    if (ImGui::Button("Reset VM")) {
        Registers::ResetRegisters();
        Memory::ResetMemory();
        m_AutoRun = false;
    }

    ImGui::EndChild();
}

void Debugger::DrawRegisters() {
    ImGui::BeginChild("Registers", ImVec2(0, 0), true);
    Registers* regs = Registers::GetRegisters();
    
    if (ImGui::BeginTable("Regs8bits", 2, ImGuiTableFlags_Borders)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("A: 0x%02X", regs->ReadFrom8bRegister(A));
        
        uint8_t flags = regs->ReadFrom16bRegister(AF) & 0xFF;
        std::string flagStr = "F: ";
        flagStr += (flags & FLAGS::SIGN) ? "S " : "- ";
        flagStr += (flags & FLAGS::ZERO) ? "Z " : "- ";
        flagStr += (flags & FLAGS::HALF_CARRY) ? "H " : "- ";
        flagStr += (flags & FLAGS::PARITY_OVERFLOW) ? "P " : "- ";
        flagStr += (flags & FLAGS::ADD_SUBTRACT) ? "N " : "- ";
        flagStr += (flags & FLAGS::CARRY) ? "C" : "-";
        
        ImGui::TableSetColumnIndex(1); ImGui::Text("%s", flagStr.c_str());
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("B: 0x%02X", regs->ReadFrom8bRegister(B));
        ImGui::TableSetColumnIndex(1); ImGui::Text("C: 0x%02X", regs->ReadFrom8bRegister(C));
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("D: 0x%02X", regs->ReadFrom8bRegister(D));
        ImGui::TableSetColumnIndex(1); ImGui::Text("E: 0x%02X", regs->ReadFrom8bRegister(E));
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("H: 0x%02X", regs->ReadFrom8bRegister(H));
        ImGui::TableSetColumnIndex(1); ImGui::Text("L: 0x%02X", regs->ReadFrom8bRegister(L));
        
        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::BeginTable("Regs16bits", 2, ImGuiTableFlags_Borders)) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("PC: 0x%04X", regs->GetProgramCounter());
        ImGui::TableSetColumnIndex(1); ImGui::Text("SP: 0x%04X", regs->GetStackPtr());
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0); ImGui::Text("IX: 0x%04X", regs->ReadFromEspRegister(IX));
        ImGui::TableSetColumnIndex(1); ImGui::Text("IY: 0x%04X", regs->ReadFromEspRegister(IY));
        
        ImGui::EndTable();
    }
    
    ImGui::EndChild();
}

void Debugger::DrawMemory() {
    ImGui::BeginChild("Memory", ImVec2(0, 0), true);
    
    static bool apply_offset = true;
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Go to:");
    ImGui::SameLine();
    
    static char addr_input[16] = "";
    ImGui::PushItemWidth(50);
    if (ImGui::InputText("##addr", addr_input, sizeof(addr_input), ImGuiInputTextFlags_EnterReturnsTrue)) {
        try {
            m_MemoryScrollToAddr = std::stoi(addr_input, nullptr, 16);
            if (apply_offset) {
                m_MemoryScrollToAddr += Registers::GetRegisters()->GetStartMemory();
            }
        } catch (...) { m_MemoryScrollToAddr = -1; }
    }
    ImGui::PopItemWidth();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Edit RAM - Address:");
    ImGui::SameLine();
    
    static char write_addr[16] = "";
    static char write_val[16] = "";
    ImGui::PushItemWidth(45);
    ImGui::InputText("##wa", write_addr, sizeof(write_addr));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("Value:");
    ImGui::SameLine();
    ImGui::PushItemWidth(35);
    ImGui::InputText("##wv", write_val, sizeof(write_val));
    ImGui::PopItemWidth();
    ImGui::SameLine();
    
    ImGui::Checkbox("Use Data Segment Offset", &apply_offset);
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("If checked, adds the StartMemory offset (end of program code) to your address, matching the processor's internal logic.");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Set")) {
        try {
            uint16_t wa = std::stoi(write_addr, nullptr, 16);
            uint8_t wv = std::stoi(write_val, nullptr, 16);
            
            if (apply_offset) {
                wa += Registers::GetRegisters()->GetStartMemory();
            }
            
            Memory::GetMemory()->ModifyMemory(wa, wv);
        } catch (...) {}
    }

    ImGui::Separator();
    
    ImGui::BeginChild("MemoryScrollRegion", ImVec2(0, 0), true);
    
    Memory* mem = Memory::GetMemory();
    int total_rows = 65536 / 16;
    
    ImGuiListClipper clipper;
    clipper.Begin(total_rows);
    
    if (m_MemoryScrollToAddr != -1) {
        ImGui::SetScrollY((m_MemoryScrollToAddr / 16) * ImGui::GetTextLineHeightWithSpacing());
        m_MemoryScrollToAddr = -1;
    }
    
    while (clipper.Step()) {
        for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
            uint16_t base_addr = row * 16;
            ImGui::Text("%04X: ", base_addr);
            for (int i = 0; i < 16; i++) {
                ImGui::SameLine();
                Word val = mem->ReadMemory(base_addr + i);
                ImGui::Text("%02X", val);
            }
        }
    }
    ImGui::EndChild();
    
    ImGui::EndChild();
}

void Debugger::DrawCode() {
    ImGui::BeginChild("Information and Status", ImVec2(0, 0), true);
    ImGui::Text("Z80 Virtual Machine Simulator running.");
    ImGui::TextWrapped("Compile and load an assembly program using the 'Controls' panel. Keep an eye on the original command terminal to check for logs and syntax errors (from the Assembler and Macro Processor).");
    ImGui::EndChild();
}
