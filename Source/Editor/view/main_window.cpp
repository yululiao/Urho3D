#include "main_window.h"
#include <QDesktopWidget>
#include <QApplication>

namespace urho3d
{
namespace editor
{
main_window::main_window(QWidget* parent)
	:QMainWindow(parent)
{
	create_views();
}
main_window::~main_window()
{

}

void main_window::create_views()
{
	setDockNestingEnabled(true);
	_menu_bar = new menu_bar(this);
	_tool_bar = new tool_bar(this);
	_render_view = new  render_view(this);
	_node_tree = new node_tree(this);
	_insepector = new inspector(this);
	_res_tree = new res_tree(this);

	_dock_render = new QDockWidget();
	_dock_node_tree = new QDockWidget();
	_dock_inspector = new QDockWidget();
	_dock_res_tree = new QDockWidget();

	_dock_render->setWidget(_render_view);
	_dock_node_tree->setWidget(_node_tree);
	_dock_inspector->setWidget(_insepector);
	_dock_res_tree->setWidget(_res_tree);

	this->addDockWidget(Qt::LeftDockWidgetArea, _dock_node_tree);
	splitDockWidget(_dock_node_tree, _dock_render, Qt::Horizontal);
	splitDockWidget(_dock_render, _dock_inspector, Qt::Horizontal);
	splitDockWidget(_dock_node_tree,_dock_res_tree,Qt::Vertical);
}

void main_window::showEvent(QShowEvent *event)
{
	QMainWindow::showEvent(event);

	if (!_is_init)
	{
		QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect clientRect = desktopWidget->availableGeometry();
        _dock_node_tree->setMaximumWidth(static_cast<int>(clientRect.width()* 0.18));
        _dock_res_tree->setMaximumWidth(static_cast<int>(clientRect.width()* 0.18));
        _dock_inspector->setMaximumWidth(static_cast<int>(clientRect.width()* 0.18));
		_is_init = true;
	}
	}
	

void main_window::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);

	QSize curentSize = this->size();
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect clientRect = desktopWidget->availableGeometry();
	if (curentSize.width() == clientRect.width())
	{
		_dock_node_tree->setMaximumWidth(clientRect.width());
		_dock_res_tree->setMaximumWidth(clientRect.width());
		_dock_inspector->setMaximumWidth(clientRect.width());
	}
}

}
}
