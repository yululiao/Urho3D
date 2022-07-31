#include <string>
#include "SceneCtrl.h"
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Graphics/GeoUtils.h>
#include <Urho3D/Graphics/Texture2D.h>
#include "Urho3D/Graphics/Geometry.h"
#include "Urho3D//Graphics/VertexBuffer.h"
#include "Urho3D/Graphics/IndexBuffer.h"
#include "Urho3D/Math/Ray.h"
#include "AssetMgr.h"
#include "Urho3D/IO/FileSystem.h"
#include "EditorApp.h"
//#include "ctrl/utils.h"
//#include "ctrl/asset_mgr.h"
//#include "ctrl/global_event.h"

namespace Urho3DEditor
{

	SceneCtrl* SceneCtrl::_instance = nullptr;
	Context* SceneCtrl::_ctx = nullptr;
	SceneCtrl* SceneCtrl::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new SceneCtrl(_ctx);
		}
		return _instance;
	}

	SceneCtrl::SceneCtrl(Context* ctx)
		:Object(ctx)
    {
        viewSize_ = Vector2(400,300);
        constRttSize = Vector2(1920,1080);
	}

	SceneCtrl::~SceneCtrl()
	{

	}

    void SceneCtrl::deleteNode(Urho3D::Node* node) 
    { 
        node->Remove();
    }

    void SceneCtrl::addModel(const String& path)
    {
        GetSubsystem<Graphics>()->MakeCurrent();
        auto* cache = GetSubsystem<ResourceCache>();
        String name = AssetMgr::getInstance()->getBaseName(path); // Utils::get_base_name(path);
        //加载fbx对应的mdl
        String mdl_path = AssetMgr::getInstance()->getFilePath(path) + "/" + name + ".mdl";
        String rpath = AssetMgr::getInstance()->pathToRelative(mdl_path);
        Node* modelNode = rttSceneRoot_->CreateChild(name);
        modelNode->SetScale(Vector3(0.01,0.01,0.01));
        auto* modelObject = modelNode->CreateComponent<AnimatedModel>();
        Model* model = cache->GetResource<Model>(rpath);
        modelObject->SetModel(model);
        SharedPtr<Material> defMat(cache->GetResource<Material>("Materials/Default.xml"));
        modelObject->SetMaterial(defMat);
    }

    void SceneCtrl::genRttTex() 
    { 
        if (!renderTexture)
            return;
        if (!rttData)
        {
            rttData = new char[constRttSize.x_* constRttSize.y_*3];
        }
        renderTexture->GetData(0,rttData);
       //renderTexture->GetImage()->SavePNG("test.png");
    }

    void SceneCtrl::OnResizeView(int w, int h) 
    { 
        Vector2 newSize(w, h);
        if (newSize == viewSize_)
        {
            return;
        }
        viewSize_ = newSize;
        //rttCam_->SetAspectRatio(viewSize_.x_/viewSize_.y_);
        rttCam_->SetAspectRatioInternal(viewSize_.x_ / viewSize_.y_);
    }

    Node* SceneCtrl::intersectObj(Ray& ray, Node* pnode, float& dis)
    {
        Node* result = nullptr;
        std::map<float, Node*> out;
        intersectObj(ray, pnode, out);
        if (out.size() == 0)
            return result;
        result = out.begin()->second;
        dis = out.begin()->first;
        return result;
    }

    void SceneCtrl::intersectObj(Ray& ray, Node* pnode, std::map<float, Node*>& out)
    {
        StaticModel* stmodel = pnode->GetComponent<StaticModel>();
        AnimatedModel* animodel = pnode->GetComponent<AnimatedModel>();
        Model* tmodel = nullptr;
        if (stmodel)
            tmodel = stmodel->GetModel();
        if (animodel)
            tmodel = animodel->GetModel();
        if (tmodel)
        {
            unsigned int geoNum = tmodel->GetNumGeometries();
            float dis = M_INFINITY;
            //std::string name = pnode->GetName().CString();
            for (int i = 0; i < geoNum; i++)
            {
                Geometry* geo = tmodel->GetGeometry(i, 0);
                VertexBuffer* vb = geo->GetVertexBuffer(0);
                unsigned vitemsize = vb->GetVertexSize();
                void* vbdata = vb->GetShadowData();
                void* vidata = geo->GetIndexBuffer()->GetShadowData();
                unsigned indexSize = geo->GetIndexBuffer()->GetIndexSize();
                unsigned indexCount = geo->GetIndexBuffer()->GetIndexCount();
                Matrix3x4 nodeRot = pnode->GetWorldTransform();
                Ray t_ray = ray;
                t_ray.hitTransform_ = nodeRot;
                float t_dis = t_ray.HitDistance(vbdata, vitemsize, vidata, indexSize, 0, indexCount);
                if (t_dis < dis)
                {
                    dis = t_dis;
                }
            }
            if (dis < M_INFINITY && out.find(dis) == out.end())
            {
                out[dis] = pnode;
            }
        }
        auto& children = pnode->GetChildren();
        for (auto it = children.Begin(); it != children.End(); it++)
        {
            intersectObj(ray, *it, out);
        }
    }

    void SceneCtrl::update_grids()
    {
        _grid_lines.clear();
        if(_grid_root.Null())
        {
            _grid_root = editorRoot_->CreateChild("GridRoot");
        }
        int line_count = 20;
        float line_dis = 1;
        //横向
        float min = -(line_count/2.0/line_dis);
        float max = (line_count/2.0/line_dis);
        for(int i=0;i<=line_count;i++)
        {
            float z = min + line_dis * i;
            std::vector<Vector3> linePos;
            linePos.push_back(Vector3(min,0,z));
            linePos.push_back(Vector3(max,0,z));
            SharedPtr<Node> lineNode(GeoUtils::create_line(context_, linePos, 0xaaaaaa));
            _grid_root->AddChild(lineNode);

        }
        //纵向
        for(int i=0;i<=line_count;i++)
        {
            float x = min + line_dis * i;
            std::vector<Vector3> linePos;
            linePos.push_back(Vector3(x,0,min));
            linePos.push_back(Vector3(x,0,max));
            SharedPtr<Node> lineNode(GeoUtils::create_line(context_, linePos, 0xaaaaaa));
            _grid_root->AddChild(lineNode);
        }

    }

    void SceneCtrl::Clear() 
    { 
        EditorApp::getInstance()->Clear();
        _grid_root = nullptr;
        rttSceneRoot_ = nullptr;
    }

    void SceneCtrl::InitScene(bool hasRoot)
    {
        if(hasRoot)
        {
            rttSceneRoot_ = rttScene_->GetChild("Root");
        }
        else
        {
            rttSceneRoot_ = rttScene_->CreateChild("Root");
            Node* zoneNode = rttSceneRoot_->CreateChild("Zone");
            auto* zone = zoneNode->CreateComponent<Zone>();
            zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
            zone->SetAmbientColor(Color(0.2f, 0.2f, 0.2f));
            zone->SetFogColor(Color(0.2f, 0.2f, 0.2f));
            zone->SetFogStart(10.0f);
            zone->SetFogEnd(100.0f);

            Node* lightNode = rttSceneRoot_->CreateChild("DirLight");
            lightNode->SetPosition(Vector3(5.0f, 5.0f, -5.0f));
            lightNode->LookAt(Vector3(0.0f, 0.0f, -0.0f));
            auto* light = lightNode->CreateComponent<Light>();
            light->SetLightType(LIGHT_DIRECTIONAL);
            light->SetColor(Color(0.8f, 0.8f, 0.8f));
            light->SetSpecularIntensity(1.0f);
        }
        editorRoot_ = rttScene_->CreateChild("EditorRoot");
        editorRoot_->SetNeedSave(false);

        rttCameraNode_ = editorRoot_->CreateChild("Camera");
        rttCameraNode_->SetPosition(Vector3(5.0f, 5.0f, -5.0f));
        rttCameraNode_->LookAt(Vector3(0.0f, 0.0f, -0.0f));
        rttCam_ = rttCameraNode_->CreateComponent<Camera>();
        rttCam_->SetFarClip(300.0f);
        rttCam_->SetAutoAspectRatio(false);
        rttCam_->SetAspectRatio(viewSize_.x_ / viewSize_.y_);
        renderTexture = new Texture2D(context_);
        renderTexture->SetSize(constRttSize.x_, constRttSize.y_, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
        renderTexture->SetFilterMode(FILTER_BILINEAR);
        RenderSurface* surface = renderTexture->GetRenderSurface();
        surface->SetUpdateMode(SURFACE_UPDATEALWAYS);
        rttViewport_ = new Viewport(context_, rttScene_, rttCam_);
        surface->SetViewport(0, rttViewport_);
        rttScene_->SetUpdateEnabled(true);

        update_grids();
    }

    void SceneCtrl::OpenScene(const String& path) 
    { 
        GetSubsystem<Graphics>()->MakeCurrent();
        Clear();
        File file(context_,path,FileMode::FILE_READ);
        rttScene_ = new Scene(context_);
        rttScene_->LoadJSON(file);
        InitScene(true);
        EditorApp::getInstance()->setCurTool("move");
        EditorApp::getInstance()->SelectNode(nullptr);
    }

    void SceneCtrl::OpenNewScene()
    {
        GetSubsystem<Graphics>()->MakeCurrent();
        Clear();
        rttScene_ = new Scene(context_);
        rttScene_->SetName("RttScene");
        rttScene_->CreateComponent<Octree>();
        InitScene(false);
        EditorApp::getInstance()->setCurTool("move");
        EditorApp::getInstance()->SelectNode(nullptr);
    }

    void SceneCtrl::createScene() 
    {
        rttScene_ = new Scene(context_);
        rttScene_->SetName("RttScene");
        rttScene_->CreateComponent<Octree>();
        InitScene(false);
        EditorApp::getInstance()->SelectNode(nullptr);
    }

	void SceneCtrl::update()
	{
		/*auto* model = _modelNode->GetComponent<AnimatedModel>(true);
		if (model->GetNumAnimationStates())
		{
			AnimationState* state = model->GetAnimationStates()[0];
			state->AddTime(0.01);
		}*/
	}

	Node* SceneCtrl::select(float x, float y)
	{
		Node* hit = nullptr;
        Ray world_ray = rttCameraNode_->GetComponent<Camera>()->GetScreenRay(x, y);
		float dis = 100000;
        hit = intersectObj(world_ray, rttSceneRoot_, dis);
        EditorApp::getInstance()->SelectNode(hit);
		return hit;
	}
}
