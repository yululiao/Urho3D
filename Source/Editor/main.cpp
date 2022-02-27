#include <QtWidgets/QApplication>
#include "editor_app.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    urho3d::editor::EditorApp::get_instance()->run();
    return a.exec();
	/*while (true)
	{
		a.processEvents();
	}*/
}
