#include <QDateTime>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QClipboard>
#include <QCursor>
#include <ctrl/utils.h>
#include <iostream>
#include "ImguiQGLWidget.h"

#ifdef USE_GLSL_ES
#define IMGUIRENDERER_GLSL_VERSION "#version 300 es\n"
#else
#define IMGUIRENDERER_GLSL_VERSION "#version 330\n"
#endif

namespace urho3d
{
namespace editor
{

const QHash<int, ImGuiKey> keyMap1 = {
    { Qt::Key_Tab, ImGuiKey_Tab },
    { Qt::Key_Left, ImGuiKey_LeftArrow },
    { Qt::Key_Right, ImGuiKey_RightArrow },
    { Qt::Key_Up, ImGuiKey_UpArrow },
    { Qt::Key_Down, ImGuiKey_DownArrow },
    { Qt::Key_PageUp, ImGuiKey_PageUp },
    { Qt::Key_PageDown, ImGuiKey_PageDown },
    { Qt::Key_Home, ImGuiKey_Home },
    { Qt::Key_End, ImGuiKey_End },
    { Qt::Key_Insert, ImGuiKey_Insert },
    { Qt::Key_Delete, ImGuiKey_Delete },
    { Qt::Key_Backspace, ImGuiKey_Backspace },
    { Qt::Key_Space, ImGuiKey_Space },
    { Qt::Key_Enter, ImGuiKey_Enter },
    { Qt::Key_Return, ImGuiKey_Enter },
    { Qt::Key_Escape, ImGuiKey_Escape },
    { Qt::Key_A, ImGuiKey_A },
    { Qt::Key_C, ImGuiKey_C },
    { Qt::Key_V, ImGuiKey_V },
    { Qt::Key_X, ImGuiKey_X },
    { Qt::Key_Y, ImGuiKey_Y },
    { Qt::Key_Z, ImGuiKey_Z },
    //{ Qt::MiddleButton, ImGuiMouseButton_Middle }
};

#ifndef QT_NO_CURSOR
const QHash<ImGuiMouseCursor, Qt::CursorShape> cursorMap1 = {
    { ImGuiMouseCursor_Arrow,      Qt::CursorShape::ArrowCursor },
    { ImGuiMouseCursor_TextInput,  Qt::CursorShape::IBeamCursor },
    { ImGuiMouseCursor_ResizeAll,  Qt::CursorShape::SizeAllCursor },
    { ImGuiMouseCursor_ResizeNS,   Qt::CursorShape::SizeVerCursor },
    { ImGuiMouseCursor_ResizeEW,   Qt::CursorShape::SizeHorCursor },
    { ImGuiMouseCursor_ResizeNESW, Qt::CursorShape::SizeBDiagCursor },
    { ImGuiMouseCursor_ResizeNWSE, Qt::CursorShape::SizeFDiagCursor },
    { ImGuiMouseCursor_Hand,       Qt::CursorShape::PointingHandCursor },
   // { ImGuiMouseCursor_NotAllowed, Qt::CursorShape::ForbiddenCursor },
};
#endif

QByteArray g_currentClipboardText1;

static const GLenum windowBuff[] = {GL_BACK_LEFT};
static const GLenum fboBuffs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

ImguiQGLWidget::ImguiQGLWidget(QWidget* p)
    : QOpenGLWidget(p)
{
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(1000.0/60.0);
}
ImguiQGLWidget::~ImguiQGLWidget()
{
    if (!has_clear)
    {
        has_clear = true;
        _timer.stop();
        deleteDevObjs();
        ImGui::DestroyContext(g_ctx);
        clearImgMap();
    }
}

//关闭窗口时清理所有，避免内存泄漏
void ImguiQGLWidget::closeEvent(QCloseEvent* event)
{
    if (!has_clear)
    {
        has_clear = true;
        _timer.stop();
        deleteDevObjs();
        ImGui::DestroyContext(g_ctx);
        clearImgMap();
    }
   
}

void ImguiQGLWidget::showEvent(QShowEvent* event)
{
    if (has_clear)
    {
        initialize();
        _timer.start(1000.0 / 60.0);
        has_clear = false;
    }
}

ImTextureID ImguiQGLWidget::getImg(const std::string& path) 
{
    if (_imgMap.find(path) != _imgMap.end())
    {
        _imgMap[path].last_use_time = _elapse;
        return (ImTextureID)_imgMap[path].id;
    }
        
    // 使用SOIL进行照片像素数据的加载
    if (!Utils::file_exist(path))
    {
        std::cerr << "faild to load imgui img,file not exist!!!!"<<std::endl;
        return 0;
    }
    QImage img(path.c_str());
    if (img.width() <= 0)
    {
        std::cerr << "faild to load imgui img,maybe fromat error!!!!" << std::endl;
        return 0;
    }
   
    // 使用GL指令生成贴图，获取贴图ID
    GLuint t_texId;
    glGenTextures(1, &t_texId);
    glBindTexture(GL_TEXTURE_2D, t_texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum format = GL_RGB;
    if (img.format() == QImage::Format::Format_ARGB32)
    {
        format = GL_RGBA;
        img.convertTo(QImage::Format::Format_RGBA8888);
    }
    else
    {
        img.convertTo(QImage::Format::Format_RGB888);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, img.width(), img.height(), 0, format, GL_UNSIGNED_BYTE, img.bits());
    glGenerateMipmap(GL_TEXTURE_2D);
    double imgsize = img.byteCount() / 1024.0 / 1024.0;
    _curImgCach = _curImgCach + imgsize;
    _imgMap[path] = ImguiGLTexture1(t_texId, imgsize, _elapse);
    return (ImTextureID)t_texId;
}

void ImguiQGLWidget::checkImgCach()
{
    std::map<std::string, ImguiGLTexture1> tmpImgMap;
    _curImgCach = 0.0;
    for (auto item : _imgMap)
    {
        if ((_elapse - item.second.last_use_time) < _maxUseTime)
        {
            tmpImgMap[item.first] = item.second;
            _curImgCach = _curImgCach + item.second.size;
        }
        else
        {
            //清理纹理对象
            std::cout << "clear imgui texture unusefull!!" << std::endl;
            glDeleteTextures(1, &item.second.id);
        }
    }
    _imgMap.clear();
    _imgMap = tmpImgMap;
    if (_curImgCach >= _maxImgCach) //清理完长时间不用的纹理后还是超标，直接全部清理
    {
        clearImgMap();
    }
}

void ImguiQGLWidget::clearImgMap() 
{
    //强制全部清理
    std::cout << "clear all imgui texture" << std::endl;
    for (auto item : _imgMap)
    {
        //清理纹理对象
        glDeleteTextures(1, &item.second.id);
    }
    _imgMap.clear();
    _curImgCach = 0.0;
}

void ImguiQGLWidget::onMouseEvent(QMouseEvent* event)
{
    g_MousePressed[0] = event->buttons() & Qt::LeftButton;
    g_MousePressed[1] = event->buttons() & Qt::RightButton;
    g_MousePressed[2] = event->buttons() & Qt::MiddleButton;
}

void ImguiQGLWidget::initialize()
{
    g_ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(g_ctx);
    ImGuiIO& io = ImGui::GetIO();
#ifndef QT_NO_CURSOR
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
#endif
    io.BackendPlatformName = "qtimgui";

    // Setup keyboard mapping
    for (ImGuiKey key : keyMap1.values())
    {
        io.KeyMap[key] = key;
    }

    io.SetClipboardTextFn = [](void* user_data, const char* text) {
        Q_UNUSED(user_data);
        QGuiApplication::clipboard()->setText(text);
    };
    io.GetClipboardTextFn = [](void* user_data) {
        Q_UNUSED(user_data);
        g_currentClipboardText1 = QGuiApplication::clipboard()->text().toUtf8();
        return (const char*)g_currentClipboardText1.data();
    };
}

void ImguiQGLWidget::newFrame()
{
    ImGui::SetCurrentContext(g_ctx);
    if (!g_FontTexture)
        createDevObjs();
    ImGuiIO& io = ImGui::GetIO();
    // Setup display size (every frame to accommodate for window resizing)
    io.DisplaySize = ImVec2(this->size().width(), this->size().height());
    io.DisplayFramebufferScale = ImVec2(this->devicePixelRatio(), this->devicePixelRatio());

    // Setup time step
    double current_time = QDateTime::currentMSecsSinceEpoch()/(double)1000;
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    // If ImGui wants to set cursor position (for example, during navigation by using keyboard)
    // we need to do it here (before getting `QCursor::pos()` below).
   // setCursorPos(io);
    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    if (this->isActiveWindow())
    {
        const QPoint pos = this->mapFromGlobal(QCursor::pos());
        io.MousePos = ImVec2(pos.x(), pos.y());   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
    }
    else
    {
        io.MousePos = ImVec2(-1, -1);
    }

    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = g_MousePressed[i];
    }

    io.MouseWheelH = g_MouseWheelH;
    io.MouseWheel = g_MouseWheel;
    g_MouseWheelH = 0;
    g_MouseWheel = 0;
    updateCursorShape(io);

    // Start the frame
    ImGui::NewFrame();
}

void ImguiQGLWidget::initializeGL() 
{
    initialize(); 
}

void ImguiQGLWidget::paintGL()
{
    //检查并清理纹理缓存
    _elapse = QDateTime::currentMSecsSinceEpoch() / (double)1000;
    checkImgCach();
    if (this->isMinimized() || !this->isVisible())
        return;
    makeCurrent();
    newFrame();
    updateUi();
    ImGui::EndFrame();
    ImGui::Render();

    auto drawData = ImGui::GetDrawData();
    renderDrawList(drawData);
    doneCurrent();
}



void ImguiQGLWidget::mousePressEvent(QMouseEvent* event)
{
    onMouseEvent(event);
}
void ImguiQGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    onMouseEvent(event);
}
void ImguiQGLWidget::wheelEvent(QWheelEvent* event)
{
    // Select current context
    ImGui::SetCurrentContext(g_ctx);

    // Handle horizontal component
    if (event->pixelDelta().x() != 0)
    {
        g_MouseWheelH += event->pixelDelta().x() / (ImGui::GetTextLineHeight());
    }
    else
    {
        // Magic number of 120 comes from Qt doc on QWheelEvent::pixelDelta()
        g_MouseWheelH += event->angleDelta().x() / 120;
    }

    // Handle vertical component
    if (event->pixelDelta().y() != 0)
    {
        // 5 lines per unit
        g_MouseWheel += event->pixelDelta().y() / (5.0 * ImGui::GetTextLineHeight());
    }
    else
    {
        // Magic number of 120 comes from Qt doc on QWheelEvent::pixelDelta()
        g_MouseWheel += event->angleDelta().y() / 120;
    }
}

void ImguiQGLWidget::onKeyEvent(QKeyEvent* event)
{
    // Select current context
    ImGui::SetCurrentContext(g_ctx);

    ImGuiIO& io = ImGui::GetIO();

    const bool key_pressed = (event->type() == QEvent::KeyPress);

    // Translate `Qt::Key` into `ImGuiKey`, and apply 'pressed' state for that key
    const auto key_it = keyMap1.constFind(event->key());
    if (key_it != keyMap1.constEnd())
    { // Qt's key found in keyMap
        const int imgui_key = *(key_it);
        io.KeysDown[imgui_key] = key_pressed;
    }

    if (key_pressed)
    {
        const QString text = event->text();
        if (text.size() == 1)
        {
            io.AddInputCharacter(text.at(0).unicode());
        }
    }

#ifdef Q_OS_MAC
    io.KeyCtrl = event->modifiers() & Qt::MetaModifier;
    io.KeyShift = event->modifiers() & Qt::ShiftModifier;
    io.KeyAlt = event->modifiers() & Qt::AltModifier;
    io.KeySuper = event->modifiers() & Qt::ControlModifier; // Comamnd key
#else
    io.KeyCtrl = event->modifiers() & Qt::ControlModifier;
    io.KeyShift = event->modifiers() & Qt::ShiftModifier;
    io.KeyAlt = event->modifiers() & Qt::AltModifier;
    io.KeySuper = event->modifiers() & Qt::MetaModifier;
#endif
}

void ImguiQGLWidget::updateCursorShape(const ImGuiIO& io)
{
#ifndef QT_NO_CURSOR
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    const ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || (imgui_cursor == ImGuiMouseCursor_None))
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        setCursor(Qt::CursorShape::BlankCursor);
    }
    else
    {
        // Show OS mouse cursor

        // Translate `ImGuiMouseCursor` into `Qt::CursorShape` and show it, if we can
        const auto cursor_it = cursorMap1.constFind(imgui_cursor);
        if (cursor_it != cursorMap1.constEnd()) // `Qt::CursorShape` found for `ImGuiMouseCursor`
        {
            const Qt::CursorShape qt_cursor_shape = *(cursor_it);
            setCursor(qt_cursor_shape);
        }
        else // shape NOT found - use default
        {
            setCursor(Qt::CursorShape::ArrowCursor);
        }
    }
