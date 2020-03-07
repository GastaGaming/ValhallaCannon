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
#include "UIManager.h"
#include <Urho3D/Urho3DAll.h>
#include "Cannon.h"
#include "MyTools.h"
#include "Viking.h"
#include "Interactable.h"
#include "LevelParser.h"
#include "Level.h"
#include "CameraController.h"// #include <Urho3D/Urho3DAll.h>
#include "Lerppa.h"
#include "MyAnimator.h"
#include "Printable.h"

using namespace Urho3D;
/**
* Using the convenient Application API we don't have
* to worry about initializing the engine or writing a main.
* You can probably mess around with initializing the engine
* and running a main manually, but this is convenient and portable.
*/
class CrashCourse : public Application
{
public:

	SharedPtr<Text> text_;
	SharedPtr<Scene> scene_;
	SharedPtr<Node> boxNode_;
	//SharedPtr<Node> cameraNode_;
	WeakPtr<UIManager> ui_;
	//WeakPtr<LevelParser> levelParser_;
	SharedPtr<CameraController> cam_;
	SharedPtr<Node> camera_;
	int totalFrames = 0;
	/**
	* This happens before the engine has been initialized
	* so it's usually minimal code setting defaults for
	* whatever instance variables you have.
	* You can also do this in the Setup method.
	*/
	CrashCourse(Context * context) : Application(context)
	{
	}

