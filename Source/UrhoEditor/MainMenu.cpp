#include "MainMenu.h"

MenuItem::MenuItem(Context* ctx, const String& path, void* hmenu) 
	: Object(ctx)
{
    path_ = path;
    menuHandle_ = hmenu;
}

MenuItem::~MenuItem() {}

void MenuItem::OnTrigger() 
{ 
    this->SendEvent("tiggered"); 
}

MainMenu::MainMenu() 
{
}

MainMenu::~MainMenu() 
{
}
