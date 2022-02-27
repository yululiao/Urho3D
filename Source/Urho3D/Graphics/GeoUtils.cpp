#include "GeoUtils.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Color.h"
#include "StaticModel.h"

#include "Resource/ResourceCache.h"
#include "Material.h"
#include "Technique.h"


#define  M3D_PI 3.1415926


namespace Urho3D
{
	Context* GeoUtils::_ctx = NULL;

	 Node* GeoUtils::create_line(Context* ctx, std::vector<Vector3> vPos, unsigned color)
	{
		_ctx = ctx;
		int vCount = vPos.size();
		bool largeIndices = vCount > 65535;
		SharedPtr<VertexBuffer> dlvb(new VertexBuffer(_ctx));
		dlvb->SetShadowed(true);
		PODVector<VertexElement> ret;
		ret.Push(VertexElement(TYPE_VECTOR3, SEM_POSITION));
		//dlvb->SetSize(vCount, MASK_POSITION);
		dlvb->SetSize(vCount, ret);
		auto vItemSize = dlvb->GetVertexSize()/sizeof(float);;
		float* dirLightVertexData = new float[vItemSize * vCount];
		int vDataIndex = 0;
		color = 0xff000000 + color;
		Color t_color(color, Color::ARGB);

		BoundingBox box;
		for (int i = 0; i < vCount; i++)
		{
			box.Merge(vPos[i]);
			dirLightVertexData[vDataIndex++] = vPos[i].x_;
			dirLightVertexData[vDataIndex++] = vPos[i].y_;
			dirLightVertexData[vDataIndex++] = vPos[i].z_;	
		}
		dlvb->SetData(dirLightVertexData);

		SharedPtr<IndexBuffer> dlib(new IndexBuffer(_ctx));
		dlib->SetShadowed(true);
		dlib->SetSize(vCount, false);
		if (largeIndices)
		{
			unsigned* dirLightIndexData = new unsigned[vCount];
			unsigned iDataIndex = 0;
			for (unsigned i = 0; i < vCount; i++)
			{
				dirLightIndexData[iDataIndex++] = i;
			}
			dlib->SetData(dirLightIndexData);
		}
		else
		{
			unsigned short* dirLightIndexData = new unsigned short[vCount];
			unsigned iDataIndex = 0;
			for (unsigned short i = 0; i < vCount; i++)
			{
				dirLightIndexData[iDataIndex++] = i;
			}
			dlib->SetData(dirLightIndexData);
		}


		Geometry *dirLightGeometry_ = new Geometry(_ctx);
		dirLightGeometry_->SetVertexBuffer(0, dlvb);
		dirLightGeometry_->SetIndexBuffer(dlib);
		dirLightGeometry_->SetDrawRange(LINE_STRIP, 0, dlib->GetIndexCount());

		SharedPtr<Model> testModel(new Model(_ctx));
		Vector<SharedPtr<VertexBuffer> > dlvbVector;
		Vector<SharedPtr<IndexBuffer> > dlibVector;
		dlvbVector.Push(dlvb);
		dlibVector.Push(dlib);
		testModel->SetNumGeometries(1);
		testModel->SetNumGeometryLodLevels(0, 1);
		testModel->SetGeometry(0, 0, dirLightGeometry_);

		// Define the model buffers and bounding box
		PODVector<unsigned> emptyMorphRange;
		testModel->SetVertexBuffers(dlvbVector, emptyMorphRange, emptyMorphRange);
		testModel->SetIndexBuffers(dlibVector);
		testModel->SetBoundingBox(box);

		//SharedPtr<Node> t_node(new Node(_ctx));
		Node* t_node = new Node(_ctx);
		auto* s_mode = t_node->CreateComponent<StaticModel>();
		s_mode->SetModel(testModel);

		auto* cache = t_node->GetSubsystem<ResourceCache>();
		SharedPtr<Material> lineMat = cache->GetResource<Material>("Materials/LineMat.xml")->Clone();
        //lineMat->GetPass(0, "Techniques/NoTextureUnlitAlpha.xml")->SetDepthTestMode(Urho3D::CompareMode::CMP_ALWAYS);
		lineMat->SetShaderParameter("MatDiffColor", t_color.ToVector4());
		s_mode->SetMaterial(lineMat);
		
		return t_node;

	}

