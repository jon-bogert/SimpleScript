#pragma once
#include "WindowManager.h"

#include "Project.h"
#include "Editor/Editor.h"

#include <memory>

class Application final
{
    Application() {}

public:
    static Application& Get() { static Application instance; return instance; }

    ~Application() = default;
    Application(const Application& other) = delete;
    Application(const Application&& other) = delete;
    Application operator=(const Application& other) = delete;
    Application operator=(const Application&& other) = delete;

    void Start();
    void Update();
    void Shutdown();

    float GetScroll() const { return m_scroll; }

    Project* GetActiveProject();
    const Project* GetActiveProject() const;

private:
    WindowManager m_windowManager;
    std::unique_ptr<Project> m_activeProject = nullptr;
    std::unique_ptr<Editor> m_editor = nullptr;

    float m_scroll = 0.f;

};

