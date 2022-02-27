#pragma once
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
using namespace Urho3D;

class ImguiUpdater : public Object
{
    URHO3D_OBJECT(ImguiUpdater, Object);
public:
    ImguiUpdater(Context* ctx);
    void update();
};

class MenuBarUpdater:public Object
{
    URHO3D_OBJECT(MenuBarUpdater, Object);
public:
    MenuBarUpdater(Context* ctx);
    void update();

};
