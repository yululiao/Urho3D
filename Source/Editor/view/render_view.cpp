#include <QResizeEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QFileInfo>
#include "QMimeData"
#include "render_view.h"
#include "editor_app.h"
#include "ctrl/global_event.h"
#include "ctrl/gizmo/TransformCtrl.h"
#include "ctrl/scene_ctrl.h"


namespace urho3d
{
namespace editor
{
render_view::render_view(QWidget* p)
	:QWidget(p)
{
	setMouseTracking(true);
	_timer = new QTimer();
	current_cursor = "Normal";
    this->setAcceptDrops(true);
	_app = EditorApp::get_instance();
	global_event::get_instance()->register_event(eGlobalEventType::SetCursor, MakeDelegate(this, &render_view::on_set_cursor));
}
render_view::~render_view()
{
	global_event::get_instance()->remove_event(eGlobalEventType::SetCursor, MakeDelegate(this, &render_view::on_set_cursor));
}

void render_view::on_set_cursor(event_data* data)
{
	current_cursor = data->stringList[0];
	if (current_cursor == "Normal")
	{
		setCursor(Qt::ArrowCursor);
	}
	else if (current_cursor == "SizeAllCursor")
	{
		setCursor(Qt::SizeAllCursor);
	}
	else if (current_cursor == "ClosedHandCursor")
	{
		setCursor(Qt::ClosedHandCursor);
	}
}

void render_view::on_timer()
{
	if (_is_init)
	{
		_app->run_frame();
	}
}

void render_view::showEvent(QShowEvent *event)
{
	if (!_is_init)
	{
		QWidget::showEvent(event);
        _app->create_engine((void*)this->winId());
		_timer->start(10);
		connect(_timer, SIGNAL(timeout()), this, SLOT(on_timer()));
		_is_init = true;
	}
	
}

void render_view::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	if (_is_init)
	{
		//_app->resize_window(event->size().width(), event->size().height());
	}
}

void render_view::mousePressEvent(QMouseEvent *event)
{
	_is_mouse_pressed = true;
	_is_mouse_moved = false;
	if (_app->_curent_tool == "camera")
	{
		_app->_cam_ctrl->onPointerDown(event->x(), event->y());
	}
	else
	{
		
		_app->_gizmoCtrl->onPointerDown(event->x() / (float)this->width(), event->y() / (float)this->height());
		if (!_app->_gizmoCtrl->isDraging())
		{
			_app->_cam_ctrl->onPointerDown(event->x(), event->y());
		}
	}
	
	
}

void render_view::mouseReleaseEvent(QMouseEvent *event)
{
	_is_mouse_pressed = false;
	if (_app->_curent_tool == "camera")
	{
		_app->_cam_ctrl->onPointerUp(event->x(), event->y());
	}
	else
	{
		if (!_is_mouse_moved)
		{
			Node* hitNode = SceneCtrl::get_inatance()->select(event->x() / (float)this->width(), event->y() / (float)this->height());
			if (hitNode)
			{
				_app->_gizmoCtrl->attach(hitNode);
			}
			else
			{
				_app->_gizmoCtrl->detach();
			}
		}

		if (!_app->_gizmoCtrl->isDraging())
		{
			_app->_cam_ctrl->onPointerUp(event->x(), event->y());
		}
		_app->_gizmoCtrl->onPointerUp(event->x() / (float)this->width(), event->y() / (float)this->height());
		
	}	
}

void render_view::mouseMoveEvent(QMouseEvent *event)
{
	if (!_is_mouse_pressed)
	{
		if (_app->_curent_tool != "camera")
		{
			_app->_gizmoCtrl->onPointerHover(event->x() / (float)this->width(), event->y() / (float)this->height());
		}
		return;
	}
	_is_mouse_moved = true;
	if (_app->_curent_tool == "camera")
	{
		_app->_cam_ctrl->onPointerMove(event->x(), event->y());
	}
	else
	{
		_app->_gizmoCtrl->onPointerMove(event->x() / (float)this->width(), event->y() / (float)this->height());
		if (!_app->_gizmoCtrl->isDraging())
		{
			_app->_cam_ctrl->onPointerMove(event->x(), event->y());
		}
	}
	
}

void render_view::wheelEvent(QWheelEvent * event)
{
	_app->_cam_ctrl->onWheel(event->delta());
}

void render_view::dragEnterEvent(QDragEnterEvent *event)
{
    auto mimeData = event->mimeData();
    if (!mimeData->hasFormat("DragAssetToScene"))//名字和res_tree里的一致
    {
        return;
    }
    QString path = QString::fromLocal8Bit(mimeData->data("DragAssetToScene"));
     QFileInfo info(path);
     QString ext_str = info.suffix().toLower();
     QString extFilter = "prefab.ptc.fbx";//可以将这些文件拖到场景里
     if(extFilter.split(".").contains(ext_str))
     {
        //scene_ctrl::get_inatance()->addModel(path.toStdString());
         event->acceptProposedAction();
     }

}

void render_view::dropEvent(QDropEvent *event)
{
    auto mimeData = event->mimeData();
    if (!mimeData->hasFormat("DragAssetToScene"))//名字和res_tree里的一致
    {
        return;
    }
    QString path = QString::fromLocal8Bit(mimeData->data("DragAssetToScene"));
     QFileInfo info(path);
     QString ext_str = info.suffix().toLower();
     QString extFilter = "prefab.ptc.fbx";//可以将这些文件拖到场景里
     if(ext_str == "fbx" && !(info.baseName().contains("@")))
     {
        SceneCtrl::get_inatance()->addModel(path.toStdString());
     }
}


}
}
