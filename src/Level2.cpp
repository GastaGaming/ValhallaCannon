#include "Level2.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level2::Level2(Context* context) :Level(context)
{
}


Level2::~Level2()
{
}
TileMap2D* tileMap;
void Level2::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL2 Init");
	//creating the level, setting dependencies etc..
	
	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	//gate
	gate = CreateInteractable("Gate", "Levels/Level_002/Objects/Spr_MetalGate01.png", -1.41, -2.16);
	gate->SetScale2D(2.46, 2.46);
	SetLayer(gate, -5, 303);
	// bg
	background = scene_->CreateChild("background");
	StaticSprite2D* bgSprite = background->CreateComponent<StaticSprite2D>();
	bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/LevelBGs/SecondLevelBG01.png"));
	background->SetScale2D(2.14, 2.14);
	bgSprite->SetLayer(-5);
	bgSprite->SetOrderInLayer(214);
	background->SetEnabled(UIManager::bgOn);

	//tilemap
	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	//tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Scenes/Karttta_test.tmx")); // Assign tmx resource file to component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_002/Level_002_animwater.tmx"));
	tileMapNode->SetScale2D(2.83,2.83);
	//tileMapNode->SetPosition(Vector3(-16, -6, 0));
	tileMapNode->SetPosition(Vector3(-62.22, -17.9, 0));
	tileMap->GetLayer(0)->SetDrawOrder(-5);
	//for (size_t i = 0; i < tileMap->GetNumLayers(); i++)
	//{
	//	TileMapLayer2D* layer = tileMap->GetLayer(i);
	//	layer->SetAnimationEnabled(true);
	//	float t = (int)MyTime->GetElapsedTime() % 5;
	//	layer->SetAnimationTime(t);
	//	//tileMap->GetLayer(i)->
	//	layer->SetAnimationTime(t*100);
	//	//layer->
	//	URHO3D_LOGINFO("tile layer " + String(i) + " named " + String(layer->GetNode()->GetName()) +" animation enabled = " +  String(layer->GetAnimationEnabled()));
	//	if (layer->GetTileMap()->)
	//	{
	//	URHO3D_LOGINFO("ANIM " + layer->GetObjectAnimation()->GetName());
	//	}
	//} 
	//File save(context_, MyFileSystem->GetProgramDir() + "Data/Scenes/levelsave.xml",FILE_WRITE);
	//MyTools::currentScene->SaveXML(save);
	//water
	/*Node* water1 = CreateSprite("water1", "Spr_Water01.png", 12.66, -8.94);
	SetLayer(water1,0,-1);
	Node* water2 = CreateSprite("water2", "Spr_Water02.png", 12.66, -9.08);
	Node* water3 = CreateSprite("water3", "Spr_Water03.png", 12.61, -9.72);*/

	rope = CreateSprite("rope", "Levels/Level_008/Objects/Spr_DrawbridgeChain01.png", 17.65, -6.4, 2.76, 0);
	SetLayer(rope, 0, -1);
	Node* knot = CreateInteractable("knot", "Levels/Level_002/Objects/Spr_Lock.png", 4.29, -7.63, 2, 0);
	SetLayer(knot, 0, 2);
	rope->SetParent(knot);
	Node* ship = CreateInteractable("ship", "Spr_ShipSailsDown.png", 15.25, -4.22, 1.27);
	ship->GetComponent<CollisionBox2D>()->SetSize(ship->GetComponent<CollisionBox2D>()->GetSize().x_, ship->GetComponent<CollisionBox2D>()->GetSize().y_ - 5);
	lock = CreateInteractable("lock", "Levels/Level_002/Objects/Spr_Lock.png", -3.13, -4.44, 2.08, 0);
	SetLayer(lock, 0, 1);
	Node* woodpillar = CreateSprite("woodPillar", "Levels/Level_002/Objects/Spr_MetalPillar01.png", 4.71, -8.06, 1.25, 0);
	SetLayer(woodpillar,0, -2);
	CreateSound(ambientSound);  WeakPtr<LevelParser> levelParser_;

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
	levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_), 2);
}
void Level2::HandleInteraction(Viking* viking, Interactable* interactable) {
	Node* interactableNode = interactable->GetNode();
	if (viking->type == VikingType::Rogue)
	{
		if (interactableNode == lock)
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			CreateSound("06_roguelockopen.ogg");
			viking->GetNode()->SetEnabled(false);
			lock->SetEnabled(false);
			gate->RemoveComponent<RigidBody2D>();
			Lerppa::LerpThis(gate,Vector2(-1,-15),1);
		}
		else if (interactable->GetNode()->GetName() == "knot")
		{
			interactable->GetNode()->SetEnabled(false);
			DelayedWin();//return so you cant also lose?
			viking->GetNode()->SetEnabled(false);
			Lerppa::LerpThis(rope, Vector2(30, 0), 1);
		}
		/*else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
		}*/
	}
	if (viking->type == VikingType::Torcher)
	{
		
		if (interactable->GetNode()->GetName() == "ship")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			ResourceCache* cache = GetSubsystem<ResourceCache>();
			CreateEffect(interactableNode, "Flame.xml", 3);
			CreateEffect(interactableNode, "SmokeStack.xml", 1)->GetNode()->Translate(Vector3::FORWARD);
			//CreateSound(interactableNode, "BigExplosion.wav");
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			viking->GetNode()->SetEnabled(false);
			Lerppa::LerpThis(interactable->GetNode(), Vector2(15.25, -15), 1);
			DelayedLose();
		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
		}
	}
	if (viking->type == VikingType::Heavy)
	{

		if (interactable->GetNode()->GetName() == "ship")
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->CreateComponent<Interactable>();
			interactable->GetNode()->RemoveComponent<Interactable>();
			//viking->GetNode()->RemoveComponent<Viking>();
		}
	}
	CheckLose();
}
void Level2::Update(float deltaTime) {
	/*for (size_t i = 0; i < tileMap->GetNumLayers(); i++)
	{
		TileMapLayer2D* layer = tileMap->GetLayer(i);
		//layer->SetAnimationEnabled(true);
		float t = (int)MyTime->GetElapsedTime() % 5;
		layer->SetAnimationTime(t);
		//tileMap->GetLayer(i)->
		layer->SetAnimationTime(t * 100);
		//layer->
		//URHO3D_LOGINFO("tile layer " + String(i) + " named " + String(layer->GetNode()->GetName()) + " animation enabled = " + String(layer->GetAnimationEnabled()));

	}*/
}

