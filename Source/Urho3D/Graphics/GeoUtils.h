#pragma once
#include <vector>
#include "Geometry.h"
#include "Model.h"


namespace Urho3D
{
	class Vector2;
	class Vector3;
	class Vector4;
	class GeoUtils
	{
	public:
		static bool close_enough(const float fCandidate, const float fCompare, const float fEpsilon)
		{
			return (fabs(fCandidate - fCompare) < fEpsilon);
		}
		static Node* create_plane(Context* ctx, float w, float h);
        static Node* create_cube(Context* ctx, float fRadius);
        static Node* create_torus(Context* ctx, float majorRadius, float minorRadius, int numMajor, int numMinor,float angle);
        static Node* create_sphere(Context* ctx, float fRadius, int iSlices, int iStacks);
        static Node* create_disk(Context* ctx, float innerRadius, float outerRadius, int nSlices, int nStacks);
        static Node* create_cylinder(Context* ctx, float baseRadius, float topRadius, float fLength, int numSlices,int numStacks);
        static Node* create_line(Context* ctx, std::vector<Vector3> vPos, unsigned color);
	private:
        static Node* create_model(std::vector<Vector3>& vPos, std::vector<Vector3>& vNormal,
                                  std::vector<Vector2>& vTex);
		static Context* _ctx;
		//static Model* create_cube(Context* ctx, float fRadius);
	};
}