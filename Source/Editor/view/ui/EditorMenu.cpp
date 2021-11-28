#include "view/ui/EditorMenu.h"

namespace urho3d
{
namespace editor
{

EditorQtAction::EditorQtAction(QAction* act)
{ 
    _action = act;
    connect(_action, &QAction::triggered, this, &EditorQtAction::OnClicked);
}

void EditorQtAction::OnClicked() 
{ 
    _menu->OnClicked(); 
}

EditorMenu::EditorMenu(Context* context, QAction* act)
    : Object(context)
{
    _action = new EditorQtAction(act);
    _action->_menu = this;
}

EditorMenu::~EditorMenu()
{ 
   
}

void EditorMenu::OnClicked()
{ 
    this->SendEvent("clicked");
}

}
}