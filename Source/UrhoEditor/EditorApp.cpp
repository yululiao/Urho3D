#include "EditorApp.h"
#include "Urho3D/Precompiled.h"
#include "Urho3D/IO/IOEvents.h"
#include "Urho3D/IO/Log.h"

#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>

#include <Urho3D/LuaScript/LuaScript.h>
#include "ctrl/scene/SceneCtrl.h"
//#include "glfw/glfw3native.h"
//#include "EditorLuaBinding.h"//todo
#include "ImGuiFileBrowser.h"
#include "view/inspectors/VariantDrawer.h"
#include "view/MainWindow.h"
#ifdef _WIN32
#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <Urho3D/Resource/ResourceCache.h>
#endif // _WIN32
#include <view/ndf/nfd.h>
#include "ctrl/res/AssetMgr.h"
#include "ctrl/res/ProjectController.h"
#include "Utils.h"
#include "Global.h"

#include "Urho3D/DebugNew.h"

namespace Urho3DEditor {
String EditorApp::_getPathResult;
EditorApp::EditorApp(Context* context)
	:Object(context)
{
    _lastCmdGuid = String(Utils::GenGuid().c_str());
}

EditorApp::~EditorApp() 
{ 
   NFD_Quit(); 
}

void EditorApp::CreateEngine(void* win_ptr)
{
	_window_ptr = win_ptr;
	_engineParameters = Engine::ParseParameters(GetArguments());
	// Create the Engine, but do not initialize it yet. Subsystems except Graphics & Renderer are registered at this point
	_engine = new Engine(context_);
	// Subscribe to log messages so that can show errors if ErrorExit() is called with empty message
	SubscribeToEvent(E_LOGMESSAGE, URHO3D_HANDLER(EditorApp, HandleLogMessage));
	Setup();
    // Auto-reload resources when modified (e.g. shader edits trigger prefab refresh)
    auto* cache = GetSubsystem<ResourceCache>();
    cache->SetAutoReloadResources(true);
	if (!_engine->Initialize(_engineParameters))
	{
		return;
	}
	Start();
    
}

void EditorApp::Setup()
{
	_engineParameters[EP_EXTERNAL_WINDOW] = _window_ptr;
    _engineParameters[EP_WINDOW_HIDE] = true;
	//_engineParameters[EP_WINDOW_TITLE] = GetTypeName();
	//_engineParameters[EP_WINDOW_RESIZABLE] = true;
	_engineParameters[EP_LOG_NAME] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
    //_engineParameters[EP_WINDOW_POSITION_X] = 10000;
    //_engineParameters[EP_WINDOW_POSITION_Y] = 10000;
	_engineParameters[EP_FULL_SCREEN] = false;
	//_engineParameters[EP_HEADLESS] = false;
	//_engineParameters[EP_SOUND] = true;
	// Construct a search path to find the resource prefix with two entries:
	// The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
	// The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
	if (!_engineParameters.Contains(EP_RESOURCE_PREFIX_PATHS))
    {
        String assetsPath = "Data;CoreData;";
        _engineParameters[EP_RESOURCE_PATHS] = assetsPath;
    }

}

void EditorApp::Start()
{
    auto* luaScript = new LuaScript(context_);
    //EditorLuaBinding::LuaBinding(luaScript->GetState());//todo
    context_->RegisterSubsystem(luaScript);
    luaScript->ExecuteFile("EditorLua/main.lua");
    sceneCtrl_ = new SceneCtrl(context_);
	sceneCtrl_->CreateScene();
}

int EditorApp::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) 
{ 
    if(uMsg == BFFM_INITIALIZED)
    {
        std::string tmp = (const char*)lpData;
        std::cout << "path: " << tmp << std::endl;
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
        _getPathResult = tmp.c_str();
    }
    return 0;
}

void EditorApp::RunEngineFrame()
{
	sceneCtrl_->Update();
	_engine->RunFrame();
    sceneCtrl_->GenRttTex();
    if (gizmoController_)
        gizmoController_->update();
}

void EditorApp::ResizeWwindow(int w, int h)
{
    //auto* graphics = GetSubsystem<Graphics>();
	//graphics->SetMode(w, h);
}

String EditorApp::DialogSelectPath() 
{
    String dirPath;
    nfdchar_t* outPath;
    nfdchar_t* defaultPath = "";
    nfdresult_t result = NFD_PickFolder(&outPath, defaultPath);
    if (result == NFD_OKAY)
    {
        puts("Success!");
        puts(outPath);
        dirPath = String(outPath);
        NFD_FreePath(outPath);
        dirPath.Replace('\\', '/');
        return dirPath;
    }
    else if (result == NFD_CANCEL)
    {
        puts("User pressed cancel.");
    }
    else
    {
        printf("Error: %s\n", NFD_GetError());
    }
    return dirPath;
}

