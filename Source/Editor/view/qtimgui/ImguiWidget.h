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

struct ImguiGLTexture
{
    ImguiGLTexture() {}
    ImguiGLTexture(GLuint pid,double psize,double time)
    { 
        id = pid;
        size = psize;
        last_use_time = time;
    }
    GLuint id = 0;
    double size = 0.0;
    double last_use_time = 0.0;
};

class ImguiWidget : public CustomGLWidget
{
    Q_OBJECT
public:
    ImguiWidget(QWidget* p);
    ~ImguiWidget();
public slots:
    void onTimer();
protected:
    virtual void updateUi() {}
    void initialize();
    void newFrame();
    void render();
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
   
    void onMouseEvent(QMouseEvent* event);
    void onKeyEvent(QKeyEvent* event);

    void updateCursorShape(const ImGuiIO& io);
    void setCursorPos(const ImGuiIO& io);

    //opengl render
    void renderDrawList(ImDrawData* draw_data);
    bool createFontsTexture();
    void deleteFonstTexture();
    void createFbo();
    void deleteFbo();
    bool createDevObjs();
    void deleteDevObjs();
    void fboToWidget();
    //绝对路径创建纹理
    ImTextureID getImg(const std::string& path);
    //判断是否需要清理纹理缓存
    void checkImgCach();
    void clearImgMap();

    void clearAll();

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
    GLuint fboID = 0;
    GLuint mirrorTexture = 0;
    GLuint depthBufferID = 0;
    bool _hasfboInit = false;
    //imgui纹理列表，超过100M时清理一次
    double _maxImgCach = 100.0;//200M
    double _curImgCach = 0.0;
    std::map<std::string, ImguiGLTexture> _imgMap;
    double _maxUseTime = 5.0;
    double _elapse = 0.0;
    unsigned char* _fbo_pixels = nullptr;
    bool has_clear = false;
    int frameRate = 60;
};

}
}

