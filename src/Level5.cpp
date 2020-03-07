#include "Level5.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level5::Level5(Context* context) :Level(context)
{
}

Level5::~Level5()
{
}
Node* enemy1;
Node* enemy2;
Node* lock;
Node* roof;
Node* gate;
void Level5::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL5 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	//gate
	
	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_005/Level_005.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale(1.74);
	tileMapNode->SetPosition2D(-32.19, -14.41);
	SetLayer(tileMapNode,0,-1,true);

	// bg
	background = scene_->CreateChild("background");
	StaticSprite2D* bgSprite = background->CreateComponent<StaticSprite2D>();
	bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/LevelBGs/FifthLevelBG01.png"));
	background->SetScale2D(2.14, 2.14);
	bgSprite->SetLayer(-5);
	bgSprite->SetOrderInLayer(214);
	background->SetEnabled(UIManager::bgOn);

	//fg
	Node* foreground = CreateSprite("foreground", "Levels/Level_005/Objects/Spr_ForeGround01.png", -0.05, 7.41, 1.86);
	SetLayer(foreground, 99, 99);

	Node* ship = CreateSprite("ship", "Spr_ShipSailsDown.png", -19.19, -3.47, 0.85);

	gate = CreateInteractable("Gate", "Levels/Level_002/Objects/Spr_MetalGate01.png", -1.41, -2.16, 2.46);
	lock = CreateInteractable("lock", "Levels/Level_002/Objects/Spr_Lock.png", -3.13, -4.44, 2.08);
	SetLayer(lock, 1, 1);
	roof = CreateSprite("roof", "none", 15.36, -2.16);
	roof->CreateComponent<CollisionBox2D>()->SetSize(6,2);
	roof->CreateComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
	roof->CreateComponent<Interactable>();
	enemy1 = CreateAnimatedInteractable("villager1", "Characters/Villager1/EnemyAnimation2.scml", "Idle", 4.08, -5.63);
	enemy2 = CreateAnimatedInteractable("villager2","Characters/Villager2/EnemyAnimation2.scml","Idle", 8.54, -6.04);
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
	//TileMapLayer2D* coinsLayer = tileMap->GetLayer(tileMap->GetNumLayers() - 2);
	//levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_));
}
void Level5::HandleInteraction(Viking* viking, Interactable* interactable) {
	
	Node* otherNode = interactable->GetNode();

	if (viking->type == VikingType::Rogue  )	
	{
		if (otherNode == lock) 
		{
			lock->SetEnabled(false);
			CreateSound("06_roguelockopen.ogg");
			Lerppa::LerpThis(gate, Vector2(-1.41, -25), 1.5);
		}
		else 
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
		}
	}
	else if (viking->type == VikingType::Heavy)
	{
		if (otherNode == enemy1 || otherNode == enemy2)
		{
			CreateSound("08_heavyhitobject.ogg");
			enemy1->SetEnabled(false);
			enemy2->SetEnabled(false);
		}
	}
	if (viking->type == VikingType::Torcher)
	{
		if (otherNode == roof) 
		{


			//fire pls
			CreateEffect(otherNode, "Flame.xml", 9);
			//smoke
			CreateEffect(otherNode, "SmokeStack.xml", 3, true)->GetNode()->Translate(Vector3::FORWARD);

			//sound
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			DelayedWin();
		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");

		}
	}
	CheckLose();
}