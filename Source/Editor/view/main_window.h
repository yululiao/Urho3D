#include <QMainWindow>
#include <QDockWidget>
#include "node_tree.h"
#include "inspector.h"
#include "menu_bar.h"
#include "render_view.h"
#include "res_tree.h"
#include "tool_bar.h"


namespace urho3d
{
namespace editor
{
class main_window : public QMainWindow
{
public:
	main_window(QWidget* p);
	~main_window();
	void create_views();
protected:
	virtual void showEvent(QShowEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
private:
	menu_bar* _menu_bar = nullptr;
	tool_bar* _tool_bar = nullptr;

	node_tree* _node_tree = nullptr;
	res_tree* _res_tree = nullptr;
	inspector* _insepector= nullptr;
	render_view* _render_view = nullptr;

	QDockWidget* _dock_node_tree = nullptr;
	QDockWidget* _dock_res_tree = nullptr;
	QDockWidget* _dock_inspector = nullptr;
	QDockWidget* _dock_render  = nullptr;

	bool _is_init = false;

};
}
}