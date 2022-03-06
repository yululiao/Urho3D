#pragma once
#include <vector>
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include "ImguiUpdater.h"
#include <memory>

class UIWindow
{
public:
    virtual ~UIWindow(){};
    virtual void update() = 0;
    void Show() { showing = true; }
    void Hide() { showing = false; }
    SharedPtr<MenuBarUpdater> _menuBarUpdater = nullptr;
protected:
    bool showing = true;
    std::string title;
    ImVec2 winSize;
    Vector2 curMousePos;
    bool mousePresed = false;
    bool mouseRelease = true;
};

class renderWindow : public UIWindow
{
public:
    renderWindow(const std::string& ptitle) 
	{ 
		title = ptitle;
        winSize = ImVec2(400,300);
	}
    virtual ~renderWindow();
    void update() override;
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

class DockerContainer :public UIWindow
{
public:
    DockerContainer() {}
    virtual ~DockerContainer() {}
    void update() override;
};

//glfw needs a window. This becomes our dockspace
class UMainWindow
{
public:
	UMainWindow(int width, int height);
	~UMainWindow();
	void Draw()const;
	bool WindowShouldClose()const;
	int getWidth()const { return width; }
	int getHeight()const { return height; }
	void Resize(int width, int height);
    GLFWwindow* getRawWindow();
    virtual void Update();
    //void showDockSpace(bool* p_open);
    void imguiUpdate();
    bool shouldClose() { return WindowShouldClose(); }
    SharedPtr<ImguiUpdater> _imguiUpdater = nullptr;
    SharedPtr<MenuBarUpdater> _menuBarUpdater = nullptr;
    void createUiUpdaer(Context* ctx) 
    { 
        _imguiUpdater = new ImguiUpdater(ctx);
        _menuBarUpdater = new MenuBarUpdater(ctx);
    }
    void AddWindow(std::unique_ptr<UIWindow> newWindow) 
    { 
        newWindow->_menuBarUpdater = _menuBarUpdater;
        windows.push_back(std::move(newWindow));
    }

    void maxSize();
    GLFWwindow* getGlfwWindow() 
    {
        return window;
    }
private:
    std::vector<std::unique_ptr<UIWindow>> windows;
	int width, height;
    GLFWwindow* window = nullptr;
    //MainMenu* menuBar = nullptr;
    bool done = false;
   
};



