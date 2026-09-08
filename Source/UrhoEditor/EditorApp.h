#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "model/SelectionModel.h"
#include "ctrl/scene/GizmoController.h"
#include "ctrl/scene/SelectionController.h"
#include "ctrl/scene/ToolController.h"
#include "model/ToolModel.h"
#include "ctrl/scene/CameraCtrl.h"
#include "ctrl/scene/PropertyEditController.h"
#include "ctrl/scene/SceneManipulationController.h"
#include "ctrl/scene/SceneCtrl.h"
#include "ctrl/base/CmdMgr.h"
#include "ctrl/res/HistoryMgr.h"
#include "ctrl/res/AssetBrowserController.h"

using namespace Urho3D;
namespace Urho3DEditor 
{

class MainWindow;
class ProjectController;

class EditorApp :public Object
{
	URHO3D_OBJECT(EditorApp, Object);
public:
	EditorApp(Context* context);
	~EditorApp();
	void Run();
	void CreateEngine(void* win_ptr);
	void RunEngineFrame();
	void ResizeWwindow(int w,int h);

    void EditorOneFrame();
   
    void SetCurTool(const String& name);
    Node* GetRootNode();
    Scene* GetScene();
    void StartGame();
    String DialogSelectPath();
    String DialogOpenFile(Urho3D::Vector<String> filer);
    void DialogSaveFile();
    void Clear();
    /// <summary>
    /// ִ��һ��������
    /// </summary>
    /// <param name="cmd">������</param>
    /// <param name="pRetMsg">ִ�з���ֵ</param>
    /// <param name="msg_len"></param>
    /// <returns></returns>
    int System(const char* cmd, char* pRetMsg, int msg_len);
    SelectionModel* GetSelectionModel() const { return selectionModel_; }
    void MakeCurent();
    int GetFps(){
        return _fps;
    }
    String GetLastCmdGuid()
    {
        return _lastCmdGuid;
    }
    void UpdateCmdGuid();
    void SetDpi(int dpi) 
    {
        _dpi = dpi;
    }
    int GetDpi()
    { 
        return _dpi;
    }
    float GetDpiScale() 
    { 
        return _dpi / 96.0f;
    }
    void SetFontSize(int fontSize)
    {
        _fontSize = fontSize;
    }
    int GetFontSize()
    {
        return _fontSize;
    }
protected:
	void HandleLogMessage(StringHash eventType, VariantMap& eventData);
	void Setup();
	void Start();
    static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

private:
	/// Urho3D engine.
	SharedPtr<Engine> _engine;
	/// Engine parameters map.
	VariantMap _engineParameters;
	String _startupErrors;
	void* _window_ptr = nullptr;
    bool _gameStarted = false;
    time_t _lastTime = -1;
    int _fps = 60;
    String _lastCmdGuid;
    int _dpi = 96;
    int _fontSize = 15;

public:
    static  String _getPathResult;
    bool _isStartView = true;
    bool miniSize = false;

    MainWindow* mainWindow = nullptr;

    SelectionModel* selectionModel_ = nullptr;
    GizmoController* gizmoController_ = nullptr;
    SelectionController* selectionController_ = nullptr;
    ToolModel* toolModel_ = nullptr;
    ToolController* toolController_ = nullptr;
    CameraCtrl* cameraController_ = nullptr;
    PropertyEditController* propertyEditController_ = nullptr;
    SceneManipulationController* sceneManipController_ = nullptr;
    ProjectController* projectController_ = nullptr;
    SceneCtrl* sceneCtrl_ = nullptr;
    CmdMgr* cmdMgr_ = nullptr;
    HistoryMgr* historyMgr_ = nullptr;
    AssetMgr* assetMgr_ = nullptr;
    AssetBrowserController* assetBrowserController_ = nullptr;
   
};
}

