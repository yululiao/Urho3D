#include "TranslateGizmo.h"
#include "ctrl/geometry_util.h"
#include "ctrl/utils.h"
#include "GizmoUtils.h"


namespace urho3d
{
	namespace editor
	{

		TranslateGizmo::TranslateGizmo(Context* ctx)
			:TransformGizmo(ctx)
		{
			createGizmos("X");
			createGizmos("Y");
			createGizmos("Z");

			init();
		}

		TranslateGizmo::~TranslateGizmo()
		{

		}

		void TranslateGizmo::createGizmos(std::string axisName)
		{
			Vector3 axisAngle;
			Vector3 axisPos;
			unsigned axisColor;
			std::vector<Vector3> linePoints;
			Vector3 planAngle;
			Vector3 planPos;
			Vector3 pickAxisPos;
			Vector3 pickPlanPos;
			std::string planeName;
			if (axisName == "X")
			{
				axisAngle = Vector3(0, 90, 0);
				axisPos = Vector3(0.8, 0, 0);
				axisColor = 0xff0000;
				linePoints = { Vector3(0,0,0),Vector3(0.8,0,0) };
				planAngle = Vector3(0, 90, 0);
				planPos = Vector3(0, 0.15, 0.15);
				pickAxisPos = Vector3(0.4, 0, 0);
				pickPlanPos = Vector3(0, 0.2, 0.2);
				planeName = "YZ";
			}
			else if (axisName == "Y")
			{
				axisAngle = Vector3(-90, 0, 0);
				axisPos = Vector3(0, 0.8, 0);
				axisColor = 0x00ff00;
				linePoints = { Vector3(0,0,0),Vector3(0,0.8,0) };
				planAngle = Vector3(-90, 0, 0);
				planPos = Vector3(0.15, 0, 0.15);
				pickAxisPos = Vector3(0, 0.4, 0);
				pickPlanPos = Vector3(0.2, 0, 0.2);
				planeName = "XZ";
			}
			else if (axisName == "Z")
			{
				//axisAngle = Vector3(0, 0, -pi / 2);
				axisPos = Vector3(0, 0, 0.8);
				axisColor = 0x0000ff;
				linePoints = { Vector3(0,0,0),Vector3(0,0,0.8) };
				planPos = Vector3(0.15, 0.15, 0);
				pickAxisPos = Vector3(0, 0, 0.4);
				pickPlanPos = Vector3(0.2, 0.2, 0);
				planeName = "XY";
			}
			GizmogeoInfo* axisArrowGizmo = new GizmogeoInfo();
			axisArrowGizmo->geo = geometry_util::create_cylinder(context_,0.05,0,0.2,12,1);
			GizmoUtils::set_default_mat(axisArrowGizmo->geo);
			GizmoUtils::set_geo_color(axisArrowGizmo->geo,0xff000000 + axisColor);
			//axisArrowGizmo->geo->set_color(axisColor);
			axisArrowGizmo->pos = axisPos;
			axisArrowGizmo->rot = axisAngle;;
			GizmogeoInfo* axisLineGizmo = new GizmogeoInfo();
			//auto xline = new lines_3d();
			SharedPtr<Node> xline = geometry_util::create_line(context_,linePoints,axisColor);
			//xline->set_vertices(linePoints);
			//xline->set_color(axisColor);
			axisLineGizmo->geo = xline;
			std::vector<GizmogeoInfo*> xGizmos;
			xGizmos.push_back(axisArrowGizmo);
			xGizmos.push_back(axisLineGizmo);
			handleGizmos[axisName] = xGizmos;
			
			GizmogeoInfo *planeGizmo = new GizmogeoInfo();
			planeGizmo->geo = geometry_util::create_plane(context_,0.29,0.29);
			//planeGizmo->geo->set_color(axisColor);
			//planeGizmo->geo->get_material()->set_blend_mode(blend_mode::NORMAL);
			//planeGizmo->geo->set_alpha(0.8 * 255);
			GizmoUtils::set_default_mat(planeGizmo->geo);
			GizmoUtils::set_geo_color(planeGizmo->geo, 0xff000000 + axisColor);
			planeGizmo->pos = planPos;
			planeGizmo->rot = planAngle;
			std::vector<GizmogeoInfo*> yzPlane;
			yzPlane.push_back(planeGizmo);
			handleGizmos[planeName] = yzPlane;
			

			GizmogeoInfo* axisPickGizmo = new GizmogeoInfo();
			axisPickGizmo->geo = geometry_util::create_cylinder(context_,0,0.15,0.6,4,1);
			//axisPickGizmo->geo->set_visible(false);
			//axisPickGizmo->geo->set_color(axisColor);
			//axisPickGizmo->geo->get_material()->set_blend_mode(blend_mode::NORMAL);
			//axisPickGizmo->geo->set_alpha( 0* 255);
			GizmoUtils::set_default_mat(axisPickGizmo->geo);
			GizmoUtils::set_geo_color(axisPickGizmo->geo, 0x00000000 + axisColor);
			axisPickGizmo->pos = pickAxisPos;
			axisPickGizmo->rot = axisAngle;
			std::vector<GizmogeoInfo*> xPickGizmo;
			xPickGizmo.push_back(axisPickGizmo);
			pickerGizmos[axisName] = xPickGizmo;

			GizmogeoInfo* planePickGizmo = new GizmogeoInfo();
			planePickGizmo->geo = geometry_util::create_plane(context_,0.4,0.4);
			/*planePickGizmo->geo->set_color(axisColor);
			planePickGizmo->geo->get_material()->set_blend_mode(blend_mode::NORMAL);
			planePickGizmo->geo->set_alpha(0*255);*/
			GizmoUtils::set_default_mat(planePickGizmo->geo);
			GizmoUtils::set_geo_color(planePickGizmo->geo, 0x00000000 + axisColor);
			planePickGizmo->pos = pickPlanPos;
			planePickGizmo->rot = planAngle;

			std::vector<GizmogeoInfo*> yzPickGizme;
			yzPickGizme.push_back(planePickGizmo);
			pickerGizmos[planeName] = yzPickGizme;
		}
		
		void TranslateGizmo::setActivePlane(std::string axis, Vector3 eye)
		{
			Matrix4 rot;
			rot = this->GetRotation().RotationMatrix();
			rot = rot.Inverse();
			Vector3 neweye;
			//rot.vec_mul(eye,neweye);
			neweye = rot * eye;
			eye = neweye;

			if (axis == "X") {
				this->activePlane = this->planesMap["XY"];
				if (std::abs(eye.y_) > std::abs(eye.z_))
					activePlane = planesMap["XZ"];
			}

			if (axis == "Y") {
				activePlane = planesMap["XY"];
				if (std::abs(eye.x_) > std::abs(eye.z_))
					activePlane = planesMap["YZ"];
			}

			if (axis == "Z") {
				activePlane = planesMap["XZ"];
				if (std::abs(eye.x_) > std::abs(eye.y_))
					activePlane = planesMap["YZ"];
			}

			if (axis == "XY") 
				activePlane = planesMap["XY"];

			if (axis == "YZ") 
				activePlane = planesMap["YZ"];

			if (axis == "XZ") 
				activePlane = planesMap["XZ"];

		}
	}
}