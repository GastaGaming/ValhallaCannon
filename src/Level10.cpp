#include "Level10.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level10::Level10(Context* context) :Level(context)
{
}

Level10::~Level10()
{
}
void Level10::Init() {
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_010/Level_010.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale(1.74*1.04);
	tileMapNode->SetPosition2D(-35.44, -15.32);
	SetLayer(tileMapNode, -3, 0, true);
	// bg
	background = CreateSprite("background", "Levels/Level_010/Level_010_BgLayer.png", 0, 0, 2.14);
	SetLayer(background, -5,0);
	background->SetEnabled(UIManager::bgOn);

	Node* tower = CreateSprite("tower", "Levels/Level_010/Objects/Spr_TowerWindowPart01.png", 12.07, 4.43, 1.83, 0);
	SetLayer(tower,-3,0);
	Node* tree = CreateSprite("tree", "Levels/Level_010/Objects/Spr_Trees.png", -4.38, 1.11, 1.97, 0);
	Node* treesidebot = CreateSprite("treesidebot", "Levels/Level_010/Objects/Spr_TreeSideTop.png", -3.46, 6.85, 1.97, 0);
	Node* treesidetop = CreateSprite("treesidetop", "Levels/Level_010/Objects/Spr_TreeSideBot.png", -2.76, -1.55, 1.97, 0);
	SetLayer(tree, -4, 0);
	SetLayer(treesidetop, 0, 0);
	SetLayer(treesidebot, 0, 0);

	Node* treeColTop = CreateInteractable("TreeTop", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -4.87, 10.34, 1);
	treeColTop->GetComponent<CollisionBox2D>()->SetSize(0.2, 2);
	SetLayer(treeColTop, -10, 213);
	Node* treeColMid = CreateInteractable("TreeMiddle", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -4.76, 3.51, 1);
	treeColMid->GetComponent<CollisionBox2D>()->SetSize(0.2, 2);
	SetLayer(treeColMid, -10, 213);
	Node* treeColBot = CreateInteractable("TreeBottom", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -3.61 - 1.3, -8.7 + 0.55, 1);
	treeColBot->GetComponent<CollisionBox2D>()->SetSize(0.2, 5);
	SetLayer(treeColBot, -10, 213);

	windowCol = CreateInteractable("windowCol", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", 12.07, 1.95, 2.15);
	windowCol->RemoveComponent<StaticSprite2D>();
	windowCol->SetEnabled(false);

	booth = CreateInteractable("booth", "Levels/Level_010/Objects/Srp_PrincessBooth01.png", 3.61, -6.55, 1.84, 0);

	Node* gate = CreateSprite("gate", "Levels/Level_010/Objects/Spr_Gate01.png", 12.38, -5.58, 1.91, 0);
	SetLayer(gate, 1, 0);
	Node* flag = CreateSprite("flag", "Levels/Level_008/Objects/Spr_WallBanner01.png", -16.18, -3.94, 1.49, 0);
	SetLayer(flag, 1, 0);
	Node* banner = CreateSprite("banner", "Levels/Level_008/Objects/Spr_SmallFlag01.png", 16.89, -0.52, 1.97, 0);
	SetLayer(banner, 1, 0);

	owl = CreateAnimatedInteractable("owl", "Owl/Anim_Owl01.scml", "Idle", -4.81, -4.54, 1.44, 1.44);
	owl2 = CreateAnimatedInteractable("owl2", "Owl/Anim_Owl01.scml", "Idle", -4.81, 5.68, 1.44, 1.44);
	SetLayer(owl, 0, 1);
	CreateSound(ambientSound);
	CreateShip(-17.66, 5.16);
	SetSticky({treeColBot,treeColMid,treeColTop});
	princess = CreateAnimatedSprite("princess", "Characters/Princess/EnemyAnimation.scml", "Idle", 12.02, 0.11);
	SetLayer(princess,-5,1);
	lock = CreateInteractable("lock","Levels/Level_008/Objects/Spr_Lock.png", 11.02, 1.52);

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
void Level10::HandleInteraction(Viking* viking, Interactable* interactable) {
	Node* other = interactable->GetNode();
	String oName = other->GetName();
	switch (viking->type)
	{
	case VikingType::Heavy:
		if (other == owl)
		{
			viking->GetNode()->SetEnabled(false);
		}
		if (other == booth)
		{
			MyTimer::New([=]() {
				viking->SetEnabled(false);
			},1);
		}
		break;
	case VikingType::Rogue:
		if (other == owl || other == owl2)
		{
			//bottom owl
			SetAnimation(owl, "Flight");
			float flightTime = 0.8;
			Vector2 target1 = Vector2(-4.81, 5.68);
			if (owl->GetPosition2D().y_ >= target1.y_)
			{
				target1 = target1 + Vector2::ONE * 10;
			}
			Lerppa::LerpThis(owl,target1,flightTime);
			MyTimer::New([=]() {SetAnimation(owl, "Idle"); }, flightTime);
			viking->GetNode()->SetEnabled(false);

			//top owl
			SetAnimation(owl2,"Flight");
			Vector2 target2 = Vector2(-8.81, 10.68);
			if (owl2->GetPosition2D().y_ >= target2.y_)
			{
				target2 = target2 + Vector2::ONE * 10;
			}
			Lerppa::LerpThis(owl2, target2, flightTime);
			MyTimer::New([=]() {SetAnimation(owl2, "Idle"); }, flightTime);
		}
		if (other == booth)
		{
			auto* rigid = viking->GetComponent<RigidBody2D>();
			if (rigid)
			{
				rigid->SetLinearVelocity(Vector2::UP * 10 + Vector2::RIGHT * 7);
			}
		}
		if (other == lock)
		{
			MyTimer::New([=]() {
				windowCol->SetEnabled(true);
			}, 2);
			lock->SetEnabled(false);
			CreateSound("06_roguelockopen.ogg");
			viking->GetNode()->SetEnabled(false);
		}
		if (other == windowCol)
		{
			DelayedWin();
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
		}
		break;
	case VikingType::Torcher:
		if (other->GetName().Contains("tree",false))
		{
			CreateEffect(Vector3(-4.76, 3.51, 0), "Flame.xml", 9);
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			DelayedLose();
		}
		if (other == booth)
		{
			CreateEffect(booth, "Flame.xml", 2);
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			DelayedLose();
		}
		break;
	}
	CheckLose();
}