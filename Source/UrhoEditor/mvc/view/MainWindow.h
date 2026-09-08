#pragma once
#include <vector>
#include <functional>
#include "EditorWidget.h"
#include "StartView.h"
#include "Menubar.h"
#include "Toolbar.h"
#include "NodeTree.h"
#include "FolderTree.h"
#include "Inspector.h"
#include "ConsoleView.h"
#include "ResPreview.h"
#include "FolderFiles.h"
#include "SceneView.h"
#include "ctrl/scene/SelectionController.h"
#include "model/SelectionModel.h"
#include "ctrl/scene/ToolController.h"
#include "ctrl/scene/GizmoController.h"
#include "ctrl/scene/CameraCtrl.h"
#include "ctrl/scene/PropertyEditController.h"
#include "ctrl/scene/SceneManipulationController.h"
#include "ctrl/res/ProjectController.h"
#include "ctrl/scene/SceneCtrl.h"
#include "ctrl/res/HistoryMgr.h"

namespace Urho3DEditor
{

class AssetBrowserController;

struct MainWindowServices
{
    std::function<void()> startGame;
    std::function<Urho3D::String()> selectPath;
    std::function<int()> getFps;
    std::function<void()> updateCmdGuid;
    std::function<bool()> isStartView;
    std::function<void(int)> setDpi;
    std::function<void(int)> setFontSize;
    std::function<int()> getFontSize;
};

//glfw needs a window. This becomes our dockspace
class MainWindow {
public:
    MainWindow(int width, int height, SelectionController& selectionCtrl, SelectionModel& selectionModel, ToolController& toolCtrl, GizmoController& gizmoCtrl, CameraCtrl& cameraCtrl, PropertyEditController& propEditCtrl, SceneManipulationController& sceneManipCtrl, ProjectController& projectCtrl, SceneCtrl& sceneCtrl, AssetBrowserController& assetBrowserCtrl, HistoryMgr& historyMgr, const MainWindowServices& services, float dpiScale);
    ~MainWindow();
    void Draw()const;
    bool WindowShouldClose()const;
    int GetWidth()const { return width; }
    int GetHeight()const { return height; }
    void Resize(int width, int height);
    GLFWwindow* GetRawWindow();
    virtual void Update();
    //void showDockSpace(bool* p_open);
    void ImguiUpdate();
    bool ShouldClose() { return WindowShouldClose(); }
    void AddWindow(std::unique_ptr<EditorWidget> newWindow)
    {
        windows.push_back(std::move(newWindow));
    }

    void MaxSize();
    GLFWwindow* GetGlfwWindow() {
        return window;
    }
    void StartGame();
    void ShowDemo(bool show)
    {
        _showDemo = show;
    }
    bool IsShowDemo()
    {
        return _showDemo;
    }
    void MakeCurrent();
    void OnWindowSize(int width, int height);
private:
    void UpdateDockerSpace();
    std::vector<std::unique_ptr<EditorWidget>> windows;
    SelectionController& selectionController_;
    SelectionModel& selectionModel_;
    ToolController& toolController_;
    GizmoController& gizmoController_;
    CameraCtrl& cameraController_;
    PropertyEditController& propertyEditController_;
    SceneManipulationController& sceneManipController_;
    ProjectController& projectController_;
    SceneCtrl& sceneCtrl_;
    AssetBrowserController& assetBrowserController_;
    HistoryMgr& historyMgr_;
    MainWindowServices services_;
    float dpiScale_;
    bool miniSize_ = false;
    int width, height;
    GLFWwindow* window = nullptr;
    //MainMenu* menuBar = nullptr;
    std::shared_ptr<StartView> _startView;
    std::shared_ptr<Menubar> _menuBar;
    std::shared_ptr<Toolbar> _toolBar;
    NodeTree* _nodeTree = nullptr;
    FolderTree* _folderTree = nullptr;
    Inspector* _inspector = nullptr;
    ResPreview* _resPreview = nullptr;
    //ConsoleView* _cosoleView = nullptr;
    FolderFiles* _folderFiles = nullptr;
    SceneView* _sceneView = nullptr;
    bool done = false;
    bool _showDemo = false;
    bool _showingDocker = true;
    bool _lastLeftMouseDown = false;

};

}