	Node* GeoUtils::create_model(std::vector<Vector3>& vPos, std::vector<Vector3>& vNormal, std::vector<Vector2>& vTex)
	{
		int vCount = vPos.size();
		bool largeIndices = vCount > 65535;
		SharedPtr<VertexBuffer> dlvb(new VertexBuffer(_ctx));
		dlvb->SetShadowed(true);
		PODVector<VertexElement> ret;
		ret.Push(VertexElement(TYPE_VECTOR3, SEM_POSITION));
		ret.Push(VertexElement(TYPE_VECTOR3, SEM_NORMAL));
		ret.Push(VertexElement(TYPE_VECTOR2, SEM_TEXCOORD));
		//dlvb->SetSize(vCount, MASK_POSITION);
		dlvb->SetSize(vCount, ret);
		auto vItemSize = dlvb->GetVertexSize()/sizeof(float);
		float* dirLightVertexData = new float[vItemSize * vCount];
		int vDataIndex = 0;
		BoundingBox box;
		for (int i = 0; i < vCount; i++)
		{
			box.Merge(vPos[i]);
			dirLightVertexData[vDataIndex++] = vPos[i].x_;
			dirLightVertexData[vDataIndex++] = vPos[i].y_;
			dirLightVertexData[vDataIndex++] = vPos[i].z_;

			dirLightVertexData[vDataIndex++] = vNormal[i].x_;
			dirLightVertexData[vDataIndex++] = vNormal[i].y_;
			dirLightVertexData[vDataIndex++] = vNormal[i].z_;

			dirLightVertexData[vDataIndex++] = vTex[i].x_;
			dirLightVertexData[vDataIndex++] = vTex[i].y_;

		}
		dlvb->SetData(dirLightVertexData);

		SharedPtr<IndexBuffer> dlib(new IndexBuffer(_ctx));
		dlib->SetShadowed(true);
		dlib->SetSize(vCount, false);
		if (largeIndices)
		{
			unsigned* dirLightIndexData = new unsigned[vCount];
			unsigned iDataIndex = 0;
			for (unsigned i = 0; i < vCount; i++)
			{
				dirLightIndexData[iDataIndex++] = i;
			}
			dlib->SetData(dirLightIndexData);
		}
		else
		{
			unsigned short* dirLightIndexData = new unsigned short[vCount];
			unsigned iDataIndex = 0;
			for (unsigned short i = 0; i < vCount; i++)
			{
				dirLightIndexData[iDataIndex++] = i;
			}
			dlib->SetData(dirLightIndexData);
		}


		Geometry *dirLightGeometry_ = new Geometry(_ctx);
		dirLightGeometry_->SetVertexBuffer(0, dlvb);
		dirLightGeometry_->SetIndexBuffer(dlib);
		dirLightGeometry_->SetDrawRange(TRIANGLE_LIST, 0, dlib->GetIndexCount());

		SharedPtr<Model> testModel(new Model(_ctx));
		Vector<SharedPtr<VertexBuffer> > dlvbVector;
		Vector<SharedPtr<IndexBuffer> > dlibVector;
		dlvbVector.Push(dlvb);
		dlibVector.Push(dlib);
		testModel->SetNumGeometries(1);
		testModel->SetNumGeometryLodLevels(0, 1);
		testModel->SetGeometry(0, 0, dirLightGeometry_);

		// Define the model buffers and bounding box
		PODVector<unsigned> emptyMorphRange;
		testModel->SetVertexBuffers(dlvbVector, emptyMorphRange, emptyMorphRange);
		testModel->SetIndexBuffers(dlibVector);
		testModel->SetBoundingBox(box);

		//SharedPtr<Node> t_node(new Node(_ctx));
        Node* t_node = new Node(_ctx);
		auto* s_mode = t_node->CreateComponent<StaticModel>();
		s_mode->SetModel(testModel);
		return t_node;

	}

