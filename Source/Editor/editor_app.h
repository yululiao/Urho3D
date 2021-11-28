#pragma once

#include <string>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "ctrl/camera_ctrl.h"
#include "view/ui/EditorMenu.h"

using namespace Urho3D;

namespace urho3d
{
namespace editor
{
class start_view;
class main_window;
class TransformCtrl;
class editor_app :public Object
{
	URHO3D_OBJECT(editor_app, Object);
public:
	editor_app(Context* context);
	~editor_app();
	void run();
	void open_work_space(const std::string& path);
	Context* get_context() { return _context; }
    static editor_app* get_instance();
	void create_engine(void* win_ptr);
	void run_frame();
	void resize_window(int w,int h);
	void set_cur_tool(const std::string& name);
    EditorMenu* AddMenu(const String path);

protected:
	void HandleLogMessage(StringHash eventType, VariantMap& eventData);
	Context* _context = nullptr;

	void setup();
	void start();
private:
    static editor_app* _instance;
	start_view* _start_ui = nullptr;
	main_window* _main_window = nullptr;
	/// Urho3D engine.
	SharedPtr<Engine> _engine;
	/// Engine parameters map.
	VariantMap _engineParameters;
	String _startupErrors;
	void* _window_ptr = nullptr;
public:
	CameraCtrl* _cam_ctrl = nullptr;
   // SharedPtr<TransformCtrl> _gizmoCtrl = nullptr;
	TransformCtrl* _gizmoCtrl = nullptr;
	std::string _curent_tool;
};

}
}
