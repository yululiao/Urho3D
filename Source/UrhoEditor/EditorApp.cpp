#include "EditorApp.h"
#include "Urho3D/Precompiled.h"
#include "Urho3D/IO/IOEvents.h"
#include "Urho3D/IO/Log.h"

#include "Urho3D/DebugNew.h"
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>

#include <Urho3D/LuaScript/LuaScript.h>
#include "ctrls/SceneCtrl.h"
//#include "glfw/glfw3native.h"
//#include "EditorLuaBinding.h"//todo
#include "ImGuiFileBrowser.h"
#ifdef _WIN32
#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <Urho3D/Resource/ResourceCache.h>
#endif // _WIN32
#include <ndf/nfd.h>
#include "ctrls/AssetMgr.h"


namespace Urho3DEditor {
String EditorApp::_getPathResult;
EditorApp* EditorApp::_instance = nullptr;
EditorApp::EditorApp(Context* context)
	:Object(context)
{
    _context = context;
    SceneCtrl::_ctx = _context;
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
	_engine = new Engine(_context);
	// Subscribe to log messages so that can show errors if ErrorExit() is called with empty message
	SubscribeToEvent(E_LOGMESSAGE, URHO3D_HANDLER(EditorApp, HandleLogMessage));
	Setup();
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
        //_engineParameters[EP_RESOURCE_PREFIX_PATHS] = ";../share/Resources;../share/Urho3D/Resources";
		//todo
        //_work_space = "D:/Urho3D/platform/projects/test";
        String assetsPath = "Data;CoreData;" + _work_space;
        _engineParameters[EP_RESOURCE_PATHS] =  assetsPath;

    }

}

void EditorApp::Start()
{
    auto* luaScript = new LuaScript(context_);
    //EditorLuaBinding::LuaBinding(luaScript->GetState());//todo
    context_->RegisterSubsystem(luaScript);
    luaScript->ExecuteFile("EditorLua/main.lua");
	SceneCtrl::getInstance()->createScene();
    SetCurTool("move");
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

void EditorApp::RunFrame()
{
	SceneCtrl::getInstance()->update();
	_engine->RunFrame();
    SceneCtrl::getInstance()->genRttTex();
    if (gizmoCtrl_)
        gizmoCtrl_->update();
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

Node* EditorApp::GetSceneRoot()
{
   return SceneCtrl::getInstance()->getRoot();
}

void EditorApp::SelectNode(Node* node)
{
    _selectedNode = node;
    if(gizmoCtrl_ && _curent_tool != "camera")
    {
        gizmoCtrl_->attach(node);
    }
}

Node* EditorApp::GetSelectNode() 
{
    return _selectedNode;
}

void EditorApp::MakeCurent() 
{
    GetSubsystem<Graphics>()->MakeCurrent();
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
    _gameStarted = true;
    _sceneView = new SceneView("renderWindow");
    mainWindow->AddWindow(std::unique_ptr<SceneView>(_sceneView));
    mainWindow->StartGame();
    mainWindow->MaxSize();
    auto* cache = GetSubsystem<ResourceCache>();
    cache->AddResourceDir(_work_space);
    _isStartView = false;
};

void EditorApp::Run() 
{
	/*_start_ui = new start_view();
	_start_ui->show();*/
	//UMainWindow ui(800, 600);
    NFD_Init();
    mainWindow = new MainWindow(800,600);
    //HWND winid = glfwGetWin32Window(ui.getRawWindow()); 
    //mainWindow->AddWindow(std::unique_ptr<DockerContainer>(new DockerContainer()));
    CreateEngine(nullptr);
    while (!mainWindow->ShouldClose())
    {
        mainWindow->Update();
        if (_gameStarted)
        {
            RunFrame();
        }
    }
    //return 0;
}

void EditorApp::OpenWorkSpace(const String& path)
{
	_work_space = path;
	//work_space::get_instance()->set_workspace(path);
	//std::string title = "urho3d   " + path + "*";
	//render_view* render_v = new render_view(nullptr);
	//render_v->show();

	//_main_window = new main_window(nullptr);
	//_main_window->setWindowTitle(title.c_str());
	//_main_window->showMaximized();
}

EditorApp* EditorApp::GetInstance()
{
    if (_instance == nullptr)
    {

        _instance = new EditorApp(new Context());
    }
    return _instance;
}

void EditorApp::Clear()
{ 
    if (gizmoCtrl_)
    {
        delete gizmoCtrl_;
        gizmoCtrl_ = nullptr;
    }
    if (cam_ctrl_)
    {
        delete cam_ctrl_;
        cam_ctrl_ = nullptr;
    }
   
}

void EditorApp::SetCurTool(const String& name) 
{
    GetSubsystem<Graphics>()->MakeCurrent();
    if (cam_ctrl_ == nullptr)
    {
        cam_ctrl_ = new CameraCtrl(SceneCtrl::getInstance()->rttCameraNode_);
    }
    if (gizmoCtrl_ == NULL && SceneCtrl::getInstance()->rttScene_)
    {
        Node* gizmoRoot = SceneCtrl::getInstance()->GetEditorRoot()->CreateChild("gizmoRoot"); 
        gizmoCtrl_ = new TransformCtrl(context_, eTransformCtrlMode::eTranslate, gizmoRoot);
        gizmoCtrl_->setScene(SceneCtrl::getInstance()->rttScene_);
        gizmoCtrl_->setCameraNode(SceneCtrl::getInstance()->rttCameraNode_);
    }
    _curent_tool = name;
    if (name == "move")
    {
        gizmoCtrl_->setMode(eTransformCtrlMode::eTranslate);
    }
    else if (name == "rotate")
    {
        gizmoCtrl_->setMode(eTransformCtrlMode::eRotate);
    }
    else if (name == "scale")
    {
        gizmoCtrl_->setMode(eTransformCtrlMode::eScale);
    }
    else if (name == "camera")
    {
        if (gizmoCtrl_)
            gizmoCtrl_->detach();
    }
}

Node* EditorApp::GetRootNode() 
{
	return SceneCtrl::getInstance()->rttSceneRoot_; 
}

Scene* EditorApp::GetScene() 
{ 
	return SceneCtrl::getInstance()->rttScene_; 
}

String EditorApp::GetWorkSpace() 
{ 
	return _work_space; 
}

String EditorApp::GetAssetRoot() 
{
    return _work_space + "/assets";
}

void EditorApp::ShowSceneView(bool show) 
{ 
    if (show)
        _sceneView->Show();
    else
        _sceneView->Hide();
}

//EditorMenu* editor_app::AddMenu(const String path) 
//{ 
//	QAction* act = _main_window->GetMenuBar()->add_menu(path.CString());
//    EditorMenu* menu = new EditorMenu(_context,act);
//    return menu;
//}

}

