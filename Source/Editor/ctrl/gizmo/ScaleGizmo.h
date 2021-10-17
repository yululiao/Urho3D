#pragma once
#include "TransformGizmo.h"


namespace urho3d
{
	namespace editor
	{
		class ScaleGizmo : public TransformGizmo
		{
			URHO3D_OBJECT(ScaleGizmo, TransformGizmo);
		public:
			ScaleGizmo(Context* ctx);
			~ScaleGizmo();
			virtual void setActivePlane(std::string axisName, Vector3 eye);
		protected:
			void createGizmos(std::string axisName);
		};
	}
}
