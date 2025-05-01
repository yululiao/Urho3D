#include "ScaleGizmo.h"
#include "Urho3D/Graphics/GeoUtils.h"
#include "GizmoUtils.h"

using namespace Urho3D;

ScaleGizmo::ScaleGizmo(Context* ctx)
	:TransformGizmo(ctx)
{
	createGizmos("X");
	createGizmos("Y");
	createGizmos("Z");
	createGizmos("XYZ");

	init();
}

ScaleGizmo::~ScaleGizmo()
{

}

void ScaleGizmo::setActivePlane(std::string axis, Vector3 eye)
{
	//ej2d::mat44 rot = this->planesMap["XY"]->get_world_mat();
	Matrix4 rot;
	rot = this->GetRotation().RotationMatrix();
	rot = rot.Inverse();
	Vector3 neweye;
	neweye = rot * eye;
	//rot.vec_mul(eye, neweye);
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

	if (axis == "XYZ")
		activePlane = planesMap["XYZE"];

}

void ScaleGizmo::createGizmos(std::string axisName)
{
	Vector3 axisAngle;
	Vector3 axisPos;
	unsigned axisColor;
	std::vector<Vector3> linePoints;
	Vector3 pickAxisPos;
	if (axisName == "X")
	{
		axisAngle = Vector3(0, 90, 0);
		axisPos = Vector3(0.8, 0, 0);
		axisColor = 0xff0000;
		linePoints = { Vector3(0,0,0),Vector3(0.8,0,0) };
		pickAxisPos = Vector3(0.3, 0, 0);
	}
	else if (axisName == "Y")
	{
		axisAngle = Vector3(-90, 0, 0);
		axisPos = Vector3(0, 0.8, 0);
		axisColor = 0x00ff00;
		linePoints = { Vector3(0,0,0),Vector3(0,0.8,0) };
		pickAxisPos = Vector3(0, 0.3, 0);
	}
	else if (axisName == "Z")
	{
		//axisAngle = vec3d(0, 0, -pi / 2);
		axisPos = Vector3(0, 0, 0.8);
		axisColor = 0x0000ff;
		linePoints = { Vector3(0,0,0),Vector3(0,0,0.8) };
		pickAxisPos = Vector3(0, 0, 0.3);
	}
	else if (axisName == "XYZ")
	{
		//axisAngle = vec3d(0, 0, -pi / 2);
		axisPos = Vector3(0, 0, 0);
		axisColor = 0xff00ff;
		pickAxisPos = Vector3(0, 0, 0);
	}
	//handles
	GizmogeoInfo* axisBoxInfo = new GizmogeoInfo();
	axisBoxInfo->geo = GeoUtils::create_cube(context_,0.05);
	//axisBoxInfo->geo->set_color(axisColor);
	GizmoUtils::set_default_mat(axisBoxInfo->geo);
	GizmoUtils::set_geo_color(axisBoxInfo->geo,0x77000000 + axisColor);
	axisBoxInfo->pos = axisPos;
	axisBoxInfo->rot = axisAngle;
	std::vector<GizmogeoInfo*> xGizmos;
	if (axisName != "XYZ")
	{
		GizmogeoInfo* axisLineInfo = new GizmogeoInfo();
		//auto xline = new lines_3d();
		SharedPtr<Node> xline(GeoUtils::create_line(context_,linePoints,axisColor));
        GizmoUtils::setLineMat(xline,axisColor);
		//xline->set_vertices(linePoints);
		//xline->set_color(axisColor);
		axisLineInfo->geo = xline;
		xGizmos.push_back(axisLineInfo);
	}
	xGizmos.push_back(axisBoxInfo);
	handleGizmos[axisName] = xGizmos;
	//picks
	GizmogeoInfo* axisPickInfo = new GizmogeoInfo();
	if (axisName != "XYZ")
	{
		axisPickInfo->geo = GeoUtils::create_cylinder(context_,0, 0.15, 0.6, 4, 1);
	}
	else
	{
		axisPickInfo->geo = GeoUtils::create_cube(context_,0.3);
	}
	//axisPickInfo->geo->set_color(axisColor);
	//axisPickInfo->geo->get_material()->set_blend_mode(blend_mode::NORMAL);
	//axisPickInfo->geo->set_alpha(0* 255);
	GizmoUtils::set_default_mat(axisPickInfo->geo);
	GizmoUtils::set_geo_color(axisPickInfo->geo,0x00000000 + axisColor);
	axisPickInfo->pos = pickAxisPos;
	axisPickInfo->rot = axisAngle;
	std::vector<GizmogeoInfo*> infoList;
	infoList.push_back(axisPickInfo);
	pickerGizmos[axisName] = infoList;

}