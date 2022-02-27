#include <GLEW/glew.h>
#include <QGuiApplication>
#include <QTimer>
#include <QSurfaceFormat>
#include <QOpenGLWindow>
#include <QOpenGLWidget>
#include <QImage>
#include "CustomGLWidget.h"
#include "imgui.h"
#include <map>

namespace urho3d
{
namespace editor
{

struct ImguiGLTexture1
{
    ImguiGLTexture1() {}
    ImguiGLTexture1(GLuint pid, double psize, double time)
    { 
        id = pid;
        size = psize;
        last_use_time = time;
    }
    GLuint id = 0;
    double size = 0.0;
    double last_use_time = 0.0;
};

class ImguiQGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    ImguiQGLWidget(QWidget* p);
    ~ImguiQGLWidget();
protected:
    virtual void updateUi() {}
    void initialize();
    void newFrame();
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
   
    void onMouseEvent(QMouseEvent* event);
    void onKeyEvent(QKeyEvent* event);

    void updateCursorShape(const ImGuiIO& io);
    void setCursorPos(const ImGuiIO& io);

    //opengl render
    void renderDrawList(ImDrawData* draw_data);
    bool createFontsTexture();
    void deleteFonstTexture();
    bool createDevObjs();
    void deleteDevObjs();
    //����·����������
    ImTextureID getImg(const std::string& path);
    //�ж��Ƿ���Ҫ����������
    void checkImgCach();
    void clearImgMap();

private:
    QTimer _timer;

    double       g_Time = 0.0f;
    bool         g_MousePressed[3] = { false, false, false };
    float        g_MouseWheel;
    float        g_MouseWheelH;
    GLuint       g_FontTexture = 0;
    int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
    int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
    int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
    unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;

    ImGuiContext* g_ctx = nullptr;
    //imgui�����б�����100Mʱ����һ��
    double _maxImgCach = 100.0;//200M
    double _curImgCach = 0.0;
    std::map<std::string, ImguiGLTexture1> _imgMap;
    double _maxUseTime = 5.0;
    double _elapse = 0.0;
    unsigned char* _fbo_pixels = nullptr;
    bool has_clear = false;
};

}
}

