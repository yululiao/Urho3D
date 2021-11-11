#include <QWidget>
#include <QTimer>


namespace urho3d
{
namespace editor
{
class event_data;
class editor_app;
class render_view :public QWidget
{
	Q_OBJECT
public:
	render_view(QWidget* p);
    ~render_view() override;
	void on_set_cursor(event_data* data);
public slots:
	void on_timer();
protected:
	void showEvent(QShowEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
	QTimer* _timer = nullptr;
	bool _is_init = false;
	bool _is_mouse_pressed = false;
	std::string current_cursor;
	editor_app* _app;
	bool _is_mouse_moved = false;
};
}
}