String EditorApp::DialogOpenFile(Urho3D::Vector<String> filter)
{ 
    String filePath;
    nfdchar_t* outPath;
    //nfdfilteritem_t filterItem[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
    nfdfilteritem_t filterItem[1] = {{filter[0].CString(), filter[1].CString()}};
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, filter.Size()/2, NULL);
    if (result == NFD_OKAY)
    {
        puts("Success!");
        puts(outPath);
        filePath = String(outPath);
        NFD_FreePath(outPath);
        filePath.Replace('\\', '/');
        return filePath;
    }
    else if (result == NFD_CANCEL)
    {
        puts("User pressed cancel.");
    }
    else
    {
        printf("Error: %s\n", NFD_GetError());
    }
   return filePath;
}

void EditorApp::DialogSaveFile() 
{

}

int EditorApp::System(const char* cmd, char* pRetMsg, int msg_len) 
{
    FILE* fp;
    char* p = NULL;
    int res = -1;
    if (cmd == NULL || pRetMsg == NULL || msg_len < 0)
    {
        printf("Param Error!\n");
        return -1;
    }
    if ((fp = _popen(cmd, "r")) == NULL)
    {
        printf("Popen Error!\n");
        return -2;
    }
    else
    {
        memset(pRetMsg, 0, msg_len);
        // get lastest result
        while (fgets(pRetMsg, msg_len, fp) != NULL)
        {
            printf("Msg:%s", pRetMsg); // print all info
        }

        if ((res = _pclose(fp)) == -1)
        {
            printf("close popenerror!\n");
            return -3;
        }
        // pRetMsg[strlen(pRetMsg) - 1] = '\0';
        return 0;
    }
}

void EditorApp::MakeCurent() 
{
    GetSubsystem<Graphics>()->MakeCurrent();
}

void EditorApp::UpdateCmdGuid()
{
    _lastCmdGuid = String(Utils::GenGuid().c_str());
}

void EditorApp::HandleLogMessage(StringHash eventType, VariantMap& eventData)
{
	using namespace LogMessage;

	if (eventData[P_LEVEL].GetInt() == LOG_ERROR)
	{
		// Strip the timestamp if necessary
		String error = eventData[P_MESSAGE].GetString();
		unsigned bracketPos = error.Find(']');
		if (bracketPos != String::NPOS)
			error = error.Substring(bracketPos + 2);
		_startupErrors += error + "\n";
	}
}

void EditorApp::StartGame() 
{ 
    // Refresh asset cache and register workspace resource directory
    assetMgr_->RefreshResCache(assetMgr_->GetAssetRoot(),true);
    auto* cache = GetSubsystem<ResourceCache>();
    cache->AddResourceDir(assetMgr_->GetWorkSpace());
    _gameStarted = true;
    mainWindow->MaxSize();
    _isStartView = false;
};


int gFpsFrameCount = 0;
int gAllFps = 0;
static const float gCalculateFpsNum = 30.0f;
void EditorApp::EditorOneFrame()
{
    time_t nowTime = GetTickCount64();
    if(_lastTime > 0 && nowTime >_lastTime)
    {
        int tmpFps = (int)(1000.0f/(nowTime - _lastTime));
        if(gFpsFrameCount < gCalculateFpsNum)
        {
            gAllFps += tmpFps;
        }
        else
        {
            _fps = (int)(gAllFps / gCalculateFpsNum);
            gFpsFrameCount = 0;
            gAllFps = 0;
        }
    }
    _lastTime = nowTime;
    mainWindow->Update();
    if (_gameStarted && !miniSize) {
        RunEngineFrame();
    }
    ++gFpsFrameCount;
}

