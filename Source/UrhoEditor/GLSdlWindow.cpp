#include"GLEW/glew.h"
#include "GLSDLWindow.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "SceneCtrl.h"
#include "GLFW/glfw3native.h"
#include "EditorApp.h"

UMainWindow::UMainWindow(int width, int height) : width{ width }, height{ height }
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simfang.ttf", 15.0f,nullptr,io.Fonts->GetGlyphRangesChineseFull());
    //simfang.ttf
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    window = glfwCreateWindow(width, height, "Urho3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
#ifdef GLFW_EXPOSE_NATIVE_WIN32
    HWND hwnd = glfwGetWin32Window(window);
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    //SetWindowLong(hwnd, GWL_STYLE, style ^ WS_CAPTION);
#endif // GLFW_EXPOSE_NATIVE_WIN32
    //menuBar = new WinMainMenu(hwnd);
    glfwMakeContextCurrent(window);
    //wglShareLists(g_HGLRC, data->hGLRC);
    //glfwMaximizeWindow(window);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 410";
    ImGui_ImplOpenGL3_Init(glsl_version);
    glfwSetWindowSize(window,800,600);
    glfwSetWindowPos(window,300,200);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
UMainWindow::~UMainWindow() 
{
    ImGui_ImplOpenGL3_Shutdown();
    glfwTerminate();
    ImGui::DestroyContext();
}

void UMainWindow::Draw() const
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}
bool UMainWindow::WindowShouldClose()const
{
	return glfwWindowShouldClose(window);
}
void UMainWindow::Resize(int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* UMainWindow::getRawWindow() 
{
	return window;
}

void UMainWindow::Update()
{
    glfwMakeContextCurrent(glfwGetCurrentContext());
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    imguiUpdate();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    Draw();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void UMainWindow::imguiUpdate()
{
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    bool show = true;
    //showDockSpace(&show);
    bool showMian = true;
    if(_showDemo)
    {
        ImGui::ShowDemoWindow(&_showDemo);
    }
    for (auto i = 0; i < windows.size(); i++)
    {
        windows[i]->update();
    }
    _imguiUpdater->update();
    // ImGui::End();
    ImGui::Render();
}

void UMainWindow::maxSize() 
{ 
    glfwMaximizeWindow(window);
}

renderWindow::~renderWindow() 
{ 
    delteGpuTex() ;
}

void renderWindow::wheelEvent(float deta) 
{
    EditorApp::getInstance()->cam_ctrl_->onWheel(deta * 50);
}

void renderWindow::mousePressEvent(Vector2 pos) 
{
    auto _app = EditorApp::getInstance();
    _is_mouse_pressed = true;
    _is_mouse_moved = false;
    if (_app->_curent_tool == "camera")
    {
        _app->cam_ctrl_->onPointerDown(pos.x_, pos.y_);
    }
    else
    {
        _app->gizmoCtrl_->onPointerDown(pos.x_ / winSize.x, pos.y_ / winSize.y);
        if (!_app->gizmoCtrl_->isDraging())
        {
            _app->cam_ctrl_->onPointerDown(pos.x_, pos.y_);
        }
    }
}
void renderWindow::mouseMoveEvent(Vector2 pos) 
{
    auto _app = EditorApp::getInstance();
    if (!_is_mouse_pressed)
    {
        if (_app->_curent_tool != "camera")
        {
            _app->gizmoCtrl_->onPointerHover(pos.x_ / winSize.x, pos.y_ / winSize.y);
        }
        return;
    }
    _is_mouse_moved = true;
    if (_app->_curent_tool == "camera")
    {
        _app->cam_ctrl_->onPointerMove(pos.x_, pos.y_);
    }
    else
    {
    _app->gizmoCtrl_->onPointerMove(pos.x_ / winSize.x, pos.y_ / winSize.y);
    if (!_app->gizmoCtrl_->isDraging())
    {
        _app->cam_ctrl_->onPointerMove(pos.x_, pos.y_);
    }
    }
}
void renderWindow::mouseReleaseEvent(Vector2 pos)
{
    auto _app = EditorApp::getInstance();
    _is_mouse_pressed = false;
    if (_app->_curent_tool == "camera")
    {
        _app->cam_ctrl_->onPointerUp(pos.x_, pos.y_);
    }
    else
    {
        if (!_is_mouse_moved)
        {
            Node* hitNode = SceneCtrl::getInstance()->select(pos.x_ / winSize.x, pos.y_ / winSize.y);
            if (hitNode)
            {
                _app->gizmoCtrl_->attach(hitNode);
            }
            else
            {
                _app->gizmoCtrl_->detach();
            }
        }

        if (!_app->gizmoCtrl_->isDraging())
        {
            _app->cam_ctrl_->onPointerUp(pos.x_, pos.y_);
        }
        _app->gizmoCtrl_->onPointerUp(pos.x_ / winSize.x, pos.y_ / winSize.y);
    }
}


void renderWindow::onIO()
{
    ImGuiIO& io = ImGui::GetIO();
    Vector2 mousePos(io.MousePos.x, io.MousePos.y);
    ImVec2 winPos = ImGui::GetWindowPos();
    ImVec2 offset = ImGui::GetWindowContentRegionMin();
    winPos.x = winPos.x + offset.x;
    winPos.y = winPos.y + offset.y;
    mousePos = Vector2(mousePos.x_ - winPos.x,mousePos.y_ - winPos.y);
    if (!ImGui::IsWindowFocused())
    {
        return;
    }
    if (io.MouseWheel != 0.0)
    {
        wheelEvent(io.MouseWheel);
    }

    if ((io.MouseDown[0] && io.MouseDown[0] != mousePresed) || (io.MouseDown[1] && io.MouseDown[1] != mousePresed))
    {
        mousePressEvent(mousePos);
    }
    mousePresed = io.MouseDown[0] || io.MouseDown[1];
    if (-mousePos.x_ != FLT_MAX)
    {
        if (curMousePos != mousePos)
        {
            mouseMoveEvent(mousePos);
        }
    }
    curMousePos = mousePos;

    if ((io.MouseReleased[0] && (io.MouseReleased[0] != mouseRelease)) ||
        (io.MouseReleased[1] && (io.MouseReleased[1] != mouseRelease)))
    {
        mouseReleaseEvent(mousePos);
    }
    mouseRelease = io.MouseReleased[0] || io.MouseReleased[1];
}

void renderWindow::update()
{
    if (showing)
    {

        //ImGui::SetNextWindowSize(ImVec2(416,335));
        ImGui::Begin(title.c_str(), &showing);
        ///ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        //winSize = ImGui::GetWindowSize();
        onIO();
        ImVec2 newSize = ImGui::GetWindowSize();
        newSize.x = newSize.x - 14;
        newSize.y = newSize.y - 37;
        if (newSize.x != winSize.x || newSize.y != winSize.y)
        {
            winSize = newSize;
            SceneCtrl::getInstance()->OnResizeView(winSize.x, winSize.y);
        }
        genGpuTex();
        ImGui::Image((ImTextureID)(intptr_t)rttTexID, ImVec2(winSize.x, winSize.y));
        if (ImGui::BeginDragDropTarget())
        {
            //void* data = ImGui::AcceptDragDropPayload("drag_res")->Data;
            if (ImGui::IsMouseReleased(0))
            {
                auto data = ImGui::AcceptDragDropPayload("drag_res");
                if(data)
                {
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
void renderWindow::genGpuTex()
{
    if (!rttTexID)
    {
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

void renderWindow::delteGpuTex()
{
    if (rttTexID)
    {
        glDeleteTextures(1, &rttTexID);
        rttTexID = 0;
    }
}


void DockerContainer::update()
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("###DockSpace", &showing, window_flags);
    ImGui::PopStyleVar();
    _menuBarUpdater->update();
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}