	Node* GeoUtils::create_plane(Context* ctx, float w, float h)
	{
		_ctx = ctx;
		std::vector<Vector3> ori_pos;
		std::vector<Vector2> ori_tex;
		w = w / 2.0;
		h = h / 2.0;
		ori_pos.push_back(Vector3(-w,h,0));
		ori_pos.push_back(Vector3(w, h,0));
		ori_pos.push_back(Vector3(w, -h,0));
		ori_pos.push_back(Vector3(-w, -h,0));

		ori_tex.push_back(Vector2(0,1));
		ori_tex.push_back(Vector2(1,1));
		ori_tex.push_back(Vector2(1, 0));
		ori_tex.push_back(Vector2(0, 0));


		std::vector<Vector3> vPos;
		std::vector<Vector3> vNormal;
		std::vector<Vector2> vTex;

		vPos.push_back(ori_pos[0]);
		vPos.push_back(ori_pos[1]);
		vPos.push_back(ori_pos[2]);
		
		vPos.push_back(ori_pos[2]);
		vPos.push_back(ori_pos[3]);
		vPos.push_back(ori_pos[0]);

		vNormal.push_back(Vector3(0, 0, 1));
		vNormal.push_back(Vector3(0, 0, 1));
		vNormal.push_back(Vector3(0, 0, 1));
		vNormal.push_back(Vector3(0, 0, 1));
		vNormal.push_back(Vector3(0, 0, 1));
		vNormal.push_back(Vector3(0, 0, 1));

		vTex.push_back(ori_tex[0]);
		vTex.push_back(ori_tex[1]);
		vTex.push_back(ori_tex[2]);

		vTex.push_back(ori_tex[2]);
		vTex.push_back(ori_tex[3]);
		vTex.push_back(ori_tex[0]);

		return create_model(vPos, vNormal, vTex);

	}

