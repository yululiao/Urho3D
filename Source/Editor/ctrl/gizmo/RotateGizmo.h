#pragma once
#include "TransformGizmo.h"

namespace urho3d
{
	namespace editor
	{
		class RotateGizmo : public TransformGizmo
		{
			URHO3D_OBJECT(RotateGizmo,TransformGizmo);
		public:
			RotateGizmo(Context* ctx);
			~RotateGizmo();
			virtual void setActivePlane(std::string axis, Vector3 eye);
			virtual void update(Matrix4 objMatrix, Vector3 eye) override;
		protected:
			void createGizmos(std::string axisName);
			float pi = 3.1415926f;
			void updateGizmos(Vector3 eye);
		};
	}
}