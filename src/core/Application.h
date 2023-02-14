#pragma once

class Application
{
private:
    void Init();
    void Event();
    void Render();
public:
    Application() = default;
    ~Application() = default;

    int Run();
};
