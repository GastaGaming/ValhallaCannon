#include "Level3.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level3::Level3(Context* context) :Level(context)
{
}

Node * icebergtop;
Node * icebergbot;
Node* icebergmid;
Level3::~Level3()
{
}
void Level3::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL3 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	//gate

	Node* icebergcrack = CreateInteractable("IcebergCrack", "Levels/Level_003/Objects/Spr_IceBergCrack.png", 5.41, -3.95, 1.82, 0);
	SetLayer(icebergcrack, 0, 3);
	icebergtop = CreateInteractable("IcebergTop", "Levels/Level_003/Objects/Spr_IceBergTop.png", 8.03, -1.49, 1.82, 0);
	SetLayer(icebergtop, 0, 2);
	icebergmid = CreateSprite("IcebergMid", "Levels/Level_003/Objects/Spr_IceBergMiddle.png", 8.06, -4.66, 1.82, 0);
	icebergmid->SetEnabled(false);
	SetLayer(icebergmid, 0, 1);
	icebergbot = CreateInteractable("IcebergBot", "Levels/Level_003/Objects/Spr_IceBergBottom.png", 8.2, -6.65, 1.82, 0);
	SetLayer(icebergbot, 0, 2);
	Node* Woodpillar = CreateInteractable("Woodpillar", "Levels/Level_002/Objects/Spr_WoodPillar01.png", 8.34, -3.61, 1.25, 0);
	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_003/Level_003_animwater.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale2D(1, 1);
	//tileMapNode->SetPosition(Vector3(-16, -6, 0));
	tileMapNode->SetPosition(Vector3(-40, -12.38, 0));
	tileMap->GetLayer(0)->SetDrawOrder(-5);
	tileMap->GetNode()->SetPosition2D(-35.65, -13.48);
	tileMap->GetNode()->SetScale2D(1.741595625, 1.741595625);


	// bg
	background = scene_->CreateChild("background");
	StaticSprite2D* bgSprite = background->CreateComponent<StaticSprite2D>();
	bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/LevelBGs/ThirdLevelBG01.png"));
	background->SetScale2D(2.14, 2.14);
	bgSprite->SetLayer(-5);
	bgSprite->SetOrderInLayer(214);
	background->SetEnabled(UIManager::bgOn);


	//mockup
	/*Node* mockup = CreateSprite("mockup", "Mockups/Level3.png", -0.02, -0.02);
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
	Node* ship = CreateSprite("ship", "Common/Spr_ShipSailsDown.png", -19.19, -3.47, 0.85);
	Node* panel = CreateSprite("panel", "Common/Spr_ShipPanel.png", -16.66, -5.69);
	SetLayer(panel, 0, -2);

	/*Node* mockup = scene_->CreateChild("mockup");
	StaticSprite2D* mockSprite = mockup->CreateComponent<StaticSprite2D>();
	mockSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/Mockups/Level3.png"));
	mockup->SetScale2D(2.22, 2.22);
	mockSprite->SetLayer(-5);
	mockSprite->SetOrderInLayer(214);
	mockup->SetPosition2D(-0.02, -0.02);*/
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
	levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_), 3);

}
void Level3::HandleInteraction(Viking* viking, Interactable* interactable) {

	Node* interactableNode = interactable->GetNode();
	if (viking->type == VikingType::Heavy)
	{
		if (interactable->GetNode()->GetName() == "IcebergCrack")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			CreateSound(interactableNode, "BigExplosion.wav");
			interactable->GetNode()->GetComponent<StaticSprite2D>()->SetEnabled(false);
			Lerppa::LerpThis(icebergtop, Vector2(8.03, -20), 1);
			icebergmid->SetEnabled(true);
			CreateSound("08_heavyhitobject.ogg");
		}

	}
	if (viking->type == VikingType::Torcher)
	{
		if (interactable->GetNode()->GetName() == "Woodpillar")
		{
			//interactable->GetNode()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			//interactable->GetNode()->GetComponent<StaticSprite2D>()->SetEnabled(false);
			/*icebergbot->SetEnabled(false);
			icebergmid->SetEnabled(false);*/
			DelayedWin();
			CreateSound("07_torcherfire.ogg");
			CreateEffect(interactableNode, "Flame.xml", 1);
			CreateEffect(interactableNode, "SmokeStack.xml", 1)->GetNode()->Translate(Vector3::FORWARD);
			Lerppa::LerpThis(interactableNode, Vector2(8.34, -15), 10);
			Lerppa::LerpThis(icebergbot, Vector2(8.2, -15), 10);
			Lerppa::LerpThis(icebergmid, Vector2(8.06, -11.97), 10);
		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
		}
	}
	if (viking->type == VikingType::Rogue) {
		viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
	}
	CheckLose();
}