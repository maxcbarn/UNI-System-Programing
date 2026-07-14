#pragma once

#include <vector>
#include <string>

class Debugger {
public:
    Debugger(const std::string& initialFilePath = "");
    ~Debugger();

    void Draw();

private:
    void DrawControlPanel();
    void DrawRegisters();
    void DrawMemory();
    void DrawCode();

    bool m_AutoRun = false;
    float m_AutoRunTimer = 0.0f;
    float m_AutoRunSpeed = 0.1f;    // Seconds per instruction

    int m_MemoryScrollToAddr = -1;
    char m_FilePathBuffer[256];
};
