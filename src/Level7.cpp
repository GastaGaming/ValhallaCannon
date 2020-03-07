#include "Level7.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level7::Level7(Context* context) :Level(context)
{
}

Level7::~Level7()
{
}
//Node* enemy1;
//Node* enemy2;
//Node* lock;
//Node* roof;
//Node* gate;
void Level7::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL5 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_007/Level_007.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale(1.74);
	tileMapNode->SetPosition2D(-32.19, -14.41);

	// bg
	background = CreateSprite("background", "Levels/Level_007/Level_007_BgLayer.png", 0, 0, 2.14);
	SetLayer(background, -5, 214);
	background->SetEnabled(UIManager::bgOn);

	//fg
	Node* foreground = CreateSprite("foreground", "Levels/Level_007/Objects/Spr_ForeGround01.png", -0.05, 7.41, 1.86);
	SetLayer(foreground, 99, 99);

	Node* gate = CreateInteractable("gate", "Levels/Level_007/Objects/Spr_WoodenGate01.png", 15.22, -2.97, 1.22);
	Node* sign = CreateSprite("sign", "Levels/Level_007/Objects/Spr_Sign.png", -2.55, -8.18, 1, 0);
	SetLayer(sign,0,1);
	Node* backflowers = CreateSprite("backFlowers", "Levels/Level_007/Objects/Spr_BackFlowers01.png", 3.33, -9.55, 2.29, 0);
	Node* frontflowers = CreateSprite("frontFlowers", "Levels/Level_007/Objects/Spr_FrontFlowers01.png", -1.58, -8.72, 1.36, 0);

	Node* ship = CreateSprite("ship", "Common/Spr_ShipSailsDown.png", -16.91, -1.33, 0.85, 0);
	Node* panel = CreateSprite("panel", "Common/Spr_ShipPanel.png", -14.43, -3.52, 1, 0);
	target = CreateInteractable("target", "Levels/Level_007/Objects/Spr_WoodenGate01.png", 13.83, -8.21, 1.6, -44.84);
	target->RemoveComponent<StaticSprite2D>();

	//Node* target = CreateSprite("target","Levels/Level_007/");
	//cannon->SetParent(ship);
	//panel->SetParent(ship);
	//ship->SetPosition2D(-17.02, -1.21);
	SetLayer(panel, 0, -2);
	cannon->SetPosition2D(-14.47, -3.39);
	CreateSound(ambientSound);
	Node* floor = CreateInteractable("floor", "Levels/Level_007/Objects/Spr_WoodenGate01.png", 1.02, -16.16, 6.65, -89.53);
	floor->RemoveComponent<StaticSprite2D>();
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
	//TileMapLayer2D* coinsLayer = tileMap->GetLayer(tileMap->GetNumLayers() - 2);
	//levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_), 6);
}
void Level7::HandleInteraction(Viking* viking, Interactable* interactable) {
	Node* other = interactable->GetNode();
	//URHO3D_LOGINFO("AVAILABLE VIKINGS " + String(Cannon::instance->availableVikings[0]));
	if (true)
	{
		if (viking->GetNode()->GetPosition2D().y_ < -6.5)//flowers
		{
			DelayedLose();
		}
		viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
		viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
		URHO3D_LOGINFO("calling createcomponent on hitter");
		viking->GetNode()->CreateComponent<Interactable>();
		URHO3D_LOGINFO("calling removecomponent on self");
		other->RemoveComponent<Interactable>();
		URHO3D_LOGINFO("calling removecomponent on viking");
		//viking->GetNode()->RemoveComponent<Viking>();
		//check if viking close enough to cannon or trigger next to it
		Vector2 targetPos(-12.16, -5.52);
		float distance = (other->GetPosition2D() - targetPos).Length();
		float minWinDistance = 4;
		if (distance <= minWinDistance)
		{
			DelayedWin();
		}
	}
	CreateSound("08_heavyhitobject.ogg");

	CheckLose();
}