	Node* GeoUtils::create_torus(Context* ctx, float majorRadius, float minorRadius, int numMajor, int numMinor,float angle)
	{
		_ctx = ctx;
		/*double majorStep = 2.0f*M3D_PI / numMajor;
		double minorStep = 2.0f*M3D_PI / numMinor;*/
		double majorStep = angle / numMajor;
		double minorStep = angle / numMinor;
		int i, j;
		std::vector<Vector3> posList;
		std::vector<Vector3> normalList;
		std::vector<Vector2> texList;
		for (i = 0; i < numMajor; ++i)
		{
			double a0 = i * majorStep;
			double a1 = a0 + majorStep;
			float x0 = (float)cos(a0);
			float y0 = (float)sin(a0);
			float x1 = (float)cos(a1);
			float y1 = (float)sin(a1);
			Vector3 vVertex[4];
			Vector3 vNormal[4];
			Vector2 vTexture[4];
			for (j = 0; j <= numMinor; ++j)
			{
				double b = j * minorStep;
				float c = (float)cos(b);
				float r = minorRadius * c + majorRadius;
				float z = minorRadius * (float)sin(b);

				// First point
				vTexture[0].x_ = (float)(i) / (float)(numMajor);
				vTexture[0].y_ = (float)(j) / (float)(numMinor);
				vNormal[0].x_ = x0 * c;
				vNormal[0].y_ = y0 * c;
				vNormal[0].z_ = z / minorRadius;
				vNormal[0].Normalize();
				vVertex[0].x_ = x0 * r;
				vVertex[0].y_ = y0 * r;
				vVertex[0].z_ = z;

				// Second point
				vTexture[1].x_ = (float)(i + 1) / (float)(numMajor);
				vTexture[1].y_ = (float)(j) / (float)(numMinor);
				vNormal[1].x_ = x1 * c;
				vNormal[1].y_ = y1 * c;
				vNormal[1].z_ = z / minorRadius;
				vNormal[1].Normalize();
				vVertex[1].x_ = x1 * r;
				vVertex[1].y_ = y1 * r;
				vVertex[1].z_ = z;

				// Next one over
				b = (j + 1) * minorStep;
				c = (float)cos(b);
				r = minorRadius * c + majorRadius;
				z = minorRadius * (float)sin(b);

				// Third (based on first)
				vTexture[2].x_ = (float)(i) / (float)(numMajor);
				vTexture[2].y_ = (float)(j + 1) / (float)(numMinor);
				vNormal[2].x_ = x0 * c;
				vNormal[2].y_ = y0 * c;
				vNormal[2].z_ = z / minorRadius;
				vNormal[2].Normalize();
				vVertex[2].x_ = x0 * r;
				vVertex[2].y_ = y0 * r;
				vVertex[2].z_ = z;

				// Fourth (based on second)
				vTexture[3].x_ = (float)(i + 1) / (float)(numMajor);
				vTexture[3].y_ = (float)(j + 1) / (float)(numMinor);
				vNormal[3].x_ = x1 * c;
				vNormal[3].y_ = y1 * c;
				vNormal[3].z_ = z / minorRadius;
				vNormal[3].Normalize();
				vVertex[3].x_ = x1 * r;
				vVertex[3].y_ = y1 * r;
				vVertex[3].z_ = z;

				posList.push_back(vVertex[0]);
				normalList.push_back(vNormal[0]);
				texList.push_back(vTexture[0]);
				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);

				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[3]);
				normalList.push_back(vNormal[3]);
				texList.push_back(vTexture[3]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);
			}
		}
		
		return create_model(posList,normalList,texList);

	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Make a sphere
    Node* GeoUtils::create_sphere(Context* ctx, float fRadius, int iSlices, int iStacks)
	{
		_ctx = ctx;
		std::vector<Vector3> posList;
		std::vector<Vector3> normalList;
		std::vector<Vector2> texList;
		float drho = (float)(3.141592653589) / (float)iStacks;
		float dtheta = 2.0f * (float)(3.141592653589) / (float)iSlices;
		float ds = 1.0f / (float)iSlices;
		float dt = 1.0f / (float)iStacks;
		float t = 1.0f;
		float s = 0.0f;
		int i, j;     // Looping variables

		//sphereBatch.BeginMesh(iSlices * iStacks * 6);
		int vCount = iSlices * iStacks * 6;

		for (i = 0; i < iStacks; i++)
		{
			float rho = (float)i * drho;
			float srho = (float)(sin(rho));
			float crho = (float)(cos(rho));
			float srhodrho = (float)(sin(rho + drho));
			float crhodrho = (float)(cos(rho + drho));

			// Many sources of OpenGL sphere drawing code uses a triangle fan
			// for the caps of the sphere. This however introduces texturing 
			// artifacts at the poles on some OpenGL implementations
			s = 0.0f;
			Vector3 vVertex[4];
			Vector3 vNormal[4];
			Vector2 vTexture[4];

			for (j = 0; j < iSlices; j++)
			{
				float theta = (j == iSlices) ? 0.0f : j * dtheta;
				float stheta = (float)(-sin(theta));
				float ctheta = (float)(cos(theta));

				float x = stheta * srho;
				float y = ctheta * srho;
				float z = crho;

				vTexture[0].x_ = s;
				vTexture[0].y_ = t;
				vNormal[0].x_ = x;
				vNormal[0].y_ = y;
				vNormal[0].x_ = z;
				vVertex[0].x_ = x * fRadius;
				vVertex[0].y_ = y * fRadius;
				vVertex[0].z_ = z * fRadius;

				x = stheta * srhodrho;
				y = ctheta * srhodrho;
				z = crhodrho;

				vTexture[1].x_ = s;
				vTexture[1].y_= t - dt;
				vNormal[1].x_ = x;
				vNormal[1].y_ = y;
				vNormal[1].z_ = z;
				vVertex[1].x_ = x * fRadius;
				vVertex[1].y_ = y * fRadius;
				vVertex[1].z_ = z * fRadius;


				theta = ((j + 1) == iSlices) ? 0.0f : (j + 1) * dtheta;
				stheta = (float)(-sin(theta));
				ctheta = (float)(cos(theta));

				x = stheta * srho;
				y = ctheta * srho;
				z = crho;

				s += ds;
				vTexture[2].x_ = s;
				vTexture[2].y_ = t;
				vNormal[2].x_= x;
				vNormal[2].y_ = y;
				vNormal[2].z_= z;
				vVertex[2].x_= x * fRadius;
				vVertex[2].y_= y * fRadius;
				vVertex[2].z_= z * fRadius;

				x = stheta * srhodrho;
				y = ctheta * srhodrho;
				z = crhodrho;

				vTexture[3].x_ = s;
				vTexture[3].y_ = t - dt;
				vNormal[3].x_= x;
				vNormal[3].y_ = y;
				vNormal[3].z_= z;
				vVertex[3].x_ = x * fRadius;
				vVertex[3].y_ =  y * fRadius;
				vVertex[3].z_ = z * fRadius;

				posList.push_back(vVertex[0]);
				normalList.push_back(vNormal[0]);
				texList.push_back(vTexture[0]);
				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);

				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[3]);
				normalList.push_back(vNormal[3]);
				texList.push_back(vTexture[3]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);
			}
			t -= dt;
		}

		return create_model(posList, normalList, texList);
	}


	//////////////////////////////////////////////////////////////////////////////////////////
    Node* GeoUtils::create_disk(Context* ctx, float innerRadius, float outerRadius, int nSlices, int nStacks)
	{
		_ctx = ctx;
		std::vector<Vector3> posList;
		std::vector<Vector3> normalList;
		std::vector<Vector2> texList;
		// How much to step out each stack
		float fStepSizeRadial = outerRadius - innerRadius;
		if (fStepSizeRadial < 0.0f)			// Dum dum...
			fStepSizeRadial *= -1.0f;

		fStepSizeRadial /= float(nStacks);

		float fStepSizeSlice = (3.1415926536f * 2.0f) / float(nSlices);

		//diskBatch.BeginMesh(nSlices * nStacks * 6);
		int vCount = nSlices * nStacks * 6;
		Vector3 vVertex[4];
		Vector3 vNormal[4];
		Vector2 vTexture[4];

		float fRadialScale = 1.0f / outerRadius;

		for (int i = 0; i < nStacks; i++)			// Stacks
		{
			float theyta;
			float theytaNext;
			for (int j = 0; j < nSlices; j++)     // Slices
			{
				float inner = innerRadius + (float(i)) * fStepSizeRadial;
				float outer = innerRadius + (float(i + 1)) * fStepSizeRadial;

				theyta = fStepSizeSlice * float(j);
				if (j == (nSlices - 1))
					theytaNext = 0.0f;
				else
					theytaNext = fStepSizeSlice * (float(j + 1));

				// Inner First
				vVertex[0].x_ = cos(theyta) * inner;	// X	
				vVertex[0].y_ = sin(theyta) * inner;	// Y
				vVertex[0].z_ = 0.0f;					// Z

				vNormal[0].x_ = 0.0f;					// Surface Normal, same for everybody
				vNormal[0].y_ = 0.0f;
				vNormal[0].z_ = 1.0f;

				vTexture[0].x_ = ((vVertex[0].x_ * fRadialScale) + 1.0f) * 0.5f;
				vTexture[0].y_ = ((vVertex[0].y_ * fRadialScale) + 1.0f) * 0.5f;

				// Outer First
				vVertex[1].x_ = cos(theyta) * outer;	// X	
				vVertex[1].y_ = sin(theyta) * outer;	// Y
				vVertex[1].z_ = 0.0f;					// Z

				vNormal[1].x_ = 0.0f;					// Surface Normal, same for everybody
				vNormal[1].y_ = 0.0f;
				vNormal[1].z_ = 1.0f;

				vTexture[1].x_ = ((vVertex[1].x_ * fRadialScale) + 1.0f) * 0.5f;
				vTexture[1].y_ = ((vVertex[1].y_ * fRadialScale) + 1.0f) * 0.5f;

				// Inner Second
				vVertex[2].x_ = cos(theytaNext) * inner;	// X	
				vVertex[2].y_ = sin(theytaNext) * inner;	// Y
				vVertex[2].z_ = 0.0f;					// Z

				vNormal[2].x_ = 0.0f;					// Surface Normal, same for everybody
				vNormal[2].y_ = 0.0f;
				vNormal[2].z_ = 1.0f;

				vTexture[2].x_ = ((vVertex[2].x_ * fRadialScale) + 1.0f) * 0.5f;
				vTexture[2].y_ = ((vVertex[2].y_ * fRadialScale) + 1.0f) * 0.5f;


				// Outer Second
				vVertex[3].x_ = cos(theytaNext) * outer;	// X	
				vVertex[3].y_ = sin(theytaNext) * outer;	// Y
				vVertex[3].z_ = 0.0f;					// Z

				vNormal[3].x_ = 0.0f;					// Surface Normal, same for everybody
				vNormal[3].y_ = 0.0f;
				vNormal[3].z_ = 1.0f;

				vTexture[3].x_ = ((vVertex[3].x_ * fRadialScale) + 1.0f) * 0.5f;
				vTexture[3].y_ = ((vVertex[3].y_ * fRadialScale) + 1.0f) * 0.5f;

				posList.push_back(vVertex[0]);
				normalList.push_back(vNormal[0]);
				texList.push_back(vTexture[0]);
				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);

				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[3]);
				normalList.push_back(vNormal[3]);
				texList.push_back(vTexture[3]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);
			}
		}
		return create_model(posList,normalList,texList);
	}

	//// Draw a cylinder. Much like gluCylinder
    Node* GeoUtils::create_cylinder(Context* ctx, float baseRadius, float topRadius,
		float fLength, int numSlices, int numStacks)
	{
		_ctx = ctx;
		std::vector<Vector3> posList;
		std::vector<Vector3> normalList;
		std::vector<Vector2> texList;

		float fRadiusStep = (topRadius - baseRadius) / float(numStacks);
		float fStepSizeSlice = (3.1415926536f * 2.0f) / float(numSlices);

		Vector3 vVertex[4];
		Vector3 vNormal[4];
		Vector2 vTexture[4];

		//cylinderBatch.BeginMesh(numSlices * numStacks * 6);
		int vCount = numSlices * numStacks * 6;

		float ds = 1.0f / float(numSlices);
		float dt = 1.0f / float(numStacks);
		float s;
		float t;

		for (int i = 0; i < numStacks; i++)
		{
			if (i == 0)
				t = 0.0f;
			else
				t = float(i) * dt;

			float tNext;
			if (i == (numStacks - 1))
				tNext = 1.0f;
			else
				tNext = float(i + 1) * dt;

			float fCurrentRadius = baseRadius + (fRadiusStep * float(i));
			float fNextRadius = baseRadius + (fRadiusStep * float(i + 1));
			float theyta;
			float theytaNext;

			float fCurrentZ = float(i) * (fLength / float(numStacks));
			float fNextZ = float(i + 1) * (fLength / float(numStacks));

			float zNormal = 0.0f;
			if (!close_enough(baseRadius - topRadius, 0.0f, 0.00001f))
			{
				// Rise over run...
				zNormal = (baseRadius - topRadius);
			}

			for (int j = 0; j < numSlices; j++)
			{
				if (j == 0)
					s = 0.0f;
				else
					s = float(j) * ds;

				float sNext;
				if (j == (numSlices - 1))
					sNext = 1.0f;
				else
					sNext = float(j + 1) * ds;

				theyta = fStepSizeSlice * float(j);
				if (j == (numSlices - 1))
					theytaNext = 0.0f;
				else
					theytaNext = fStepSizeSlice * (float(j + 1));

				// Inner First
				vVertex[1].x_ = cos(theyta) * fCurrentRadius;	// X	
				vVertex[1].y_= sin(theyta) * fCurrentRadius;	// Y
				vVertex[1].z_= fCurrentZ;						// Z

				vNormal[1].x_ = vVertex[1].x_;					// Surface Normal, same for everybody
				vNormal[1].y_ = vVertex[1].y_;
				vNormal[1].z_ = zNormal;
				//m3dNormalizeVector3(vNormal[1]);
				vNormal[1].Normalize();

				vTexture[1].x_ = s;					// Texture Coordinates, I have no idea...
				vTexture[1].y_ = t;

				// Outer First
				vVertex[0].x_ = cos(theyta) * fNextRadius;	// X	
				vVertex[0].y_ = sin(theyta) * fNextRadius;	// Y
				vVertex[0].z_ = fNextZ;						// Z

				if (!close_enough(fNextRadius, 0.0f, 0.00001f)) {
					vNormal[0].x_ = vVertex[0].x_;					// Surface Normal, same for everybody
					vNormal[0].y_ = vVertex[0].y_;					// For cones, tip is tricky
					vNormal[0].z_ = zNormal;
					//m3dNormalizeVector3(vNormal[0]);
					vNormal[0].Normalize();
				}
				else
					vNormal[0] = vNormal[1];
					//memcpy(vNormal[0], vNormal[1], sizeof(M3DVector3f));


				vTexture[0].x_ = s;					// Texture Coordinates, I have no idea...
				vTexture[0].y_ = tNext;

				// Inner second
				vVertex[3].x_ = cos(theytaNext) * fCurrentRadius;	// X	
				vVertex[3].y_= sin(theytaNext) * fCurrentRadius;	// Y
				vVertex[3].z_ = fCurrentZ;						// Z

				vNormal[3].x_ = vVertex[3].x_;					// Surface Normal, same for everybody
				vNormal[3].y_ = vVertex[3].y_;
				vNormal[3].z_ = zNormal;
				//m3dNormalizeVector3(vNormal[3]);
				vNormal[3].Normalize();

				vTexture[3].x_ = sNext;					// Texture Coordinates, I have no idea...
				vTexture[3].y_ = t;

				// Outer second
				vVertex[2].x_ = cos(theytaNext) * fNextRadius;	// X	
				vVertex[2].y_ = sin(theytaNext) * fNextRadius;	// Y
				vVertex[2].z_ = fNextZ;						// Z

				if (!close_enough(fNextRadius, 0.0f, 0.00001f)) {
					vNormal[2].x_ = vVertex[2].x_;					// Surface Normal, same for everybody
					vNormal[2].y_ = vVertex[2].y_;
					vNormal[2].z_ = zNormal;
					//m3dNormalizeVector3(vNormal[2]);
					vNormal[2].Normalize();
				}
				else
					vNormal[2] = vNormal[3];
					//memcpy(vNormal[2], vNormal[3], sizeof(vec));

				vTexture[2].x_ = sNext;					// Texture Coordinates, I have no idea...
				vTexture[2].y_ = tNext;

				posList.push_back(vVertex[0]);
				normalList.push_back(vNormal[0]);
				texList.push_back(vTexture[0]);
				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);

				posList.push_back(vVertex[1]);
				normalList.push_back(vNormal[1]);
				texList.push_back(vTexture[1]);
				posList.push_back(vVertex[3]);
				normalList.push_back(vNormal[3]);
				texList.push_back(vTexture[3]);
				posList.push_back(vVertex[2]);
				normalList.push_back(vNormal[2]);
				texList.push_back(vTexture[2]);

			}
		}

		return create_model(posList, normalList, texList);
	}


	/////////////////////////////////////////////////////////////////////////////////////////
	// Make a cube, centered at the origin, and with a specified "radius"
    Node* GeoUtils::create_cube(Context* ctx, float fRadius)
	{
		_ctx = ctx;
		std::vector<Vector3> posList;
		std::vector<Vector3> normalList;
		std::vector<Vector2> texList;
		//cubeBatch.Begin(GL_TRIANGLES, 36, 1);
		Geometry* geo = new Geometry(ctx);
		Model* outModel = new Model(ctx);
		//torusBatch.BeginMesh(numMajor * (numMinor + 1) * 6);
		int vCount = 36;
		bool largeIndices = vCount > 65535;
		VertexBuffer* vb = new VertexBuffer(ctx);
		IndexBuffer* ib = new IndexBuffer(ctx);
		ib->SetSize(vCount, largeIndices);
		vb->SetShadowed(true);
		ib->SetShadowed(true);
		PODVector<VertexElement> ret;
		ret.Push(VertexElement(TYPE_VECTOR3, SEM_POSITION));
		ret.Push(VertexElement(TYPE_VECTOR3, SEM_NORMAL));
		ret.Push(VertexElement(TYPE_VECTOR2, SEM_TEXCOORD));
		vb->SetSize(vCount, ret);
		unsigned char* vertexData = vb->GetShadowData();
		unsigned char* indexData = ib->GetShadowData();
		auto* dest = (float*)((unsigned char*)vertexData);

		for (unsigned ti = 0; ti < vCount; ti++)
		{
			unsigned short* dest_i = (unsigned short*)indexData;
			*dest_i++ = ti;
		}

		/////////////////////////////////////////////
		// Top of cube
		normalList.push_back(Vector3(0.0f, fRadius, 0.0f));
		texList.push_back(Vector2(fRadius, fRadius));
		posList.push_back(Vector3(fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, fRadius, 0.0f));
		texList.push_back(Vector2(fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, fRadius, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, fRadius, 0.0f));
		texList.push_back(Vector2(fRadius, fRadius));
		posList.push_back(Vector3(fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, fRadius, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, fRadius, 0.0f));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, fRadius));


		////////////////////////////////////////////
		// Bottom of cube
		normalList.push_back(Vector3(0.0f, -fRadius, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, -fRadius, 0.0f));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, -fRadius, 0.0f));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(fRadius, -fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, -fRadius, 0.0f));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, -fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, -fRadius, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, -fRadius, 0.0f));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(fRadius, -fRadius, fRadius));

		///////////////////////////////////////////
		// Left side of cube
		normalList.push_back(Vector3(-fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(-fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(-fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(-fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(-fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(-fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(-fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, -fRadius, fRadius));

		// Right side of cube
		normalList.push_back(Vector3(fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(fRadius, -fRadius, fRadius));

		normalList.push_back(Vector3(fRadius, 0.0f, 0.0f));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, -fRadius));

		// Front and Back
		// Front
		normalList.push_back(Vector3(0.0f, 0.0f, fRadius));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, fRadius));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, fRadius));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, fRadius));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, fRadius));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, -fRadius, fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, fRadius));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, fRadius));

		// Back
		normalList.push_back(Vector3(0.0f, 0.0f, -fRadius));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, -fRadius));
		texList.push_back(Vector2( 0.0f, 0.0f));
		posList.push_back(Vector3(-fRadius, -fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, -fRadius));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, -fRadius));
		texList.push_back(Vector2( 0.0f, fRadius));
		posList.push_back(Vector3(-fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, -fRadius));
		texList.push_back(Vector2( fRadius, fRadius));
		posList.push_back(Vector3(fRadius, fRadius, -fRadius));

		normalList.push_back(Vector3(0.0f, 0.0f, -fRadius));
		texList.push_back(Vector2( fRadius, 0.0f));
		posList.push_back(Vector3(fRadius, -fRadius, -fRadius));

		return create_model(posList, normalList, texList);
	}

}