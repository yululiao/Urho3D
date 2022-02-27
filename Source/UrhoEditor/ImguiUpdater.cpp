#include "ImguiUpdater.h"

ImguiUpdater::ImguiUpdater(Context* ctx)
    : Object(ctx)
{

}

void ImguiUpdater::update() 
{ 
    this->SendEvent("imguiUpdate"); 
}

MenuBarUpdater::MenuBarUpdater(Context* ctx)
    : Object(ctx)
{
}

void MenuBarUpdater::update() 
{ 
    this->SendEvent("menuBarUpdate");
}