	/**
	* This method is called before the engine has been initialized.
	* Thusly, we can setup the engine parameters before anything else
	* of engine importance happens (such as windows, search paths,
	* resolution and other things that might be user configurable).
	*/
	virtual void Setup()
	{
		// These parameters should be self-explanatory.
		// See http://urho3d.github.io/documentation/1.5/_main_loop.html
		// for a more complete list.
		ngineParameters_["WindowTitle"] = "Valhalla Cannon";
		engineParameters_["FullScreen"] = false;
		engineParameters_["WindowWidth"] = 1280;
		engineParameters_["WindowHeight"] = 720;//1280 : 960 for 4:3
		engineParameters_["WindowResizable"] = true;
		GetSubsystem<Engine>()->SetMaxFps(999999);
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

	/**
	* This method is called after the engine has been initialized.
	* This is where you set up your actual content, such as scenes,
	* models, controls and what not. Basically, anything that needs
	* the engine initialized and ready goes in here.
	*/
	virtual void Start()
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


		// We need a camera from which the viewport can render.
		cam_ = scene_->CreateComponent<CameraController>();
		cam_->Init(scene_);

		// We subscribe to the events we'd like to handle.
		// In this example we will be showing what most of them do,
		// but in reality you would only subscribe to the events
		// you really need to handle.
		// These are sort of subscribed in the order in which the engine
		// would send the events. Read each handler method's comment for
		// details.
		SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(CrashCourse, HandleBeginFrame));
		SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(CrashCourse, HandleKeyDown));
		SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(CrashCourse, HandleUpdate));
		SubscribeToEvent(E_POSTUPDATE, URHO3D_HANDLER(CrashCourse, HandlePostUpdate));
		SubscribeToEvent(E_RENDERUPDATE, URHO3D_HANDLER(CrashCourse, HandleRenderUpdate));
		SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(CrashCourse, HandlePostRenderUpdate));
		SubscribeToEvent(E_ENDFRAME, URHO3D_HANDLER(CrashCourse, HandleEndFrame));
		//SubscribeToEvent(E_RESIZED, URHO3D_HANDLER(CrashCourse, HandleResize));
		


		auto* input = GetSubsystem<Input>();
		input->SetTouchEmulation(true);
		scene_->CreateComponent<DebugRenderer>();
		scene_->CreateComponent<MyTools>();
		MyTools::LoadValues();
		UIManager::unlockedLevel = MyTools::GetInt("unlockedLevel");
		DoUI();

		//Vilin TMX tutkimuksia

		// Spawna TMX filess olevan mapin peliin
		/*SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap"));
		tileMapNode->SetScale(0.9);
		tileMapNode->SetPosition2D(Vector2(-18, -10.25));
		auto* tileMap = tileMapNode->CreateComponent<TileMap2D>();
		tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_005/Level_005.tmx"));
		const TileMapInfo2D& info = tileMap->GetInfo();

		// Luo kolliision TMX filess "Physics" layeriin, joka pitisi siin olla listan ekana (layer 1)
		TileMapLayer2D* tileMapLayer = tileMap->GetLayer(tileMap->GetNumLayers() - 1);
		levelParser_ = new LevelParser(context_);
		levelParser_->CreateCollisionShapesFromTMXObjects(tileMapNode, tileMapLayer, info);

		//Pitisi spawnaa animoituja vesitilei TMX filess layer 2 objection kohdalle (kolikoita tll hetkell
		TileMapLayer2D* coinsLayer = tileMap->GetLayer(tileMap->GetNumLayers() - 2);
		levelParser_->PopulateCoins(coinsLayer);
		*/
		SoundSource* musicSource = scene_->CreateComponent<SoundSource>();
		Sound* music = cache->GetResource<Sound>("Sounds/game.ogg");
		
		MyTimer::New([=]() {
			musicSource->Play(music);
		},0,music->GetLength(),true);
		VariantMap a;
		HandleResize("",a);
	}

	/**
	* Good place to get rid of any system resources that requires the
	* engine still initialized. You could do the rest in the destructor,
	* but there's no need, this method will get called when the engine stops,
	* for whatever reason (short of a segfault).
	*/
	virtual void Stop()
	{
	}

	/**
	* Every frame's life must begin somewhere. Here it is.
	*/
	void HandleBeginFrame(StringHash eventType, VariantMap& eventData)
	{
		// We really don't have anything useful to do here for this example.
		// Probably shouldn't be subscribing to events we don't care about.
	}

	/**
	* Input from keyboard is handled here. I'm assuming that Input, if
	* available, will be handled before E_UPDATE.
	*/
	void HandleKeyDown(StringHash eventType, VariantMap& eventData)
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

	/**
	* You can get these events from when ever the user interacts with the UI.
	*/
	void HandleClosePressed(StringHash eventType, VariantMap& eventData)
	{
		engine_->Exit();
	}
	/**
	* Your non-rendering logic should be handled here.
	* This could be moving objects, checking collisions and reaction, etc.
	*/
	void HandleUpdate(StringHash eventType, VariantMap& eventData)
	{
		float timeStep = eventData[Update::P_TIMESTEP].GetFloat();

		// Movement speed as world units per second
		float MOVE_SPEED = 10.0f;
		// Mouse sensitivity as degrees per pixel
		const float MOUSE_SENSITIVITY = 0.1f;


		ui_->Update();
		//scene_->GetComponent<CameraController>()->Update(timeStep);
		cam_->Update(MyTime->GetTimeStep());
		totalFrames++;
		MakeProtoScene();
		//scene_->GetComponent<PhysicsWorld2D>()->DrawDebugGeometry(MyTools::currentScene->GetComponent<DebugRenderer>(), false);
	}
	/**
	* Anything in the non-rendering logic that requires a second pass,
	* it might be well suited to be handled here.
	*/
	void HandlePostUpdate(StringHash eventType, VariantMap& eventData)
	{
		// We really don't have anything useful to do here for this example.
		// Probably shouldn't be subscribing to events we don't care about.
	}
	/**
	* If you have any details you want to change before the viewport is
	* rendered, try putting it here.
	* See http://urho3d.github.io/documentation/1.32/_rendering.html
	* for details on how the rendering pipeline is setup.
	*/
	void HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
	{
		// We really don't have anything useful to do here for this example.
		// Probably shouldn't be subscribing to events we don't care about.
	}
	/**
	* After everything is rendered, there might still be things you wish
	* to add to the rendering. At this point you cannot modify the scene,
	* only post rendering is allowed. Good for adding things like debug
	* artifacts on screen or brush up lighting, etc.
	*/
	void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
	{
		// We could draw some debuggy looking thing for the octree.
		// scene_->GetComponent<Octree>()->DrawDebugGeometry(true);
	}
	/**
	* All good things must come to an end.
	*/
	void HandleEndFrame(StringHash eventType, VariantMap& eventData)
	{
		// We really don't have anything useful to do here for this example.
		// Probably shouldn't be subscribing to events we don't care about.
	}
	void DoUI()
	{
		UIElement* root = GetSubsystem<UI>()->GetRoot();
		ui_ = new UIManager(context_);
		ui_->CreateMenus();
		ui_->HideAll();
		ui_->GetOne(8);

	}

	bool createdProto = false;
	void MakeProtoScene() {
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
	void HandleResize(StringHash eventType, VariantMap& eventData) {
		//normalizing horizontal view distance across all aspect ratios to match 16/9
		Graphics* gfx = GetSubsystem<Graphics>();
		float currentAspect = (float)gfx->GetWidth() / gfx->GetHeight();
		float aspectMultiplier = ((float)16 / 9) / currentAspect;
		//CameraController::cameraNode_
		Camera* camera = cam_->cameraNode_->GetComponent<Camera>();
		//Camera* camera = cameraNode_->GetComponent<Camera>();
		//URHO3D_LOGINFO("currentAspect " + String(currentAspect) + " 16/ 9 " + String((float)16 / 9) + " camera original ortho " + String(camera->GetOrthoSize()) + " multiplier " + String(aspectMultiplier));
		//camera->SetOrthoSize(20*aspectMultiplier);//20 is the default orthosize
	}
};

/**
* This macro is expanded to (roughly, depending on OS) this:
*
* > int RunApplication()
* > {
* > Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
* > Urho3D::SharedPtr<className> application(new className(context));
* > return application->Run();
* > }
* >
* > int main(int argc, char** argv)
* > {
* > Urho3D::ParseArguments(argc, argv);
* > return function;
* > }
*/URHO3D_DEFINE_APPLICATION_MAIN(CrashCourse)
