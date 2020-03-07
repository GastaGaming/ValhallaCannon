#include "Level4.h"
#include "Cannon.h"
#include "UIManager.h"
#include "Levelparser.h"

Level4::Level4(Context* context) :Level(context)
{
}
Level4::~Level4()
{
}
void Level4::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL4 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;

	//gate
	CreateObject("Gate", "Levels/Level_004/Objects/Spr_WoodenGate01.png", 11.55, -3.59)->SetScale2D(1.98, 1.98);
	CreateInteractable("WoodPillar", "Levels/Level_004/Objects/Spr_WoodPillar01.png", -6.74, -6.01)->SetScale2D(1.44, 1.44);
	CreateAnimatedInteractable("Guard", "Characters/Guard/GuardAnimation.scml","Idle", 2.15, -4.72)->SetScale2D(1.53, 1.53);
	CreateAnimatedInteractable("Seagull1", "Seagull/Anim_Seagull01.scml","Idle", -0.14, 0.44,2)->SetScale2D(1.44, 1.44);
	CreateAnimatedInteractable("Seagull2", "Seagull/Anim_Seagull01.scml","Idle" ,-6.61, -2.73,1)->SetScale2D(1.44, 1.44);
	seagull = scene_->GetChild("Seagull2");
	Node* lock = CreateInteractable("lock", "Levels/Level_004/Objects/Spr_Lock.png", 9.96, -4.09, 1.71, 0);
	SetLayer(lock, 0, 1);

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_004/Level_004_animwater.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale2D(1.14, 1.14);
	//tileMapNode->SetPosition(Vector3(-16, -6, 0));
	tileMapNode->SetPosition(Vector3(-34.79, -14.93, 0));
	tileMap->GetNode()->SetPosition2D(-34.79, -14.54);
	tileMap->GetNode()->SetScale2D(1.737521544, 1.737521544);


	// bg
	background = scene_->CreateChild("background");
	StaticSprite2D* bgSprite = background->CreateComponent<StaticSprite2D>();
	bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/LevelBGs/FourthLevelBG01.png"));
	background->SetScale2D(2.14, 2.14);
	bgSprite->SetLayer(-5);
	bgSprite->SetOrderInLayer(214);
	background->SetEnabled(UIManager::bgOn);

	foreground = scene_->CreateChild("foreground");
	StaticSprite2D* fgSprite = foreground->CreateComponent<StaticSprite2D>();
	fgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/Levels/Level_004/Objects/Spr_ForeGround01.png"));
	foreground->SetPosition(Vector3(0, 5.97, 0));
	foreground->SetScale2D(1.88, 1.88);
	fgSprite->SetLayer(5);
	fgSprite->SetOrderInLayer(214);
	foreground->SetEnabled(UIManager::bgOn);
	//mockup
	/*Node* mockup = CreateSprite("mockup", "Mockups/Level4.png", -0.02, -0.02);
	mockup->SetScale2D(2.22, 2.22);
	StaticSprite2D* mockSprite = mockup->CreateComponent<StaticSprite2D>();
	mockSprite->SetLayer(-5);
	mockSprite->SetOrderInLayer(214);*/

	////water
	//Node* water1 = CreateSprite("water1", "Spr_Water01.png", -2.2, -7.52, 2.11);
	//SetLayer(water1, 0, -1);
	//Node* water2 = CreateSprite("water2", "Spr_Water02.png", -0.22, -7.6, 1.93);
	//SetLayer(water2, 0, 0);
	//Node* water3 = CreateSprite("water3", "Spr_Water03.png", -1.57, -8.57, 2.02);
	//SetLayer(water3, 0, 3);
	//Node* foreground = CreateSprite("foreground", "Spr_ForeGround01.png", 0, 0, 0, 0);

	Node* ship = CreateSprite("ship", "Spr_ShipSailsDown.png", -19.19, -3.47, 0.85);
	CreateSound(ambientSound);

	//Vilin TMX tutkimuksia

	WeakPtr<LevelParser> levelParser_;

	// Spawna TMX filess olevan mapin peliin
	/*SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap"));
	tileMapNode->SetScale(0.9);
	tileMapNode->SetPosition2D(Vector2(-18, -10.25));
	auto* tileMap = tileMapNode->CreateComponent<TileMap2D>();
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_005/Level_005.tmx"));*/
	const TileMapInfo2D& info = tileMap->GetInfo();

	// Luo kolliision TMX filess "Physics" layeriin, joka pitisi siin olla listan ekana (layer 1)
	TileMapLayer2D* tileMapLayer = tileMap->GetLayer(tileMap->GetNumLayers() - 1);
	levelParser_ = new LevelParser(context_);
	levelParser_->CreateCollisionShapesFromTMXObjects(tileMapNode, tileMapLayer, info);

	//Pitisi spawnaa animoituja vesitilei TMX filess layer 2 objection kohdalle (kolikoita tll hetkell
	TileMapLayer2D* coinsLayer = tileMap->GetLayer(tileMap->GetNumLayers() - 2);
	levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_), 4);
}
void Level4::HandleInteraction(Viking* viking, Interactable* interactable) {

	Node* interactableNode = interactable->GetNode();
	if (viking->type == VikingType::Heavy)
	{
		if (interactable->GetNode()->GetName() == "Guard")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			//CreateSound(interactableNode, "BigExplosion.wav");
			CreateSound("08_heavyhitobject.ogg");

			interactable->GetNode()->GetComponent<AnimatedSprite2D>()->SetEnabled(false);
		}
		else if (interactable->GetNode()->GetName() == "Seagull1")
		{
			viking->GetNode()->SetEnabled(false);
		}
		else if (interactable->GetNode()->GetName() == "Seagull2")
		{
			viking->GetNode()->SetEnabled(false);

		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->CreateComponent<Interactable>();
			//viking->GetNode()->RemoveComponent<Viking>();
		}
	}
	else if (viking->type == VikingType::Rogue)
	{
		if (interactable->GetNode()->GetName() == "Seagull1")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			CreateSound(interactableNode, "BigExplosion.wav");
			interactable->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
			//interactable->GetNode()->GetComponent<StaticSprite2D>()->SetEnabled(false);
			Lerppa::LerpThis(interactable->GetNode(), Vector2(-10, 15), 1);

		}
		if (interactable->GetNode()->GetName() == "Seagull2")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			CreateSound(interactableNode, "BigExplosion.wav");
			interactable->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
			//interactable->GetNode()->GetComponent<StaticSprite2D>()->SetEnabled(false);
			Lerppa::LerpThis(interactable->GetNode(), Vector2(-10, 15), 1);

		}
		if (interactable->GetNode()->GetName() == "lock")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			interactable->GetNode()->GetComponent<StaticSprite2D>()->SetEnabled(false);
			DelayedWin();
		}
		else if (interactable->GetNode()->GetName() == "Guard")
		{

			viking->GetNode()->SetEnabled(false);
		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
		}
	}
	else
	{
		if (interactable->GetNode()->GetName() == "WoodPillar")
		{
			viking->GetNode()->SetEnabled(false);
			ResourceCache* cache = GetSubsystem<ResourceCache>();
			CreateEffect(interactableNode, "Flame.xml", 1);
			CreateEffect(interactableNode, "SmokeStack.xml")->GetNode()->Translate(Vector3::FORWARD);
			//CreateSound(interactableNode, "BigExplosion.wav");
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			viking->GetNode()->SetEnabled(false);
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			seagull->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
			Lerppa::LerpThis(seagull, Vector2(-10, 15), 1);
		}
		else if (interactable->GetNode()->GetName() == "Seagull1")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			ResourceCache* cache = GetSubsystem<ResourceCache>();
			CreateEffect(interactableNode, "Flame.xml", 1);
			CreateEffect(interactableNode, "SmokeStack.xml")->GetNode()->Translate(Vector3::FORWARD);
			interactable->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
			//CreateSound(interactableNode, "BigExplosion.wav");
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			Lerppa::LerpThis(interactable->GetNode(), Vector2(-10, 15), 0.5);
			
		}
		else if (interactable->GetNode()->GetName() == "Seagull2")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			ResourceCache* cache = GetSubsystem<ResourceCache>();
			CreateEffect(interactableNode, "Flame.xml", 1);
			CreateEffect(interactableNode, "SmokeStack.xml")->GetNode()->Translate(Vector3::FORWARD);
			interactable->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
			//CreateSound(interactableNode, "BigExplosion.wav");
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			Lerppa::LerpThis(interactable->GetNode(), Vector2(-10, 15), 0.5);

		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
		}

	}

	CheckLose();
}