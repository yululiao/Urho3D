#include "SceneView.h"
#include "ctrls/SceneCtrl.h"
#include "EditorApp.h"

namespace Urho3DEditor 
{

SceneView::~SceneView() {
    delteGpuTex();
}

void SceneView::wheelEvent(float deta) {
    EditorApp::getInstance()->cam_ctrl_->onWheel(deta * 50);
}

void SceneView::mousePressEvent(Vector2 pos) {
    auto _app = EditorApp::getInstance();
    _is_mouse_pressed = true;
    _is_mouse_moved = false;
    if (_app->_curent_tool == "camera") {
        _app->cam_ctrl_->onPointerDown(pos.x_, pos.y_);
    }
    else {
        _app->gizmoCtrl_->onPointerDown(pos.x_ / winSize.x, pos.y_ / winSize.y);
        if (!_app->gizmoCtrl_->isDraging()) {
            _app->cam_ctrl_->onPointerDown(pos.x_, pos.y_);
        }
    }
}
void SceneView::mouseMoveEvent(Vector2 pos) {
    auto _app = EditorApp::getInstance();
    if (!_is_mouse_pressed) {
        if (_app->_curent_tool != "camera") {
            _app->gizmoCtrl_->onPointerHover(pos.x_ / winSize.x, pos.y_ / winSize.y);
        }
        return;
    }
    _is_mouse_moved = true;
    if (_app->_curent_tool == "camera") {
        _app->cam_ctrl_->onPointerMove(pos.x_, pos.y_);
    }
    else {
        _app->gizmoCtrl_->onPointerMove(pos.x_ / winSize.x, pos.y_ / winSize.y);
        if (!_app->gizmoCtrl_->isDraging()) {
            _app->cam_ctrl_->onPointerMove(pos.x_, pos.y_);
        }
    }
}
void SceneView::mouseReleaseEvent(Vector2 pos) {
    auto _app = EditorApp::getInstance();
    _is_mouse_pressed = false;
    if (_app->_curent_tool == "camera") {
        _app->cam_ctrl_->onPointerUp(pos.x_, pos.y_);
    }
    else {
        if (!_is_mouse_moved) {
            Node* hitNode = SceneCtrl::getInstance()->select(pos.x_ / winSize.x, pos.y_ / winSize.y);
            if (hitNode) {
                _app->gizmoCtrl_->attach(hitNode);
            }
            else {
                _app->gizmoCtrl_->detach();
            }
        }

        if (!_app->gizmoCtrl_->isDraging()) {
            _app->cam_ctrl_->onPointerUp(pos.x_, pos.y_);
        }
        _app->gizmoCtrl_->onPointerUp(pos.x_ / winSize.x, pos.y_ / winSize.y);
    }
}


void SceneView::onIO() {
    ImGuiIO& io = ImGui::GetIO();
    Vector2 mousePos(io.MousePos.x, io.MousePos.y);
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 offset = ImGui::GetWindowContentRegionMin();
    winPos.x = winPos.x + offset.x;
    winPos.y = winPos.y + offset.y;
    mousePos = Vector2(mousePos.x_ - winPos.x, mousePos.y_ - winPos.y);
    if (!ImGui::IsWindowFocused()) {
        return;
    }
    if (io.MouseWheel != 0.0) {
        wheelEvent(io.MouseWheel);
    }

    if ((io.MouseDown[0] && io.MouseDown[0] != mousePresed) || (io.MouseDown[1] && io.MouseDown[1] != mousePresed)) {
        mousePressEvent(mousePos);
    }
    mousePresed = io.MouseDown[0] || io.MouseDown[1];
    if (-mousePos.x_ != FLT_MAX) {
        if (curMousePos != mousePos) {
            mouseMoveEvent(mousePos);
        }
    }
    curMousePos = mousePos;

    if ((io.MouseReleased[0] && (io.MouseReleased[0] != mouseRelease)) ||
        (io.MouseReleased[1] && (io.MouseReleased[1] != mouseRelease))) {
        mouseReleaseEvent(mousePos);
    }
    mouseRelease = io.MouseReleased[0] || io.MouseReleased[1];
}

void SceneView::Update() {
    if (showing) {

        //ImGui::SetNextWindowSize(ImVec2(416,335));
        ImGui::Begin(title.c_str(), &showing);
        ///ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        //winSize = ImGui::GetWindowSize();
        onIO();
        ImVec2 newSize = ImGui::GetWindowSize();
        newSize.x = newSize.x - 14;
        newSize.y = newSize.y - 37;
        if (newSize.x != winSize.x || newSize.y != winSize.y) {
            winSize = newSize;
            SceneCtrl::getInstance()->OnResizeView(winSize.x, winSize.y);
        }
        genGpuTex();
        ImGui::Image((ImTextureID)(intptr_t)rttTexID, ImVec2(winSize.x, winSize.y));
        if (ImGui::BeginDragDropTarget()) {
            //void* data = ImGui::AcceptDragDropPayload("drag_res")->Data;
            if (ImGui::IsMouseReleased(0)) {
                auto data = ImGui::AcceptDragDropPayload("drag_res");
                if (data) {
                    char path[200] = { 0 };
                    memcpy(path, data->Data, data->DataSize);
                    //std::string strpath = path;
                    SceneCtrl::getInstance()->addModel(path);
                    std::cout << "onDrop" << std::endl;
                }

            }

            ImGui::EndDragDropTarget();
        }
        //ImGui::Image((ImTextureID)(intptr_t)rttTexID, ImVec2(winSize.x, winSize.y));
        ImGui::End();
    }
}
void SceneView::genGpuTex() {
    if (!rttTexID) {
        glGenTextures(1, &rttTexID);
    }
    glBindTexture(GL_TEXTURE_2D, rttTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Vector2 rttSize = SceneCtrl::getInstance()->constRttSize;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rttSize.x_, rttSize.y_, 0, GL_RGB, GL_UNSIGNED_BYTE,
        SceneCtrl::getInstance()->getRttData());
    //glGenerateMipmap(GL_TEXTURE_2D);

}

void SceneView::delteGpuTex() {
    if (rttTexID) {
        glDeleteTextures(1, &rttTexID);
        rttTexID = 0;
    }
}



}