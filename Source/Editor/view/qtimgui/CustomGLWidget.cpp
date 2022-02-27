#include "CustomGLWidget.h"
#include <windows.h>
#include <iostream>

namespace urho3d
{
namespace editor
{

CustomGLWidget::CustomGLWidget(QWidget* p) 
    :QWidget(p) 
{ 
    initGL();
}

CustomGLWidget::~CustomGLWidget()
{ 
    if (_context)
    {
        wglDeleteContext(_context);
    } 
}
void CustomGLWidget::initGL()
{
    if (_render_to_tex)
        return;
    _hdc = GetDC((HWND)this->winId());
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
    if (pixelFormat == 0)
    {
        return;
    }

    DescribePixelFormat(_hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (!SetPixelFormat(_hdc, pixelFormat, &pfd))
    {
        return;
    }

    _context = wglCreateContext(_hdc);
    wglMakeCurrent(_hdc, _context);
}
void CustomGLWidget::makeCurrent()
{
    if (_render_to_tex)
        return;
    wglMakeCurrent(_hdc, _context);
}

void CustomGLWidget::swapBuffer()
{
    // makeCurent();
    if (_render_to_tex)
        return;
    SwapBuffers(_hdc);
}

} // namespace editor
} // namespace ej2d