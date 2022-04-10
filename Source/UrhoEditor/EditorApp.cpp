#include "EditorApp.h"
#include "Urho3D/Precompiled.h"
#include "Urho3D/IO/IOEvents.h"
#include "Urho3D/IO/Log.h"

#include "Urho3D/DebugNew.h"
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>

#include <Urho3D/LuaScript/LuaScript.h>
#include "SceneCtrl.h"
//#include "glfw/glfw3native.h"
#include "EditorLuaBinding.h"
#include "ImGuiFileBrowser.h"
#ifdef _WIN32
#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>
#include <Urho3D/Resource/ResourceCache.h>
#endif // _WIN32

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

}

void EditorApp::createEngine(void* win_ptr)
{
	_window_ptr = win_ptr;
	_engineParameters = Engine::ParseParameters(GetArguments());
	// Create the Engine, but do not initialize it yet. Subsystems except Graphics & Renderer are registered at this point
	_engine = new Engine(_context);
	// Subscribe to log messages so that can show errors if ErrorExit() is called with empty message
	SubscribeToEvent(E_LOGMESSAGE, URHO3D_HANDLER(EditorApp, handleLogMessage));
	setup();
	if (!_engine->Initialize(_engineParameters))
	{
		return;
	}
	start();
    
}

void EditorApp::setup()
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

void EditorApp::start()
{
    auto* luaScript = new LuaScript(context_);
    EditorLuaBinding::LuaBinding(luaScript->GetState());
    context_->RegisterSubsystem(luaScript);
    luaScript->ExecuteFile("EditorLua/main.lua");
	SceneCtrl::getInstance()->createScene();
    setCurTool("camera");
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

void EditorApp::runFrame()
{
	SceneCtrl::getInstance()->update();
	_engine->RunFrame();
    SceneCtrl::getInstance()->genRttTex();
    if (gizmoCtrl_)
        gizmoCtrl_->update();
}

void EditorApp::resizeWwindow(int w, int h)
{
    //auto* graphics = GetSubsystem<Graphics>();
	//graphics->SetMode(w, h);
}

imgui_addons::ImGuiFileBrowser file_dialog;
String EditorApp::dialogSelectPath() 
{
    //String result;
    //ImGui::OpenPopup("Select Path");
    // //if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,ImVec2(200, 200),".rar,.zip,.7z"))
    //if (file_dialog.showFileDialog("Select Path",imgui_addons::ImGuiFileBrowser::DialogMode::SELECT, ImVec2(200, 200)))
    //{
    //    std::string selected_file = file_dialog.selected_fn;
    //    std::string path = file_dialog.selected_path;
    //    result = path.c_str();
    //}
    //return result;
#ifdef _WIN32
    TCHAR path[MAX_PATH];

    char path_param[] = "C:\\Windows"; // saved_path.c_str();
    BROWSEINFO bi = {0};
    bi.lpszTitle = ("Select Path");
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = (LPARAM)path_param;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    if (pidl != 0)
    {
        // get the name of the folder and put it in path
        SHGetPathFromIDList(pidl, path);

        // free memory used
        IMalloc* imalloc = 0;
        if (SUCCEEDED(SHGetMalloc(&imalloc)))
        {
            imalloc->Free(pidl);
            imalloc->Release();
        }

        return String(path);
    }
    else
    {
        return String("");
    }


#endif // _WIN32

    
}

String EditorApp::dialogOpenFile() { return String(); }

void EditorApp::dialogSaveFile() {}

int EditorApp::system(const char* cmd, char* pRetMsg, int msg_len) 
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

void EditorApp::SelectNode(unsigned id)
{
    Node* node = getScene()->GetNode(id);
    if(node && gizmoCtrl_ && _curent_tool != "camera")
    {
        gizmoCtrl_->attach(node);
    }
}

void EditorApp::handleLogMessage(StringHash eventType, VariantMap& eventData)
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

void EditorApp::startGame() 
{ 
    _gameStarted = true;
    _sceneView = new renderWindow("renderWindow");
    mainWindow->AddWindow(std::unique_ptr<renderWindow>(_sceneView));
    mainWindow->maxSize();
    auto* cache = GetSubsystem<ResourceCache>();
    cache->AddResourceDir(_work_space);
};

void EditorApp::run() 
{
	/*_start_ui = new start_view();
	_start_ui->show();*/
	//UMainWindow ui(800, 600);
    mainWindow = new UMainWindow(800,600);
    mainWindow->createUiUpdaer(_context);
    //HWND winid = glfwGetWin32Window(ui.getRawWindow()); 
    mainWindow->AddWindow(std::unique_ptr<DockerContainer>(new DockerContainer()));
    //StartWindow* startWin = new StartWindow("Urho3D");
    //startWin->mainWindow = mainWindow;
    //mainWindow->AddWindow(std::unique_ptr<StartWindow>(startWin));
    createEngine(nullptr);
    while (!mainWindow->shouldClose())
    {
        mainWindow->Update();
        if (_gameStarted)
        {
            runFrame();
        }
    }
    //return 0;
}

void EditorApp::openWorkSpace(const String& path)
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

EditorApp* EditorApp::getInstance()
{
    if (_instance == nullptr)
    {

        _instance = new EditorApp(new Context());
    }
    return _instance;
}

ImguiUpdater* EditorApp::getUiUpdater() 
{ 
	return mainWindow->_imguiUpdater;
}

MenuBarUpdater* EditorApp::getManuBarUpdater() 
{ 
	return mainWindow->_menuBarUpdater;
    
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

void EditorApp::setCurTool(const String& name) 
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

Node* EditorApp::getRootNode() 
{
	return SceneCtrl::getInstance()->rttSceneRoot_; 
}

Scene* EditorApp::getScene() 
{ 
	return SceneCtrl::getInstance()->rttScene_; 
}

String EditorApp::getWorkSpace() 
{ 
	return _work_space; 
}

String EditorApp::getAssetRoot() { return String(); }

void EditorApp::showSceneView(bool show) 
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

