#pragma once

#include <string>

struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool ShouldClose() const;
    void BeginFrame();
    void EndFrame();
    
    GLFWwindow* GetGLFWWindow() const { return m_Window; }

private:
    GLFWwindow* m_Window;
};
