#include <string>
#include <sstream>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include "CrashCourse.h"


URHO3D_DEFINE_APPLICATION_MAIN(CrashCourse)
CrashCourse::CrashCourse(Context * context) : Sample(context)
{
}

void CrashCourse::Setup() 
{
	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = 1280;
	engineParameters_["WindowHeight"] = 720;//1280 : 960 for 4:3
	engineParameters_["WindowResizable"] = true;
	GetSubsystem<Engine>()->SetMaxFps(60);
	context_->RegisterFactory<Cannon>();
	context_->RegisterFactory<MyTools>();
	context_->RegisterFactory<Viking>();
	context_->RegisterFactory<Interactable>();
	context_->RegisterFactory<Level>();
	context_->RegisterFactory<MyTimer>();
	context_->RegisterFactory<CameraController>();
	Level::RegisterLevels(context_);
	context_->RegisterFactory<Lerppa>();
	context_->RegisterFactory<MyAnimator>();

}
void CrashCourse::Start() 
{
	// We will be needing to load resources.
		// All the resources used in this example comes with Urho3D.
		// If the engine can't find them, check the ResourcePrefixPath (see http://urho3d.github.io/documentation/1.5/_main_loop.html).
	ResourceCache* cache = GetSubsystem<ResourceCache>();


	// Let's use the default style that comes with Urho3D.
	GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));



	// Let's setup a scene to render.
	scene_ = new Scene(context_);
	// Let the scene have an Octree component!
	scene_->CreateComponent<Octree>();
	// Let's add an additional scene component for fun.
	scene_->CreateComponent<DebugRenderer>();


	auto* zone = scene_->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
	zone->SetFogColor(Color::GRAY);
	zone->SetFogStart(100.0f);
	zone->SetFogEnd(300.0f);
	PhysicsWorld* physicsWorld = scene_->CreateComponent<PhysicsWorld>();
	PhysicsWorld2D* physics2d = scene_->CreateComponent<PhysicsWorld2D>();


	cam_ = scene_->CreateComponent<CameraController>();
	cam_->Init(scene_);
	//scene_->CreateComponent<CameraController>()->Init(scene_);
	
	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(CrashCourse, HandleKeyDown));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CrashCourse, HandleUpdate));
	//SubscribeToEvent(E_RESIZED, URHO3D_HANDLER(CrashCourse, HandleResize));



	auto* input = GetSubsystem<Input>();
	input->SetTouchEmulation(true);
	scene_->CreateComponent<DebugRenderer>();
	scene_->CreateComponent<MyTools>();
	MyTools::LoadValues();
	UIManager::unlockedLevel = MyTools::GetInt("unlockedLevel");
	DoUI();
	//scene_->CreateComponent<CameraController>()->Init();
	SoundSource* musicSource = scene_->CreateComponent<SoundSource>();
	Sound* music = cache->GetResource<Sound>("Sounds/game.ogg");

	MyTimer::New([=]() {
		musicSource->Play(music);
	}, 0, music->GetLength(), true);
	VariantMap a;
	HandleResize("", a);
}
void CrashCourse::CreateText()
{

}
void CrashCourse::SubscribeToEvents() 
{
	SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(CrashCourse, HandleKeyDown));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CrashCourse, HandleUpdate));
	
}
void CrashCourse::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

	// Movement speed as world units per second
	float MOVE_SPEED = 10.0f;
	// Mouse sensitivity as degrees per pixel
	const float MOUSE_SENSITIVITY = 0.1f;


	ui_->Update();

	cam_->Update(MyTime->GetTimeStep());
	totalFrames++;
	MakeProtoScene();
}
void CrashCourse::HandleKeyDown(StringHash eventType, VariantMap& eventData)
{
	using namespace KeyDown;
	int key = eventData[P_KEY].GetInt();
	if (key == KEY_ESCAPE)
		engine_->Exit();

	if (key == KEY_TAB)    // toggle mouse cursor when pressing tab
	{
		GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
	}
}
void CrashCourse::HandleClosePressed(StringHash eventType, VariantMap& eventData)
{
	engine_->Exit();
}
void CrashCourse::HandleResize(StringHash eventType, VariantMap& eventData)
{
	//normalizing horizontal view distance across all aspect ratios to match 16/9
	Graphics* gfx = GetSubsystem<Graphics>();
	float currentAspect = (float)gfx->GetWidth() / gfx->GetHeight();
	float aspectMultiplier = ((float)16 / 9) / currentAspect;
	//CameraController::cameraNode_
	Camera* camera = cam_->cameraNode_->GetComponent<Camera>();
	//Camera* camera = cameraNode_->GetComponent<Camera>();
	//URHO3D_LOGINFO("currentAspect " + String(currentAspect) + " 16/ 9 " + String((float)16 / 9) + " camera original ortho " + String(camera->GetOrthoSize()) + " multiplier " + String(aspectMultiplier));
	camera->SetOrthoSize(20 * aspectMultiplier);//20 is the default orthosize
}
void CrashCourse::DoUI()
{
	UIElement* root = GetSubsystem<UI>()->GetRoot();
	ui_ = new UIManager(context_);
	ui_->CreateMenus();
	ui_->HideAll();
	ui_->GetOne(1);

}
void CrashCourse::MakeProtoScene() {
	//debug load
	Input* input = GetSubsystem<Input>();
	if (input->GetKeyPress(Key::KEY_0))
	{
		Level::currentLevel->Unload();
	}
	if (input->GetKeyPress(Key::KEY_1))
	{
		Level::currentLevel->Init();
	}

	if (createdProto || totalFrames < 2)//only run once after first frame because urho spaghetticam screentoworld doesnt work on first frame because of default aspect
		return;
	createdProto = true;

	//Level::NewLevel(1);
	//Level::currentLevel->Init();
	//ui_->HideAll();
}