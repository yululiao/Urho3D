#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "Urho3D/Container/Str.h"
#include "GLSDLWindow.h"
#include "gizmo/TransformCtrl.h"
#include "CameraCtrl.h"

using namespace Urho3D;

class EditorApp :public Object
{
	URHO3D_OBJECT(EditorApp, Object);
public:
	EditorApp(Context* context);
	~EditorApp();
	void run();
	void openWorkSpace(const String& path);
	Context* getContext() { return _context; }
    static EditorApp* getInstance();
	void createEngine(void* win_ptr);
	void runFrame();
	void resizeWwindow(int w,int h);
    ImguiUpdater* getUiUpdater();
    MenuBarUpdater* getManuBarUpdater();
    UMainWindow* mainWindow = nullptr;
    void setCurTool(const String& name);
    Node* getRootNode();
    Scene* getScene();
    String getWorkSpace(); //{ return _work_space; }
    String getAssetRoot(); //{ return _work_space + "/assets"; }
    void showSceneView(bool show);
    void startGame();
    String dialogSelectPath();
    String dialogOpenFile();
    void dialogSaveFile();
    void Clear();
    /// <summary>
    /// 执行一个命令行
    /// </summary>
    /// <param name="cmd">命令行</param>
    /// <param name="pRetMsg">执行返回值</param>
    /// <param name="msg_len"></param>
    /// <returns></returns>
    int system(const char* cmd, char* pRetMsg, int msg_len);
    Node* GetSceneRoot();
    void SelectNode(unsigned id);
protected:
	void handleLogMessage(StringHash eventType, VariantMap& eventData);
	Context* _context = nullptr;

	void setup();
	void start();
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
    renderWindow* _sceneView = nullptr;
    bool _gameStarted = false;

public:
	CameraCtrl* cam_ctrl_ = nullptr;
   // SharedPtr<TransformCtrl> _gizmoCtrl = nullptr;
	TransformCtrl* gizmoCtrl_ = nullptr;
	String  _curent_tool;
    static  String _getPathResult;
};

