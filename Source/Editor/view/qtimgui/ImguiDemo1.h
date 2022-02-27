#pragma once
#include "ImguiQGLWidget.h"

namespace urho3d
{
namespace editor
{
class ImguiDemo1 :public ImguiQGLWidget
{
public:
    ImguiDemo1(QWidget* p);
    ~ImguiDemo1();
protected:
	void updateUi() override;
	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);
    //ImVec4 clear_color = ImColor(0, 0, 0);
};

}
}
