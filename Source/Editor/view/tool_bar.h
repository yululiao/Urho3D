#include<QToolBar>
#include <QMainWindow>
#include <QAction>

namespace urho3d
{
namespace editor
{
class EditorApp;
class tool_bar : public QObject
{
	Q_OBJECT;
public:
	tool_bar(QMainWindow* p);
	~tool_bar();
public slots:
	void on_save();
	void on_redo();
	void on_undo();
	void on_grab();
	void on_move();
	void on_rotate();
	void on_scale();
protected:
	void create_toolbar();
	void hilight_tool(QAction* tool);
private:
	EditorApp* _app;
	QMainWindow* _main_window = nullptr;
	QToolBar* _toolbar = nullptr;
	QAction* _action_save = nullptr;
	QAction* _action_undo = nullptr;
	QAction* _action_redo = nullptr;

	QAction* _action_grab = nullptr;
	QAction* _action_move = nullptr;
	QAction* _action_rotate = nullptr;
	QAction* _action_scale = nullptr;

	std::vector<QAction*> _all_tools;
};
}
}


