#pragma once
#include "ImguiWidget.h"

namespace urho3d
{
namespace editor
{
class ImguiDemo :public ImguiWidget
{
public:
	ImguiDemo(QWidget* p);
	~ImguiDemo();
protected:
	void updateUi() override;
	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);
    //ImVec4 clear_color = ImColor(0, 0, 0);
};

}
}