#else
    Q_UNUSED(io);
#endif
}

void ImguiQGLWidget::setCursorPos(const ImGuiIO& io)
{
    if (!io.WantSetMousePos)
    {
        return;
    }
#ifndef QT_NO_CURSOR
    // Convert position from window-space into screen-space
    QPoint local_pos = { (int)io.MousePos.x, (int)io.MousePos.y };
    const QPoint global_pos = mapToGlobal(local_pos);

    QCursor cursor = this->cursor();
    cursor.setPos(global_pos);
    setCursor(cursor);
#else
    Q_UNUSED(local_pos);
#endif
}

void ImguiQGLWidget::keyPressEvent(QKeyEvent* event)
{
    onKeyEvent(event);
}
void ImguiQGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    onKeyEvent(event);
}


void ImguiQGLWidget::renderDrawList(ImDrawData* draw_data)
{
    ImGui::SetCurrentContext(g_ctx);

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    const ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Backup GL state
    GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
    GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
    GLint last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, &last_blend_src_rgb);
    GLint last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, &last_blend_dst_rgb);
    GLint last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, &last_blend_src_alpha);
    GLint last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, &last_blend_dst_alpha);
    GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
    GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    const float ortho_projection[4][4] =
    {
        { 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
        { 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
        { 0.0f,                  0.0f,                  -1.0f, 0.0f },
        {-1.0f,                  1.0f,                   0.0f, 1.0f },
    };
    glUseProgram(g_ShaderHandle);
    glUniform1i(g_AttribLocationTex, 0);
    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
    glBindVertexArray(g_VaoHandle);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(size_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glActiveTexture(last_active_texture);
    glBindVertexArray(last_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}
bool ImguiQGLWidget::createFontsTexture()
{
    // Select current context
    ImGui::SetCurrentContext(g_ctx);

    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Store our identifier
    io.Fonts->TexID = (void*)(size_t)g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

    return true;
}
void ImguiQGLWidget::deleteFonstTexture() 
{ 
    ImGuiIO& io = ImGui::GetIO();
    glDeleteTextures(1, &g_FontTexture);
    io.Fonts->SetTexID(0);
    g_FontTexture = 0;
}

bool ImguiQGLWidget::createDevObjs()
{
    ImGui::SetCurrentContext(g_ctx);

    // Backup GL state
    GLint last_texture, last_array_buffer, last_vertex_array;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

    const GLchar* vertex_shader =
        IMGUIRENDERER_GLSL_VERSION
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 UV;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "	Frag_UV = UV;\n"
        "	Frag_Color = Color;\n"
        "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
        "}\n";

    const GLchar* fragment_shader =
        IMGUIRENDERER_GLSL_VERSION
        "precision mediump float;"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main()\n"
        "{\n"
        "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
        "}\n";

    g_ShaderHandle = glCreateProgram();
    g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
    g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
    glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
    glCompileShader(g_VertHandle);
    glCompileShader(g_FragHandle);
    glAttachShader(g_ShaderHandle, g_VertHandle);
    glAttachShader(g_ShaderHandle, g_FragHandle);
    glLinkProgram(g_ShaderHandle);

    g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
    g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
    g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
    g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
    g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

    glGenBuffers(1, &g_VboHandle);
    glGenBuffers(1, &g_ElementsHandle);

    glGenVertexArrays(1, &g_VaoHandle);
    glBindVertexArray(g_VaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
    glEnableVertexAttribArray(g_AttribLocationPosition);
    glEnableVertexAttribArray(g_AttribLocationUV);
    glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
    glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
    glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

    createFontsTexture();

    // Restore modified GL state
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
    glBindVertexArray(last_vertex_array);

    return true;
}

void ImguiQGLWidget::deleteDevObjs() 
{
    if (g_VboHandle)
    {
        glDeleteBuffers(1, &g_VboHandle);
        g_VboHandle = 0;
    }
    if (g_ElementsHandle)
    {
        glDeleteBuffers(1, &g_ElementsHandle);
        g_ElementsHandle = 0;
    }
    if (g_ShaderHandle && g_VertHandle)
    {
        glDetachShader(g_ShaderHandle, g_VertHandle);
    }
    if (g_ShaderHandle && g_FragHandle)
    {
        glDetachShader(g_ShaderHandle, g_FragHandle);
    }
    if (g_VertHandle)
    {
        glDeleteShader(g_VertHandle);
        g_VertHandle = 0;
    }
    if (g_FragHandle)
    {
        glDeleteShader(g_FragHandle);
        g_FragHandle = 0;
    }
    if (g_ShaderHandle)
    {
        glDeleteProgram(g_ShaderHandle);
        g_ShaderHandle = 0;
    }
    deleteFonstTexture();
}


}
}