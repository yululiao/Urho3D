#pragma once
namespace Urho3D
{

class Node;

}

class GizmoUtils
{
public:
	static void set_default_mat(Urho3D::Node* geoNode);
	static void set_geo_color(Urho3D::Node* geoNode,unsigned value);
};
