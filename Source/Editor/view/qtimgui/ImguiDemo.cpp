#include "ImguiDemo.h"

namespace urho3d
{
namespace editor
{

ImguiDemo::ImguiDemo(QWidget* p)
	:ImguiWidget(p)
{

}

ImguiDemo::~ImguiDemo()
{
}

void ImguiDemo::updateUi()
{
    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImTextureID image_id = this->getImg("D:/setting.png");
        if (ImGui::ImageButton(image_id, ImVec2(32, 32)))
        {
            printf("µã»÷°´Å¥ÁË\n");
        }

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Test Window")) show_test_window ^= 1;
        if (ImGui::Button("Another Window")) show_another_window ^= 1;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();
    }

    glViewport(0, 0, width(), height());
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

}
}