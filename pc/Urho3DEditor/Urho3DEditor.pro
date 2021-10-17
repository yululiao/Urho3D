QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Source/Editor/ctrl/asset_mgr.cpp \
    ../../Source/Editor/ctrl/camera_ctrl.cpp \
    ../../Source/Editor/ctrl/edit_mgr.cpp \
    ../../Source/Editor/ctrl/event_emitter.cpp \
    ../../Source/Editor/ctrl/file_system_watcher.cpp \
    ../../Source/Editor/ctrl/geometry_util.cpp \
    ../../Source/Editor/ctrl/gizmo/GizmoUtils.cpp \
    ../../Source/Editor/ctrl/gizmo/RotateGizmo.cpp \
    ../../Source/Editor/ctrl/gizmo/ScaleGizmo.cpp \
    ../../Source/Editor/ctrl/gizmo/TransformCtrl.cpp \
    ../../Source/Editor/ctrl/gizmo/TransformGizmo.cpp \
    ../../Source/Editor/ctrl/gizmo/TranslateGizmo.cpp \
    ../../Source/Editor/ctrl/global_event.cpp \
    ../../Source/Editor/ctrl/history_mgr.cpp \
    ../../Source/Editor/ctrl/scene_ctrl.cpp \
    ../../Source/Editor/ctrl/translate.cpp \
    ../../Source/Editor/ctrl/work_space.cpp \
    ../../Source/Editor/editor_app.cpp \
    ../../Source/Editor/main.cpp \
    ../../Source/Editor/model/proj_history.cpp \
    ../../Source/Editor/view/inspector.cpp \
    ../../Source/Editor/view/main_window.cpp \
    ../../Source/Editor/view/menu_bar.cpp \
    ../../Source/Editor/view/node_tree.cpp \
    ../../Source/Editor/view/render_view.cpp \
    ../../Source/Editor/view/res_tree.cpp \
    ../../Source/Editor/view/start_view.cpp \
    ../../Source/Editor/view/styles/dark_style.cpp \
    ../../Source/Editor/view/tool_bar.cpp

HEADERS += \
    ../../Source/Editor/ctrl/FastDelegate.h \
    ../../Source/Editor/ctrl/asset_mgr.h \
    ../../Source/Editor/ctrl/camera_ctrl.h \
    ../../Source/Editor/ctrl/edit_command.h \
    ../../Source/Editor/ctrl/edit_mgr.h \
    ../../Source/Editor/ctrl/editor_delegate.h \
    ../../Source/Editor/ctrl/event_data.h \
    ../../Source/Editor/ctrl/event_emitter.h \
    ../../Source/Editor/ctrl/file_system_watcher.h \
    ../../Source/Editor/ctrl/geometry_util.h \
    ../../Source/Editor/ctrl/gizmo/GizmoUtils.h \
    ../../Source/Editor/ctrl/gizmo/RotateGizmo.h \
    ../../Source/Editor/ctrl/gizmo/ScaleGizmo.h \
    ../../Source/Editor/ctrl/gizmo/TransformCtrl.h \
    ../../Source/Editor/ctrl/gizmo/TransformGizmo.h \
    ../../Source/Editor/ctrl/gizmo/TranslateGizmo.h \
    ../../Source/Editor/ctrl/global_event.h \
    ../../Source/Editor/ctrl/history_mgr.h \
    ../../Source/Editor/ctrl/scene_ctrl.h \
    ../../Source/Editor/ctrl/translate.h \
    ../../Source/Editor/ctrl/utils.h \
    ../../Source/Editor/ctrl/work_space.h \
    ../../Source/Editor/editor_app.h \
    ../../Source/Editor/model/proj_history.h \
    ../../Source/Editor/view/inspector.h \
    ../../Source/Editor/view/main_window.h \
    ../../Source/Editor/view/menu_bar.h \
    ../../Source/Editor/view/node_tree.h \
    ../../Source/Editor/view/render_view.h \
    ../../Source/Editor/view/res_tree.h \
    ../../Source/Editor/view/start_view.h \
    ../../Source/Editor/view/styles/dark_style.h \
    ../../Source/Editor/view/tool_bar.h

FORMS += \
    ../../Source/Editor/forms/start.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../../Source/Editor/qtres/Urho3dEditor.qrc \
    ../../Source/Editor/qtres/styles/darkstyle.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../msbuild/lib/ -lUrho3D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../msbuild/lib/ -lUrho3D_d
else:unix: LIBS += -L$$PWD/../msbuild/lib/ -lUrho3D

INCLUDEPATH += $$PWD/../../Source/Editor
INCLUDEPATH += $$PWD/../msbuild/include
INCLUDEPATH += $$PWD/../msbuild/include/Urho3D
INCLUDEPATH += $$PWD/../msbuild/include/Urho3D/ThirdParty

DEPENDPATH += $$PWD/../msbuild/include

unix|win32: LIBS += -luser32
unix|win32: LIBS += -lgdi32
unix|win32: LIBS += -lwinmm
unix|win32: LIBS += -limm32
unix|win32: LIBS += -lole32
unix|win32: LIBS += -loleaut32
unix|win32: LIBS += -lsetupapi
unix|win32: LIBS += -lversion
unix|win32: LIBS += -luuid
unix|win32: LIBS += -lws2_32
unix|win32: LIBS += -liphlpapi
unix|win32: LIBS += -ldbghelp
unix|win32: LIBS += -lopengl32
unix|win32: LIBS += -lkernel32
unix|win32: LIBS += -lwinspool
unix|win32: LIBS += -lshell32
unix|win32: LIBS += -lcomdlg32
unix|win32: LIBS += -ladvapi32
