#include "RotateGizmo.h"
#include "Urho3D/Math/Vector3.h"
#include "Urho3D/Math/Vector2.h"
#include "Urho3D/Math/Matrix4.h"
#include "ctrl/geometry_util.h"
#include "ctrl/utils.h"
#include "GizmoUtils.h"


namespace urho3d
{
	namespace editor
	{
		RotateGizmo::RotateGizmo(Context* ctx)
			:TransformGizmo(ctx)
		{
			createGizmos("X");
			createGizmos("Y");
			createGizmos("Z");
			//createGizmos("E");
			createGizmos("XYZE");

			init();
		}

		RotateGizmo::~RotateGizmo()
		{

		}

		void RotateGizmo::setActivePlane(std::string axis, Vector3 eye)
		{
			if (axis == "E") 
				activePlane = planesMap["XYZE"];

			if (axis == "X")
				activePlane = planesMap["YZ"];

			if (axis == "Y") 
				activePlane = planesMap["XZ"];

			if (axis == "Z") 
				activePlane = planesMap["XY"];
			
		}

		void RotateGizmo::update(Matrix4 objMatrix, Vector3 eye2)
		{
			TransformGizmo::update(objMatrix,eye2);
			updateGizmos(eye2);
		}

		void RotateGizmo::updateGizmos(Vector3 eye2)
		{
			if (mAllChild.size() <= 0)
				return;
			Matrix4 tempMatrix;
			Matrix4 worldRotation;
			Vector3 unitX(1, 0, 0);
			Vector3 unitY(0, 1, 0);
			Vector3 unitZ(0, 0, 1);
			Quaternion quaternionX;
			Quaternion quaternionY;
			Quaternion quaternionZ;
			Vector3 eye(0, 0, 0);
			worldRotation = this->GetRotation().RotationMatrix();
			Vector3 axis(0, 0, 0);
			float angle;
			tempMatrix = worldRotation;
			tempMatrix = tempMatrix.Inverse();
			eye = tempMatrix * eye2;
			for (auto it = mAllChild.begin(); it != mAllChild.end(); it++)
			{
				std::string name = (*it)->GetName().CString();
				if (name == "X")
				{
					quaternionX.FromAngleAxis(atan2(-eye.y_, eye.z_)*180/pi,unitX);
					(*it)->GetParent()->SetRotation(quaternionX);

				}
				else if (name == "Y")
				{
					quaternionY.FromAngleAxis(atan2(eye.x_, eye.z_) * 180/pi,unitY);
					(*it)->GetParent()->SetRotation(quaternionY);
				}
				else if (name == "Z")
				{
					quaternionZ.FromAngleAxis(atan2(eye.y_, eye.x_) *180/pi,unitZ);
					(*it)->GetParent()->SetRotation(quaternionZ);
				}
			}
		}

		void RotateGizmo::createGizmos(std::string axisName)
		{
			//line3d* line = new line3d();
			std::vector<Vector3> linePoints;
			float arc = 0.5;
			float radius = 0.7f;
			//vec3d pickPos;
			Vector3 pickAngles;
			unsigned circleColor;

			float pickRadius = 0.7;
			float radialSegments = 4;
			float tubularSegments = 12;
			float pickArc = pi;
			if (axisName == "X")
			{
				circleColor = 0xff0000;
				pickAngles = Vector3(0, -90, -90.0);

			}
			else if (axisName == "Y")
			{
				circleColor = 0x00ff00;
				pickAngles = Vector3(90, 0, 0);
			}
			else if (axisName == "Z")
			{
				circleColor = 0x0000ff;
				pickAngles = Vector3(0, 0, -90);
			}
			else if (axisName == "E")
			{
				arc = 1;
				radius = 0.8;
				circleColor = 0xffffff;
				pickRadius = 0.8;
				radialSegments = 4;
				tubularSegments = 24;
				pickArc = 2 * pi;
			}
			else if (axisName == "XYZE")
			{
				arc = 1;
				radius = 0.7;
				circleColor = 0xffffff;
			}

			for (int i = 0; i <= 64 * arc; ++i)
			{
				if (axisName == "X") linePoints.push_back(Vector3(0, cos(i / 32.0 * pi), sin(i / 32.0 * pi)) * radius);
				if (axisName == "Y") linePoints.push_back(Vector3(cos(i / 32.0 * pi), 0, sin(i / 32.0 * pi)) * radius);
				if (axisName == "Z" || axisName == "E" || axisName == "XYZE") linePoints.push_back(Vector3(sin(i / 32.0 * pi), cos(i / 32.0 * pi), 0) * radius);
			}
			//handles
			GizmogeoInfo* handleInfo = new GizmogeoInfo();
			SharedPtr<Node> circle = GeoUtils::create_line(context_,linePoints,circleColor);
			handleInfo->geo = circle;
			std::vector<GizmogeoInfo*> handleInfoList;
			handleInfoList.push_back(handleInfo);
			handleGizmos[axisName] = handleInfoList;
			//pick
			if (axisName == "XYZE" /*|| axisName == "E"*/) //no picker
				return;
			GizmogeoInfo* pickinfo = new GizmogeoInfo();
			pickinfo->geo = GeoUtils::create_torus(context_,pickRadius, 0.08, tubularSegments, radialSegments,pickArc);
			GizmoUtils::set_default_mat(pickinfo->geo);
			GizmoUtils::set_geo_color(pickinfo->geo,0x00000000 + circleColor);
			pickinfo->rot = pickAngles;
			std::vector<GizmogeoInfo*> pickInfoList;
			pickInfoList.push_back(pickinfo);
			pickerGizmos[axisName] = pickInfoList;

		}
		
	}
}