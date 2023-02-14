#pragma once

class Application
{
private:
    bool m_running;

    void Init();
    void OnEvent();
    void OnRender();
public:
    Application() = default;
    ~Application() = default;

    int Run();

    void Quit();
};
