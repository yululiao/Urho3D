#pragma once
#include "QWidget"
#include "QVBoxLayout"
#include "Urho3D/Scene/Component.h"

namespace urho3d
{
namespace editor
{
class CompInspector:public QWidget
{
public:
    CompInspector(QWidget* p,Urho3D::Component* comp);
    ~CompInspector();
public slots:
protected:
private:
    Urho3D::Component* _comp = nullptr;
};
}

}