void EditorApp::Run() 
{
	/*_start_ui = new start_view();
	_start_ui->show();*/
	//UMainWindow ui(800, 600);
    NFD_Init();
    //HWND winid = glfwGetWin32Window(ui.getRawWindow()); 
    //mainWindow->AddWindow(std::unique_ptr<DockerContainer>(new DockerContainer()));
    CreateEngine(nullptr);
    VariantDrawer::SetDialogOpenFileHandler([this](const Vector<String>& filter) { return DialogOpenFile(filter); });

    // Create MVC objects after engine/scene are ready
    selectionModel_ = new SelectionModel();
    cmdMgr_ = new CmdMgr();
    historyMgr_ = new HistoryMgr(context_);
    assetMgr_ = new AssetMgr(context_);
    Node* gizmoRoot = sceneCtrl_->GetEditorRoot()->CreateChild("gizmoRoot");
    gizmoController_ = new GizmoController(context_, gizmoRoot, sceneCtrl_->GetScene(), sceneCtrl_->rttCameraNode_, *cmdMgr_);
    selectionController_ = new SelectionController(*selectionModel_, *gizmoController_, *sceneCtrl_);
    toolModel_ = new ToolModel();
    toolController_ = new ToolController(*toolModel_, *gizmoController_);
    cameraController_ = new CameraCtrl(sceneCtrl_->rttCameraNode_);
    propertyEditController_ = new PropertyEditController(*cmdMgr_, *assetMgr_);
    sceneManipController_ = new SceneManipulationController(*sceneCtrl_, *cmdMgr_, *assetMgr_);
    projectController_ = new ProjectController(*sceneCtrl_, *assetMgr_, *cmdMgr_, *sceneManipController_,
                                               *selectionController_, *toolController_);
    assetBrowserController_ = new AssetBrowserController(*assetMgr_, *selectionController_);

    MainWindowServices services;
    services.startGame = [this]() { StartGame(); };
    services.selectPath = [this]() { return DialogSelectPath(); };
    services.getFps = [this]() { return GetFps(); };
    services.updateCmdGuid = [this]() { UpdateCmdGuid(); };
    services.isStartView = [this]() { return _isStartView; };
    services.setDpi = [this](int dpi) { SetDpi(dpi); };
    services.setFontSize = [this](int fontSize) { SetFontSize(fontSize); };
    services.getFontSize = [this]() { return GetFontSize(); };

    mainWindow = new MainWindow(800, 520, *selectionController_, *selectionModel_, *toolController_, *gizmoController_,
                                *cameraController_, *propertyEditController_, *sceneManipController_,
                                *projectController_, *sceneCtrl_, *assetBrowserController_, *historyMgr_, services, GetDpiScale());

  

    while (!mainWindow->ShouldClose())
    {
        EditorOneFrame();
    }
    //return 0;
}

void EditorApp::Clear()
{ 
    if (gizmoController_)
    {
        delete gizmoController_;
        gizmoController_ = nullptr;
    }
    if (cameraController_)
    {
        delete cameraController_;
        cameraController_ = nullptr;
    }
    if (toolController_)
    {
        delete toolController_;
        toolController_ = nullptr;
    }
    if (toolModel_)
    {
        delete toolModel_;
        toolModel_ = nullptr;
    }
    if (selectionController_)
    {
        delete selectionController_;
        selectionController_ = nullptr;
    }
    if (selectionModel_)
    {
        delete selectionModel_;
        selectionModel_ = nullptr;
    }
    if (propertyEditController_)
    {
        delete propertyEditController_;
        propertyEditController_ = nullptr;
    }
    if (sceneManipController_)
    {
        delete sceneManipController_;
        sceneManipController_ = nullptr;
    }
    if (projectController_)
    {
        delete projectController_;
        projectController_ = nullptr;
    }
    if (assetBrowserController_)
    {
        delete assetBrowserController_;
        assetBrowserController_ = nullptr;
    }
    if (cmdMgr_)
    {
        delete cmdMgr_;
        cmdMgr_ = nullptr;
    }
    if (historyMgr_)
    {
        delete historyMgr_;
        historyMgr_ = nullptr;
    }
    if (assetMgr_)
    {
        delete assetMgr_;
        assetMgr_ = nullptr;
    }
    if (sceneCtrl_)
    {
        delete sceneCtrl_;
        sceneCtrl_ = nullptr;
    }
}

void EditorApp::SetCurTool(const String& name) 
{
    GetSubsystem<Graphics>()->MakeCurrent();
    if (toolController_)
        toolController_->SetTool(name);
}

Node* EditorApp::GetRootNode() 
{
	return sceneCtrl_ ? sceneCtrl_->rttSceneRoot_ : nullptr; 
}

Scene* EditorApp::GetScene() 
{ 
	return sceneCtrl_ ? sceneCtrl_->rttScene_ : nullptr; 
}
//EditorMenu* editor_app::AddMenu(const String path) 
//{ 
//	QAction* act = _main_window->GetMenuBar()->add_menu(path.CString());
//    EditorMenu* menu = new EditorMenu(_context,act);
//    return menu;
//}

}

