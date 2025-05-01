#pragma once
#include "TransformGizmo.h"

class TranslateGizmo : public TransformGizmo
{
    URHO3D_OBJECT(TranslateGizmo, TransformGizmo);

public:
    TranslateGizmo(Context* ctx);
    ~TranslateGizmo();
    virtual void setActivePlane(std::string axis, Vector3 eye);

protected:
    void createGizmos(std::string axisName);

private:
};