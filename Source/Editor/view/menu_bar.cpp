#include <QMessageBox>
#include "menu_bar.h"
#include "ctrl/utils.h"

namespace urho3d
{
namespace editor
{

menu_bar::menu_bar(QMainWindow* p)
{
	_parent = p;
	create_menus();
}
menu_bar::~menu_bar()
{

}

void menu_bar::create_menus()
{
	_menu_bar = _parent->menuBar();
	QAction* act_open = add_menu("files/open");
	connect(act_open,&QAction::triggered,this,&menu_bar::on_open_file);
	QAction* act_save = add_menu("files/save");
	connect(act_save, &QAction::triggered, this, &menu_bar::on_save_proj);
    QAction* act_imgui = add_menu("test/imgui");
    connect(act_imgui, &QAction::triggered, this, &menu_bar::on_test_imgui);
}
QAction* menu_bar::add_menu(const std::string& path)
{
	QAction* result = nullptr;
	if (_action_map.find(path) != _action_map.end())
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("添加菜单失败！"));
		return result;
	}
	std::vector<std::string> strs = Utils::splite_str(path,"/");
	std::string tmp_path = "";
	QMenu* last_menu = nullptr;
    for (size_t i = 0; i < strs.size() - 1; i++)
	{
		tmp_path = tmp_path + strs[i];
		if (_menu_map.find(tmp_path) == _menu_map.end())
		{
			QMenu* menu_tmp = nullptr;
			if (last_menu == nullptr)
				menu_tmp = _menu_bar->addMenu(Utils::str2qstr(strs[i]));
			else
				menu_tmp = last_menu->addMenu(Utils::str2qstr(strs[i]));
			_menu_map[tmp_path] = menu_tmp;
			last_menu = menu_tmp;
		}
		else
		{
			last_menu = _menu_map[tmp_path];
		}
	}
	QString action_name = Utils::str2qstr(strs[strs.size() - 1]);
	result = last_menu->addAction(action_name);
	_action_map[path] = result;

	return result;
}

void menu_bar::on_open_file()
{
	QMessageBox::warning(nullptr, "warning", "on_open");
}

void menu_bar::on_save_proj()
{
	QMessageBox::warning(nullptr, "warning", "on_save"); }

void menu_bar::on_test_imgui() 
{
    if (_imguiW != nullptr)
    {
        _imguiW->show();
        return;
    }
    _imguiW = new ImguiDemo(nullptr);
    _imguiW->resize(1280, 720);
    _imguiW->show();

	/*if (_imguiW1 != nullptr)
    {
        _imguiW1->show();
        return;
    }
    _imguiW1 = new ImguiDemo1(nullptr);
    _imguiW1->resize(1280, 720);
    _imguiW1->show();*/
    
}

}
}
