#pragma once
#include "CustomGLWidget.h"
#include <windows.h>

#include "QWidget"

namespace urho3d
{
namespace editor
{

class CustomGLWidget : public QWidget
{
public:
    CustomGLWidget(QWidget* p);
    ~CustomGLWidget();

    void initGL();
    void makeCurrent();
    void swapBuffer();

protected:
    HGLRC _context = nullptr;
    //��Ⱦ������ķ�ʽʵ�֣���Զ�contexЧ�ʸ�
    bool _render_to_tex = true;
    HDC _hdc;

};
}
} // namespace urho3d