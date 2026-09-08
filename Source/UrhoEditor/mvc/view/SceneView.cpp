#include "SceneView.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "ctrl/res/AssetMgr.h"

namespace Urho3DEditor 
{

SceneView::~SceneView() {
    DelteGpuTex();
}

void SceneView::WheelEvent(float deta) {
    cameraController_.onWheel(deta * 50);
}

void SceneView::MousePressEvent(Vector2 pos) {
    _is_mouse_pressed = true;
    _is_mouse_moved = false;
    if (toolController_.GetCurrentTool() == "camera") {
        cameraController_.onPointerDown(pos.x_, pos.y_);
    }
    else {
        gizmoController_.onPointerDown(pos.x_ / winSize.x, pos.y_ / winSize.y);
        if (!gizmoController_.isDraging()) {
            cameraController_.onPointerDown(pos.x_, pos.y_);
        }
    }
}

void SceneView::MouseHoverEvent(Vector2 pos) {
    if (!_is_mouse_pressed) {
        if (toolController_.GetCurrentTool() != "camera") {
            gizmoController_.onPointerHover(pos.x_ / winSize.x, pos.y_ / winSize.y);
        }
    }
}

void SceneView::MouseMoveEvent(Vector2 pos) {
    if (!_is_mouse_pressed) {
        return;
    }
    _is_mouse_moved = true;
    if (toolController_.GetCurrentTool() == "camera") {
        cameraController_.onPointerMove(pos.x_, pos.y_);
    }
    else {
        gizmoController_.onPointerMove(pos.x_ / winSize.x, pos.y_ / winSize.y);
        if (!gizmoController_.isDraging()) {
            cameraController_.onPointerMove(pos.x_, pos.y_);
        }
    }
}
void SceneView::MouseReleaseEvent(Vector2 pos) {
    _is_mouse_pressed = false;
    if (toolController_.GetCurrentTool() == "camera") {
        cameraController_.onPointerUp(pos.x_, pos.y_);
    }
    else {
        if (!_is_mouse_moved) {
            selectionController_.OnSceneClicked(Vector2(pos.x_ / winSize.x, pos.y_ / winSize.y));
        }

        if (!gizmoController_.isDraging()) {
            cameraController_.onPointerUp(pos.x_, pos.y_);
        }
        gizmoController_.onPointerUp(pos.x_ / winSize.x, pos.y_ / winSize.y);
    }
}


void SceneView::OnIO() {
    ImGuiIO& io = ImGui::GetIO();
    Vector2 mousePos(io.MousePos.x, io.MousePos.y);
    _winPos = ImGui::GetWindowPos();
    ImVec2 offset = ImGui::GetWindowContentRegionMin();
    ImVec2 contentOriPos;
    contentOriPos.x = _winPos.x + offset.x;
    contentOriPos.y = _winPos.y + offset.y;
    mousePos = Vector2(mousePos.x_ - contentOriPos.x, mousePos.y_ - contentOriPos.y);
    ImVec2 rightBottom(contentOriPos.x + winSize.x, contentOriPos.y + winSize.y);
    ImRect contentRec(contentOriPos, rightBottom);
    if(!contentRec.Contains(io.MousePos))
    {
        return;
    }
    if (io.MouseWheel != 0.0) {
        WheelEvent(io.MouseWheel);
    }
    if (-mousePos.x_ != FLT_MAX) {
        if (curMousePos != mousePos) {
            MouseHoverEvent(mousePos);
        }
    }
    if(ImGui::IsWindowFocused()){
        if ((io.MouseDown[0] && io.MouseDown[0] != mousePresed) || (io.MouseDown[1] && io.MouseDown[1] != mousePresed)) {
            MousePressEvent(mousePos);
        }
        mousePresed = io.MouseDown[0] || io.MouseDown[1];
        if (-mousePos.x_ != FLT_MAX) {
            if (curMousePos != mousePos) {
                MouseMoveEvent(mousePos);
            }
        }
    }
    curMousePos = mousePos;
    if (ImGui::IsWindowFocused()) {
        if ((io.MouseReleased[0] && (io.MouseReleased[0] != mouseRelease)) ||
            (io.MouseReleased[1] && (io.MouseReleased[1] != mouseRelease))) {
            MouseReleaseEvent(mousePos);
        }
        mouseRelease = io.MouseReleased[0] || io.MouseReleased[1];
    }
}

void SceneView::Update() {
    if (showing) {
        //ImGui::SetNextWindowSize(ImVec2(416,335));
        ImGui::Begin(title.c_str(), &showing);
        ///ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        //winSize = ImGui::GetWindowSize();
        OnIO();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 newSize(contentMax.x - contentMin.x, contentMax.y - contentMin.y);
        if (newSize.x != winSize.x || newSize.y != winSize.y) {
            winSize = newSize;
            sceneCtrl_.OnResizeView(winSize.x, winSize.y);
        }
        GenGpuTex();
        ImGui::Image((ImTextureID)(intptr_t)rttTexID, ImVec2(winSize.x, winSize.y));
        if (ImGui::BeginDragDropTarget()) {
            if (ImGui::IsMouseReleased(0)) {
                auto data = ImGui::AcceptDragDropPayload("drag_file");
                if (data) {
                    String path;
                    path.Resize(data->DataSize);
                    memcpy((void*)path.CString(), data->Data, data->DataSize);
                    sceneManipController_.ImportModel(path, nullptr, 0);
                    std::cout << "onDrop:drag_file" << std::endl;
                }

            }

            ImGui::EndDragDropTarget();
        }
        ImVec2 fpsPos(_winPos.x + 15, _winPos.y + winSize.y - 15);
        ImGui::SetCursorScreenPos(fpsPos);
        ImGui::Text("Fps:%d", fpsProvider_ ? fpsProvider_() : 0);
        //ImGui::Image((ImTextureID)(intptr_t)rttTexID, ImVec2(winSize.x, winSize.y));
        ImGui::End();
    }
}
void SceneView::GenGpuTex() {
    if (!rttTexID) {
        glGenTextures(1, &rttTexID);
    }
    glBindTexture(GL_TEXTURE_2D, rttTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Vector2 rttSize = sceneCtrl_.constRttSize;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rttSize.x_, rttSize.y_, 0, GL_RGB, GL_UNSIGNED_BYTE,
        sceneCtrl_.GetRttData());
    //glGenerateMipmap(GL_TEXTURE_2D);

}

void SceneView::DelteGpuTex() {
    if (rttTexID) {
        glDeleteTextures(1, &rttTexID);
        rttTexID = 0;
    }
}



}