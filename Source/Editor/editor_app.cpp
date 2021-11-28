#include <QAction>
#include "editor_app.h"
#include "view/styles/dark_style.h"
#include "view/start_view.h"
#include "ctrl/work_space.h"
#include "view/main_window.h"


#include "Urho3D/Precompiled.h"
#include "Urho3D/IO/IOEvents.h"
#include "Urho3D/IO/Log.h"

#include "Urho3D/DebugNew.h"
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>

#include "ctrl/gizmo/TransformCtrl.h"
#include "ctrl/utils.h"
#include "ctrl/scene_ctrl.h"
#include "ctrl/asset_mgr.h"

#include "ctrl/EditorLuaBinding.h"
#include <Urho3D/LuaScript/LuaScript.h>


namespace urho3d
{
namespace editor
{
editor_app* editor_app::_instance = nullptr;
editor_app::editor_app(Context* context)
	:Object(context)
{
	QApplication::setStyle(new DarkStyle());
	_context = context;
	scene_ctrl::_ctx = _context;
}

editor_app::~editor_app() 
{

}

void editor_app::create_engine(void* win_ptr)
{
	_window_ptr = win_ptr;
	_engineParameters = Engine::ParseParameters(GetArguments());
	// Create the Engine, but do not initialize it yet. Subsystems except Graphics & Renderer are registered at this point
	_engine = new Engine(_context);
	// Subscribe to log messages so that can show errors if ErrorExit() is called with empty message
	SubscribeToEvent(E_LOGMESSAGE, URHO3D_HANDLER(editor_app, HandleLogMessage));
	setup();
	if (!_engine->Initialize(_engineParameters))
	{
		return;
	}
	start();
}

void editor_app::setup()
{
	_engineParameters[EP_EXTERNAL_WINDOW] = _window_ptr;
	//_engineParameters[EP_WINDOW_TITLE] = GetTypeName();
	//_engineParameters[EP_WINDOW_RESIZABLE] = true;
	_engineParameters[EP_LOG_NAME] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
	//_engineParameters[EP_FULL_SCREEN] = false;
	//_engineParameters[EP_HEADLESS] = false;
	//_engineParameters[EP_SOUND] = true;
	// Construct a search path to find the resource prefix with two entries:
	// The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
	// The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
	if (!_engineParameters.Contains(EP_RESOURCE_PREFIX_PATHS))
    {
        //_engineParameters[EP_RESOURCE_PREFIX_PATHS] = ";../share/Resources;../share/Urho3D/Resources";
        std::string assetsPath = "Data;CoreData;"+ work_space::get_instance()->get_workspace();
        _engineParameters[EP_RESOURCE_PATHS] =  assetsPath.c_str();

    }

}

void editor_app::start()
{
    auto* luaScript = new LuaScript(context_);
    EditorLuaBinding::LuaBinding(luaScript->GetState());
    context_->RegisterSubsystem(luaScript);
    luaScript->ExecuteFile("assets/scripts/main.lua");
	scene_ctrl::get_inatance()->create_scene();

	_cam_ctrl = new CameraCtrl(scene_ctrl::get_inatance()->_cameraNode);
}

void editor_app::run_frame()
{
	scene_ctrl::get_inatance()->update();
	_engine->RunFrame();
	if (_gizmoCtrl)
		_gizmoCtrl->update();
}

void editor_app::resize_window(int w, int h)
{
    //auto* graphics = GetSubsystem<Graphics>();
	//graphics->SetMode(w, h);
}

void editor_app::HandleLogMessage(StringHash eventType, VariantMap& eventData)
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

void editor_app::run() 
{
	_start_ui = new start_view();
	_start_ui->show();
}

void editor_app::open_work_space(const std::string& path)
{
	work_space::get_instance()->set_workspace(path);
	std::string title = "urho3d   " + path + "*";
	//render_view* render_v = new render_view(nullptr);
	//render_v->show();

	_main_window = new main_window(nullptr);
	_main_window->setWindowTitle(title.c_str());
	_main_window->showMaximized();
}

editor_app* editor_app::get_instance()
{
	if (_instance == nullptr)
	{

		_instance = new editor_app(new Context());
	}
	return _instance;
}



void editor_app::set_cur_tool(const std::string& name)
{
    if (_gizmoCtrl==NULL && scene_ctrl::get_inatance()->_scene)
	{
		Node* gizmoRoot = scene_ctrl::get_inatance()->_scene->CreateChild("gizmoRoot");
		_gizmoCtrl = new TransformCtrl(context_, TransformCtrl::eMode::eTranslate, gizmoRoot);
		_gizmoCtrl->setScene(scene_ctrl::get_inatance()->_scene);
		_gizmoCtrl->setCameraNode(scene_ctrl::get_inatance()->_cameraNode);
	}
	_curent_tool = name;
	if (name == "move")
	{
		_gizmoCtrl->setMode(TransformCtrl::eMode::eTranslate);
	}
	else if (name == "rotate")
	{
		_gizmoCtrl->setMode(TransformCtrl::eMode::eRotate);
	}
	else if (name == "scale")
	{
		_gizmoCtrl->setMode(TransformCtrl::eMode::eScale);
	}
	else if (name == "camera")
	{
		if(_gizmoCtrl)
			_gizmoCtrl->detach();
	}
}

EditorMenu* editor_app::AddMenu(const String path) 
{ 
	QAction* act = _main_window->GetMenuBar()->add_menu(path.CString());
    EditorMenu* menu = new EditorMenu(_context,act);
    return menu;
}

}
}
