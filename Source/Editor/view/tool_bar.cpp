#include "tool_bar.h"
#include "editor_app.h"

namespace urho3d
{
namespace editor
{
tool_bar::tool_bar(QMainWindow* p)
{
	_main_window = p;
	_app = EditorApp::get_instance();
	create_toolbar();
}
tool_bar::~tool_bar()
{

}

void tool_bar::create_toolbar()
{
	_toolbar = _main_window->addToolBar("toolbar");

	_action_save = new QAction();
	_action_save->setIcon(QIcon(":/Urho3dEditor/images/save.png"));
	connect(_action_save, &QAction::triggered, this, &tool_bar::on_save);
	_action_save->setToolTip(QStringLiteral("保存"));
	_toolbar->addAction(_action_save);

	_action_undo = new QAction();
	_action_undo->setIcon(QIcon(":/Urho3dEditor/images/undo.png"));
	connect(_action_undo, &QAction::triggered, this, &tool_bar::on_undo);
	_action_undo->setToolTip(QStringLiteral("撤销"));
	_toolbar->addAction(_action_undo);

	_action_redo = new QAction();
	_action_redo->setIcon(QIcon(":/Urho3dEditor/images/redo.png"));
	connect(_action_redo, &QAction::triggered, this, &tool_bar::on_redo);
	_action_redo->setToolTip(QStringLiteral("重做"));
	_toolbar->addAction(_action_redo);

	_toolbar->addSeparator();

	_action_grab = new QAction();
	_action_grab->setIcon(QIcon(":/Urho3dEditor/images/hand.png"));
	connect(_action_grab, &QAction::triggered, this, &tool_bar::on_grab);
	_action_grab->setToolTip(QStringLiteral("抓取"));
	_toolbar->addAction(_action_grab);

	_action_move = new QAction();
	_action_move->setIcon(QIcon(":/Urho3dEditor/images/move.png"));
	connect(_action_move, &QAction::triggered, this, &tool_bar::on_move);
	_action_move->setToolTip(QStringLiteral("移动"));
	_toolbar->addAction(_action_move);

	_action_rotate = new QAction();
	_action_rotate->setIcon(QIcon(":/Urho3dEditor/images/rotate.png"));
	connect(_action_rotate, &QAction::triggered, this, &tool_bar::on_rotate);
	_action_rotate->setToolTip(QStringLiteral("旋转"));
	_toolbar->addAction(_action_rotate);

	_action_scale = new QAction();
	_action_scale->setIcon(QIcon(":/Urho3dEditor/images/scale.png"));
	connect(_action_scale, &QAction::triggered, this, &tool_bar::on_scale);
	_action_scale->setToolTip(QStringLiteral("平移"));
	_toolbar->addAction(_action_scale);

	_all_tools.push_back(_action_grab);
	_all_tools.push_back(_action_move);
	_all_tools.push_back(_action_rotate);
	_all_tools.push_back(_action_scale);
	on_grab();
}

void tool_bar::hilight_tool(QAction* tool)
{
	for (auto item : _all_tools)
	{
		item->setCheckable(true);
		if (tool == item)
		{
			item->setChecked(true);
		}
		else
		{
			item->setChecked(false);
		}
	}
}

void tool_bar::on_save()
{

}

void tool_bar::on_redo()
{

}

void tool_bar::on_undo()
{

}

void tool_bar::on_grab()
{
	hilight_tool(_action_grab);
	_app->set_cur_tool("camera");
}

void tool_bar::on_move()
{
	hilight_tool(_action_move);
	_app->set_cur_tool("move");
}

void tool_bar::on_rotate()
{
	hilight_tool(_action_rotate);
	_app->set_cur_tool("rotate");
}

void tool_bar::on_scale()
{
	hilight_tool(_action_scale);
	_app->set_cur_tool("scale");
}


}
}