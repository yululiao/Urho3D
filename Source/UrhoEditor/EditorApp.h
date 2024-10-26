#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "views/MainWindow.h"
#include "views/SceneView.h"
#include "gizmo/TransformCtrl.h"
#include "ctrls/CameraCtrl.h"

using namespace Urho3D;
namespace Urho3DEditor 
{

class EditorApp :public Object
{
	URHO3D_OBJECT(EditorApp, Object);
public:
	EditorApp(Context* context);
	~EditorApp();
	void Run();
	void OpenWorkSpace(const String& path);
	Context* GetContext() { return _context; }
    static EditorApp* GetInstance();
	void CreateEngine(void* win_ptr);
	void RunEngineFrame();
	void ResizeWwindow(int w,int h);

    void EditorOneFrame();
   
    void SetCurTool(const String& name);
    String GetCurTool(){return _curent_tool;}
    Node* GetRootNode();
    Scene* GetScene();
    String GetWorkSpace(); //{ return _work_space; }
    String GetAssetRoot(); //{ return _work_space + "/assets"; }
    void ShowSceneView(bool show);
    void StartGame();
    String DialogSelectPath();
    String DialogOpenFile(Urho3D::Vector<String> filer);
    void DialogSaveFile();
    void Clear();
    /// <summary>
    /// 执行一个命令行
    /// </summary>
    /// <param name="cmd">命令行</param>
    /// <param name="pRetMsg">执行返回值</param>
    /// <param name="msg_len"></param>
    /// <returns></returns>
    int System(const char* cmd, char* pRetMsg, int msg_len);
    Node* GetSceneRoot();
    void SelectNode(Node* node);
    Node* GetSelectNode();
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
	Context* _context = nullptr;

	void Setup();
	void Start();
    static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

private:
    static EditorApp* _instance;
	/// Urho3D engine.
	SharedPtr<Engine> _engine;
	/// Engine parameters map.
	VariantMap _engineParameters;
	String _startupErrors;
	void* _window_ptr = nullptr;
    String _work_space;
    SceneView* _sceneView = nullptr;
    bool _gameStarted = false;
    Node* _selectedNode = nullptr;
    time_t _lastTime = -1;
    int _fps = 60;
    String _lastCmdGuid;
    int _dpi = 96;
    int _fontSize = 15;

public:
    MainWindow* mainWindow = nullptr;
	CameraCtrl* cam_ctrl_ = nullptr;
   // SharedPtr<TransformCtrl> _gizmoCtrl = nullptr;
	TransformCtrl* gizmoCtrl_ = nullptr;
	String  _curent_tool;
    static  String _getPathResult;
    bool _isStartView = true;
    bool miniSize = false;
   
};
}

