#pragma once
#include <QAction>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"


using namespace Urho3D;

namespace urho3d
{
namespace editor
{

class EditorMenu;
class EditorQtAction : public QObject
{
public:
    EditorQtAction(QAction* act);
    EditorMenu* _menu = nullptr;
public slots:
    void OnClicked();
private:
    QAction* _action = nullptr;
};
class EditorMenu:public Object
{
    URHO3D_OBJECT(EditorMenu, Object);
public:
    EditorMenu(Context* context,QAction* act);
    ~EditorMenu();
    void OnClicked();
private:
    EditorQtAction* _action = nullptr;

};

}
}