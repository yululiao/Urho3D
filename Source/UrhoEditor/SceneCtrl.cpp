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
//#include "ctrl/utils.h"
//#include "ctrl/asset_mgr.h"
//#include "ctrl/global_event.h"

namespace Urho3D
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

    void SceneCtrl::create_models()
    {
        auto* cache = GetSubsystem<ResourceCache>();

        // Create scene node & StaticModel component for showing a static plane
        Node* planeNode = scene_->CreateChild("Plane");
        planeNode->SetScale(Vector3(50.0f, 1.0f, 50.0f));
        auto* planeObject = planeNode->CreateComponent<StaticModel>();
        planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
        //Material* tmp_mat = cache->GetResource<Material>("Materials/StoneTiled.xml");
        planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
        //planeObject->GetMaterial()->SetFillMode(FillMode::FILL_WIREFRAME);

        SharedPtr<Node> geoNode(GeoUtils::create_sphere(context_, 1, 30, 30));
        geoNode->SetName("geoNode");
        //geoNode->SetScale(Vector3(0.1f, 0.1f, 0.1f));
        geoNode->SetPosition(Vector3(2.0f, 1.0f, 15.0f));
        geoNode->Pitch(-90);
        //SharedPtr<Model> model = geometry_util::create_plane(_context,2,2);
        //SharedPtr<Model> model = geometry_util::create_torus(_context, 1, 0.5,30,30);
        //SharedPtr<Model> model = geometry_util::create_cylinder(_context, 1, 0.0, 2, 30,10);
        //SharedPtr<Model> model = geometry_util::create_disk(_context, 0.5, 1.0, 20, 20);
        //SharedPtr<Model> model = geometry_util::create__cube(_context, 1.0);
        geoNode->GetComponent<StaticModel>()->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
        //geoNode->GetComponent<StaticModel>()->GetMaterial()->SetCullMode(CullMode::CULL_CW);
        _scene_root->AddChild(geoNode);

        _modelNode = _scene_root->CreateChild("Jill");
        _modelNode->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
        //modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
        auto* modelObject = _modelNode->CreateComponent<AnimatedModel>();
        modelObject->SetModel(cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
        Material* model_mat = cache->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml");
        modelObject->SetMaterial(model_mat);
        //model_mat->SetShaderParameter("MatDiffColor", Vector4(1.0,0.0,0.0,1));
        //modelObject->GetMaterial()->SetFillMode(FillMode::FILL_WIREFRAME);
        modelObject->SetCastShadows(true);

        auto* walkAnimation = cache->GetResource<Animation>("Models/Kachujin/Kachujin_Walk.ani");
        AnimationState* state = modelObject->AddAnimationState(walkAnimation);
        if (state)
        {
            state->SetWeight(1.0f);
            state->SetLooped(true);
            state->SetTime(Random(walkAnimation->GetLength()));
        }

    }

    void SceneCtrl::deleteNode(Urho3D::Node* node) 
    { 
        node->Remove();
    }

    void SceneCtrl::addModel(const String& path)
    {
        auto* cache = GetSubsystem<ResourceCache>();
        
        String name = AssetMgr::getInstance()->getBaseName(path); // Utils::get_base_name(path);
        //加载fbx对应的mdl
        String mdl_path = AssetMgr::getInstance()->getFilePath(path) + "/" + name + ".mdl";
        String rpath = AssetMgr::getInstance()->pathToRelative(mdl_path);

        Node* modelNode = rttSceneRoot_->CreateChild(name);
        
       // modelNode->SetScale(Vector3(0.01,0.01,0.01));
        //modelNode->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        //modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
        auto* modelObject = modelNode->CreateComponent<AnimatedModel>();
        //modelObject->SetModel(cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
        //modelObject->SetModel(cache->GetResource<Model>("Models/001_Mesh.mdl"));
        Model* model = cache->GetResource<Model>(rpath);
        modelObject->SetModel(model);

        Node* cube = GeoUtils::create_cube(_ctx, 1);
        rttSceneRoot_->AddChild(cube);

        //rttScene_->Update();
        //Material* model_mat = cache->GetResource<Material>("assets/models/001/tex/001.xml");
        //modelObject->SetMaterial(model_mat);
        //global_event::get_instance()->emit_event(eGlobalEventType::AddNodeToScene);
        //renderTexture->GetImage()->SavePNG("test.png");
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
            _grid_root = rttScene_->CreateChild("GridRoot");
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

	void SceneCtrl::create_scene()
	{
        create_rttScene();
        auto* cache = GetSubsystem<ResourceCache>();
        scene_ = new Scene(context_);
        scene_->SetName("SceneMain");
        scene_->CreateComponent<Octree>();
        Node* zoneNode = scene_->CreateChild("Zone");
        auto* zone = zoneNode->CreateComponent<Zone>();
        zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
        zone->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
        zone->SetFogStart(100.0f);
        zone->SetFogEnd(300.0f);
        Node* lightNode = scene_->CreateChild("DirectionalLight");
        lightNode->SetDirection(Vector3(0.5f, -1.0f, 0.5f));
        auto* light = lightNode->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetColor(Color(0.2f, 0.2f, 0.2f));
        light->SetSpecularIntensity(1.0f);
        Node* screenNode = scene_->CreateChild("Screen");
        screenNode->SetPosition(Vector3(0.0f, 10.0f, -0.27f));
        screenNode->SetRotation(Quaternion(-90.0f, 0.0f, 0.0f));
        screenNode->SetScale(Vector3(20.0f, 0.0f, 15.0f));
        auto* screenObject = screenNode->CreateComponent<StaticModel>();
        screenObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
        renderMaterial_ = new Material(context_);
        renderMaterial_->SetTechnique(0, cache->GetResource<Technique>("Techniques/DiffUnlit.xml"));
        renderMaterial_->SetTexture(TU_DIFFUSE, renderTexture);
        renderMaterial_->SetDepthBias(BiasParameters(-0.001f, 0.0f));
        screenObject->SetMaterial(renderMaterial_);

        cameraNode_ = scene_->CreateChild("Camera");
        auto* camera = cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(300.0f);
        cameraNode_->SetPosition(Vector3(0.0f, 7.0f, -30.0f));
        auto* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);
	}

    void SceneCtrl::create_rttScene() 
    {
        auto* cache = GetSubsystem<ResourceCache>();
        rttScene_ = new Scene(context_);
        rttScene_->SetName("RttScene");
        rttScene_->CreateComponent<Octree>();
        rttSceneRoot_ = rttScene_->CreateChild("Root");
        Node* zoneNode = rttScene_->CreateChild("Zone");
        auto* zone = zoneNode->CreateComponent<Zone>();
        zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
        zone->SetAmbientColor(Color(0.2f, 0.2f, 0.2f));
        zone->SetFogColor(Color(0.2f, 0.2f, 0.2f));
        zone->SetFogStart(10.0f);
        zone->SetFogEnd(100.0f);
        
        rttCameraNode_ = rttScene_->CreateChild("Camera");
        rttCameraNode_->SetPosition(Vector3(0.0f, 5.0f, -10.0f));
        rttCameraNode_->LookAt(Vector3(0.0f, 0.0f, -0.0f));
        rttCam_ = rttCameraNode_->CreateComponent<Camera>();
        rttCam_->SetFarClip(300.0f);
        rttCam_->SetAutoAspectRatio(false);
        rttCam_->SetAspectRatio(constRttSize.x_/constRttSize.y_);
        //auto* light = rttCameraNode_->CreateComponent<Light>();
        //light->SetLightType(LIGHT_POINT);
        //light->SetRange(30.0f);

        auto* light = rttCameraNode_->CreateComponent<Light>();
        light->SetLightType(LIGHT_DIRECTIONAL);
        light->SetColor(Color(0.2f, 0.2f, 0.2f));
        light->SetSpecularIntensity(1.0f);

        renderTexture = new Texture2D(context_);
        renderTexture->SetSize(constRttSize.x_, constRttSize.y_, Graphics::GetRGBFormat(), TEXTURE_RENDERTARGET);
        renderTexture->SetFilterMode(FILTER_BILINEAR);
        RenderSurface* surface = renderTexture->GetRenderSurface();
        rttViewport_ = new Viewport(context_, rttScene_, rttCam_);
        surface->SetViewport(0, rttViewport_);
        rttScene_->SetUpdateEnabled(true);
        //Node* cube = GeoUtils::create_cube(_ctx, 1);
        //rttSceneRoot_->AddChild(cube);
       
        update_grids();
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
        Ray world_ray = cameraNode_->GetComponent<Camera>()->GetScreenRay(x, y);
		float dis = 100000;
        hit = intersectObj(world_ray, rttSceneRoot_, dis);
		return hit;
	}
}
