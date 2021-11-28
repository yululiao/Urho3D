#include "scene_ctrl.h"

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
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include "ctrl/geometry_util.h"
#include "ctrl/utils.h"
#include "ctrl/asset_mgr.h"
#include "ctrl/global_event.h"

namespace urho3d
{
namespace editor
{
	scene_ctrl* scene_ctrl::_instance = nullptr;
	Context* scene_ctrl::_ctx = nullptr;
	scene_ctrl* scene_ctrl::get_inatance()
	{
		if (_instance == nullptr)
		{
			_instance = new scene_ctrl(_ctx);
		}
		return _instance;
	}

	scene_ctrl::scene_ctrl(Context* ctx)
		:Object(ctx)
	{

	}

	scene_ctrl::~scene_ctrl()
	{

	}

    void scene_ctrl::create_models()
    {
        auto* cache = GetSubsystem<ResourceCache>();

        // Create scene node & StaticModel component for showing a static plane
        Node* planeNode = _scene->CreateChild("Plane");
        planeNode->SetScale(Vector3(50.0f, 1.0f, 50.0f));
        auto* planeObject = planeNode->CreateComponent<StaticModel>();
        planeObject->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
        //Material* tmp_mat = cache->GetResource<Material>("Materials/StoneTiled.xml");
        planeObject->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));
        //planeObject->GetMaterial()->SetFillMode(FillMode::FILL_WIREFRAME);

        SharedPtr<Node> geoNode = geometry_util::create_sphere(context_, 1, 30, 30);
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

    void scene_ctrl::addModel(const std::string &path)
    {
        global_event::get_instance()->emit_event(eGlobalEventType::BeginInsertNode);
         auto* cache = GetSubsystem<ResourceCache>();
        std::string name = Utils::get_base_name(path);
        //加载fbx对应的mdl
        std::string mdl_path = Utils::get_file_path(path) + "/" + name + ".mdl";
        std::string rpath = asset_mgr::get_instance()->pathToRelative(mdl_path);

        Node* modelNode = _scene_root->CreateChild(name.c_str());
        modelNode->SetScale(Vector3(0.01,0.01,0.01));
        //modelNode->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        //modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
        auto* modelObject = modelNode->CreateComponent<AnimatedModel>();
        //modelObject->SetModel(cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
        //modelObject->SetModel(cache->GetResource<Model>("Models/001_Mesh.mdl"));
        Model* model = cache->GetResource<Model>(rpath.c_str());
        modelObject->SetModel(model);
        //Material* model_mat = cache->GetResource<Material>("assets/models/001/tex/001.xml");
        //modelObject->SetMaterial(model_mat);
        //global_event::get_instance()->emit_event(eGlobalEventType::AddNodeToScene);
        global_event::get_instance()->emit_event(eGlobalEventType::EndInsertNode);
    }

    void scene_ctrl::update_grids()
    {
        _grid_lines.clear();
        if(_grid_root.Null())
        {
            _grid_root = _scene->CreateChild("GridRoot");
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
            SharedPtr<Node> lineNode = geometry_util::create_line(context_, linePos, 0x999999);
            _grid_root->AddChild(lineNode);

        }
        //纵向
        for(int i=0;i<=line_count;i++)
        {
            float x = min + line_dis * i;
            std::vector<Vector3> linePos;
            linePos.push_back(Vector3(x,0,min));
            linePos.push_back(Vector3(x,0,max));
            SharedPtr<Node> lineNode = geometry_util::create_line(context_, linePos,0x999999);
            _grid_root->AddChild(lineNode);
        }

    }

	void scene_ctrl::create_scene()
	{
		_scene = new Scene(context_);

		// Create octree, use default volume (-1000, -1000, -1000) to (1000, 1000, 1000)
		// Also create a DebugRenderer component so that we can draw debug geometry
		_scene->CreateComponent<Octree>();
		_scene->CreateComponent<DebugRenderer>();
		_scene_root = _scene->CreateChild("SceneRoot");

		// Create a Zone component for ambient lighting & fog control
		Node* zoneNode = _scene->CreateChild("Zone");
		auto* zone = zoneNode->CreateComponent<Zone>();
		zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
		zone->SetAmbientColor(Color(0.5f, 0.5f, 0.5f));
		zone->SetFogColor(Color(0.4f, 0.5f, 0.8f));
		zone->SetFogStart(100.0f);
		zone->SetFogEnd(300.0f);

		// Create a directional light to the world. Enable cascaded shadows on it
		Node* lightNode = _scene->CreateChild("DirectionalLight");
		lightNode->SetDirection(Vector3(0.6f, -1.0f, 0.8f));
		auto* light = lightNode->CreateComponent<Light>();
		light->SetLightType(LIGHT_DIRECTIONAL);
		light->SetCastShadows(true);
		light->SetColor(Color(0.5f, 0.5f, 0.5f));
		light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
		// Set cascade splits at 10, 50 and 200 world units, fade shadows out at 80% of maximum shadow distance
		light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));

		_cameraNode = _scene->CreateChild("Camera");
		auto* camera = _cameraNode->CreateComponent<Camera>();
		camera->SetFarClip(300.0f);

		// Set an initial position for the camera scene node above the plane
		_cameraNode->SetPosition(Vector3(0.0f, 5.0f, -10.0f));

        //_cameraNode->LookAt(_modelNode->GetWorldPosition());
        _cameraNode->LookAt(Vector3(0.0f, 0.0f, -0.0f));

		auto* renderer = GetSubsystem<Renderer>();
		// Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
		SharedPtr<Viewport> viewport(new Viewport(context_, _scene, _cameraNode->GetComponent<Camera>()));
		renderer->SetViewport(0, viewport);

        update_grids();
        global_event::get_instance()->emit_event(eGlobalEventType::CreateScene);

	}

	void scene_ctrl::update()
	{
		/*auto* model = _modelNode->GetComponent<AnimatedModel>(true);
		if (model->GetNumAnimationStates())
		{
			AnimationState* state = model->GetAnimationStates()[0];
			state->AddTime(0.01);
		}*/
	}

	Node* scene_ctrl::select(float x, float y)
	{
		Node* hit = nullptr;
		Ray world_ray = _cameraNode->GetComponent<Camera>()->GetScreenRay(x, y);
		float dis = 100000;
		hit = Utils::intersectObj(world_ray, _scene_root, dis);

		return hit;
	}
}
}
