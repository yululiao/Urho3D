#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <map>
#include "ctrl/editor_delegate.h"

namespace urho3d
{
namespace editor
{
class menu_bar : public QObject
{
	Q_OBJECT;
public:
	menu_bar(QMainWindow* p);
	~menu_bar();
	QAction* add_menu(const std::string& path);
	//void add_menu(const std::string& path, editor_delegate0 handle);
public slots:
	void on_open_file();
	void on_save_proj();
protected:
	void create_menus();
private:
	QMainWindow* _parent = nullptr;
	QMenuBar* _menu_bar = nullptr;
	std::map<std::string, QMenu*> _menu_map;
	std::map<std::string, QAction*> _action_map;
};
}

}

