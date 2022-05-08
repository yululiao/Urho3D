#pragma once
#include <vector>
#include "EditorWidget.h"
#include "StartView.h"
#include "Menubar.h"
#include "Toolbar.h"
#include "NodeTree.h"
#include "ResTree.h"
#include "Inspector.h"
#include "ConsoleView.h"
#include "ResPreview.h"

namespace Urho3DEditor
{

//glfw needs a window. This becomes our dockspace
class MainWindow {
public:
    MainWindow(int width, int height);
    ~MainWindow();
    void Draw()const;
    bool WindowShouldClose()const;
    int getWidth()const { return width; }
    int getHeight()const { return height; }
    void Resize(int width, int height);
    GLFWwindow* getRawWindow();
    virtual void Update();
    //void showDockSpace(bool* p_open);
    void imguiUpdate();
    bool shouldClose() { return WindowShouldClose(); }
    void AddWindow(std::unique_ptr<EditorWidget> newWindow)
    {
        windows.push_back(std::move(newWindow));
    }

    void maxSize();
    GLFWwindow* getGlfwWindow() {
        return window;
    }
    void StartGame();
private:
    void UpdateDockerSpace();
    std::vector<std::unique_ptr<EditorWidget>> windows;
    int width, height;
    GLFWwindow* window = nullptr;
    //MainMenu* menuBar = nullptr;
    std::shared_ptr<StartView> _startView;
    std::shared_ptr<Menubar> _menuBar;
    std::shared_ptr<Toolbar> _toolBar;
    NodeTree* _nodeTree = nullptr;
    ResTree* _resTree = nullptr;
    Inspector* _inspector = nullptr;
    ResPreview* _resPreview = nullptr;
    ConsoleView* _cosoleView = nullptr;
    bool done = false;
    bool _showDemo = false;
    bool _showingDocker = true;

};

}

