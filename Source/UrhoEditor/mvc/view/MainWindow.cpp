#include "MainWindow.h"
#include "ctrl/res/AssetBrowserController.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>   // for glfwGetWin32Window
#endif
#include "stb/stb_image.h"
#include "FileContextMenus.h"
#include "NodeContextMenus.h"


namespace Urho3DEditor
{
void MainWindowSizeCallBack(GLFWwindow* window, int width, int height)
{
    auto* mainWindow = static_cast<MainWindow*>(glfwGetWindowUserPointer(window));
    if (mainWindow)
        mainWindow->OnWindowSize(width, height);
}

MainWindow::MainWindow(int width, int height, SelectionController& selectionCtrl, SelectionModel& selectionModel, ToolController& toolCtrl, GizmoController& gizmoCtrl, CameraCtrl& cameraCtrl, PropertyEditController& propEditCtrl, SceneManipulationController& sceneManipCtrl, ProjectController& projectCtrl, SceneCtrl& sceneCtrl, AssetBrowserController& assetBrowserCtrl, HistoryMgr& historyMgr, const MainWindowServices& services, float dpiScale)
    : width{ width }
    , height{ height }
    , selectionController_(selectionCtrl)
    , selectionModel_(selectionModel)
    , toolController_(toolCtrl)
    , gizmoController_(gizmoCtrl)
    , cameraController_(cameraCtrl)
    , propertyEditController_(propEditCtrl)
    , sceneManipController_(sceneManipCtrl)
    , projectController_(projectCtrl)
    , sceneCtrl_(sceneCtrl)
    , assetBrowserController_(assetBrowserCtrl)
    , historyMgr_(historyMgr)
    , services_(services)
    , dpiScale_(dpiScale)
    , _startView(new StartView(
        [this]() { this->StartGame(); },
        services.selectPath,
        [this](const String& path) { projectController_.SetWorkSpace(path); },
        historyMgr))
    , _menuBar(new Menubar(projectCtrl, [this]() { _showDemo = !_showDemo; }))
    , _toolBar(new Toolbar(toolCtrl, projectCtrl, [&assetBrowserCtrl](const String& path) { return assetBrowserCtrl.GetImguiTex(path); }))
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
   
    //simfang.ttf
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    int dpi = 96;
#ifdef WIN32
    // HWND hwnd = glfwGetWin32Window(window);
    // LONG style = GetWindowLong(hwnd, GWL_STYLE);
    // SetWindowLong(hwnd, GWL_STYLE, style ^ WS_CAPTION);
    dpi = GetDpiForSystem();
#elif APPLE

#elif UNIX

#endif
    services_.setDpi(dpi);
    _toolBar->IntItemSize(dpi / 96.0f);
    float localDpiScale = dpi / 96.0f;
    float fontSize = localDpiScale * 16.0f;
    services_.setFontSize(fontSize);
    this->width = width * localDpiScale;
    this->height = height * localDpiScale;
    window = glfwCreateWindow(this->width, this->height, "Urho3D", NULL, NULL);
    glfwSetWindowUserPointer(window, this);
    GLFWimage images[4];
    //res/icons/urho64.png
    std::vector<std::string> imgPaths ={"urho64.png","urho48.png","urho32.png","urho16.png"};
    for(int i=0;i<4;++i)
    {
        images[i].pixels = stbi_load(("res/icons/"+imgPaths[i]).c_str(),&images[i].width,&images[i].height,0,4);
    }
    glfwSetWindowIcon(window,4,images);
    glfwSetWindowSizeCallback(window, MainWindowSizeCallBack);
    glfwSetWindowSize(window, this->width, this->height);
    glfwSetWindowPos(window, 300, 200);
    io.Fonts->AddFontFromFileTTF("res/simfang.ttf", fontSize, nullptr,
        io.Fonts->GetGlyphRangesChineseFull());
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    //menuBar = new WinMainMenu(hwnd);
    glfwMakeContextCurrent(window);
    //wglShareLists(g_HGLRC, data->hGLRC);
    //glfwMaximizeWindow(window);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 410";
    ImGui_ImplOpenGL3_Init(glsl_version);
    
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
    services_.startGame();
    _sceneView = new SceneView(selectionController_, toolController_, gizmoController_, cameraController_, sceneManipController_, sceneCtrl_, services_.getFps, "renderWindow");
   AddWindow(std::unique_ptr<SceneView>(_sceneView));
    _nodeTree = new NodeTree(selectionController_, selectionModel_, sceneCtrl_, sceneManipController_, dpiScale_);
   AddWindow(std::unique_ptr<NodeTree>(_nodeTree));
   _folderTree = new FolderTree(assetBrowserController_);
   AddWindow(std::unique_ptr<FolderTree>(_folderTree));
   _inspector = new Inspector(selectionModel_, propertyEditController_, projectController_);
   AddWindow(std::unique_ptr<Inspector>(_inspector));
   //_cosoleView = new ConsoleView();
   //AddWindow(std::unique_ptr<ConsoleView>(_cosoleView));
   _resPreview = new ResPreview();
   AddWindow(std::unique_ptr<ResPreview>(_resPreview));
   _folderFiles = new FolderFiles(projectController_, toolController_, assetBrowserController_, services_.getFontSize());
   AddWindow(std::unique_ptr<FolderFiles>(_folderFiles));

   FileContextMenus::Init(sceneCtrl_.GetContext(), &projectController_);
   NodeContextMenus::Init(sceneCtrl_.GetContext(), &sceneManipController_);
}

void MainWindow::OnWindowSize(int width, int height)
{
    if (width == 0)
    {
        miniSize_ = true;
    }
    else
    {
        if (miniSize_)
        {
            MakeCurrent();
        }
        miniSize_ = false;
    }
}

void MainWindow::MakeCurrent() {
    glfwMakeContextCurrent(window);
}

void MainWindow::UpdateDockerSpace()
{
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None| ImGuiDockNodeFlags_PassthruCentralNode;

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

  /*  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;*/

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("###DockSpace", &_showingDocker, window_flags);
    ImGui::PopStyleVar(1);
    //_menuBarUpdater->update();//todo
    if (services_.isStartView())
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
    if(ImGui::IsMouseDown(ImGuiMouseButton_Left) && !_lastLeftMouseDown)
    {
        //����л�״̬
        services_.updateCmdGuid();
    }
    _lastLeftMouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    glfwMakeContextCurrent(glfwGetCurrentContext());
    //����ȫ��Բ��
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding
        ,4.0f);
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
    ImGui::PopStyleVar(1);
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