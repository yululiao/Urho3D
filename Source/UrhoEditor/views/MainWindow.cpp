#include "MainWindow.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3native.h"
#include "EditorApp.h"

namespace Urho3DEditor
{
MainWindow::MainWindow(int width, int height) : width{ width }, height{ height }
    ,_startView(new StartView),_menuBar(new Menubar()),_toolBar(new Toolbar())
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\simfang.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    //simfang.ttf
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    window = glfwCreateWindow(width, height, "Urho3D", NULL, NULL);
    if (window == NULL) {
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
    glfwSetWindowSize(window, 800, 600);
    glfwSetWindowPos(window, 300, 200);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
MainWindow::~MainWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    glfwTerminate();
    ImGui::DestroyContext();
}

void MainWindow::Draw() const {
    glfwSwapBuffers(window);
    glfwPollEvents();
}
bool MainWindow::WindowShouldClose()const {
    return glfwWindowShouldClose(window);
}
void MainWindow::Resize(int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* MainWindow::GetRawWindow() {
    return window;
}

void MainWindow::StartGame() 
{
    _nodeTree = new NodeTree();
   AddWindow(std::unique_ptr<NodeTree>(_nodeTree));
   _resTree = new ResTree();
   AddWindow(std::unique_ptr<ResTree>(_resTree));
   _inspector = new Inspector();
   AddWindow(std::unique_ptr<Inspector>(_inspector));
   _cosoleView = new ConsoleView();
   AddWindow(std::unique_ptr<ConsoleView>(_cosoleView));
   _resPreview = new ResPreview();
   AddWindow(std::unique_ptr<ResPreview>(_resPreview));
}

void MainWindow::UpdateDockerSpace()
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
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
    ImGui::Begin("###DockSpace", &_showingDocker, window_flags);
    ImGui::PopStyleVar();
    //_menuBarUpdater->update();//todo
    if (EditorApp::GetInstance()->_isStartView)
    {
        _startView->Update();
    }
    else 
    {
        _menuBar->Update();
        _toolBar->Update();
    }
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}

void MainWindow::Update() 
{
    glfwMakeContextCurrent(glfwGetCurrentContext());
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImguiUpdate();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    Draw();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void MainWindow::ImguiUpdate() {
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    bool show = true;
    //showDockSpace(&show);
    UpdateDockerSpace();
    for (auto i = 0; i < windows.size(); i++)
    {
        windows[i]->Update();
    }
    if (_showDemo) {
        ImGui::ShowDemoWindow(&_showDemo);
    }
    //_imguiUpdater->update();
    // ImGui::End();
    ImGui::Render();
}

void MainWindow::MaxSize() {
    glfwMaximizeWindow(window);
}


}