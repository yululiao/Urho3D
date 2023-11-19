#pragma once
#include <string>
#include <iostream>
#include "imgui.h"
#include "Urho3D/Core/Context.h"
#include "Urho3D/Core/Main.h"
#include "Urho3D/Engine/Engine.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"

using namespace Urho3D;

namespace Urho3DEditor
{
class EditorWidget {
public:
    virtual ~EditorWidget() {};
    virtual void Update() = 0;
    void Show() { showing = true; }
    void Hide() { showing = false; }
    bool IsInWindow(ImVec2 pos);
protected:
    bool showing = true;
    std::string title;
    ImVec2 winSize;
    ImVec2 _winPos;
    Vector2 curMousePos;
    bool mousePresed = false;
    bool mouseRelease = true;
};

}

