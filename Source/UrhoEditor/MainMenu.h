#pragma once
#include <map>
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
using namespace Urho3D;

class MenuItem :public Object
{
    URHO3D_OBJECT(MenuItem, Object);
public:
    MenuItem(Context* ctx, const String& path, void* hmenu);
    ~MenuItem();
    void OnTrigger();
    void* GetHMenu() { return menuHandle_; }

private:
    String path_;
    void* menuHandle_ = nullptr;
};

class MainMenu
{
public:
    MainMenu();
    virtual ~MainMenu();
    virtual MenuItem* CreateMenuItem(const String& path) = 0;
protected:
    std::map<String, MenuItem*> menuMap_;
};