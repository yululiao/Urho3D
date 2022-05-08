#pragma once
#include "EditorWidget.h"


namespace Urho3DEditor
{
class SceneView:public EditorWidget
{
public:
    SceneView(const std::string& ptitle) {
        title = ptitle;
        winSize = ImVec2(400, 300);
    }
    virtual ~SceneView();
    void Update() override;
    void genGpuTex();
    void delteGpuTex();
    void onIO();
    void wheelEvent(float deta);
    void mouseMoveEvent(Vector2 pos);
    void mouseReleaseEvent(Vector2 pos);
    void mousePressEvent(Vector2 pos);

protected:
    GLuint rttTexID = 0;
    bool _is_mouse_pressed = false;
    bool _is_mouse_moved = false;
};
}