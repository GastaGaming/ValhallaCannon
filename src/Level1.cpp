#include "Level1.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level1::Level1(Context* context) :Level(context)
{
}


Level1::~Level1()
{
}
void Level1::Init() {
	URHO3D_LOGINFO("LVL1 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	//gate

	//tree
	Node* treeNode = CreateInteractable("Tree", "Levels/Level_001/Objects/Spr_Tree01.png", 5.16, -3.44);
	treeNode->Scale(Vector3::ONE * 1.7);
	treeNode->GetComponent<CollisionBox2D>()->SetSize(0.1,4);

	//rock
	CreateInteractable("Rock", "Levels/Level_001/Objects/Spr_Rock01.png", 12.61, -4.69, 1.45);

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_001/Level_001.tmx")); 
	tileMapNode->SetScale2D(Vector2::ONE*2.14*0.82);
	tileMapNode->SetPosition(Vector3(-33.21, -15.79, 0));
	tileMap->GetLayer(0)->SetDrawOrder(-5);
	SetLayer(tileMapNode, -2, -5, true);
	// bg
	background = scene_->CreateChild("background");
	StaticSprite2D* bgSprite = background->CreateComponent<StaticSprite2D>();
	bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/LevelBGs/FirstLevelBG01.png"));
	background->SetScale2D(2.03, 2.03);
	bgSprite->SetLayer(-5);
	bgSprite->SetOrderInLayer(214);
	background->SetEnabled(UIManager::bgOn);//clouds
	CreateSprite("cloud1", "Spr_Cloud01.png", -2.44, 7.38, 2.13, 179.7);
	CreateSprite("cloud2", "Spr_Cloud02.png", 13.27, 6.88, 2.87, 178.41);
	//CreateEffect(Vector3::ZERO, "Flame.xml", 1);
	/*CreateEffect(Vector3::RIGHT*-10, "Torch.xml", 1);
	CreateEffect(Vector3::RIGHT*-5 + Vector3::DOWN * 5, "Burning.xml", 2);
	CreateEffect(Vector3::RIGHT*-15 + Vector3::DOWN * 5, "GroundDust.xml", 1,3);*/

//CreateSound(ambientSound);

	Node* rogue = node_->CreateChild("rogue");
	rogue->SetScale2D(0.22, 0.22);
	//rogue->SetPosition2D(12.61, -4.69);
	rogue->SetPosition2D(12.61, -6.3);
	auto* anim = rogue->CreateComponent<AnimatedSprite2D>();
	anim->SetAnimationSet(MyCache->GetResource<AnimationSet2D>("Urho2D/Anim_Rogue/Anim_Rogue.scml"));
	anim->SetAnimation("HelgaHold");
	rogue->SetRotation2D(180);
	anim->SetOrderInLayer(-1);

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
	//levelParser_->PopulateCoins(coinsLayer);
}
void Level1::HandleInteraction(Viking* viking, Interactable* interactable) {
	auto* cache = GetSubsystem<ResourceCache>();
	if (viking->type == VikingType::Torcher) {
		if (interactable->GetNode()->GetName() == "Tree")
		{
			Node* interactableNode = interactable->GetNode();
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);

			interactable->GetComponent<StaticSprite2D>()->SetSprite(cache->GetResource<Sprite2D>("Urho2D/Levels/Level_001/Objects/Spr_BurntTree01.png"));
			viking->GetNode()->SetEnabled(false);

			//fire
			CreateEffect(interactableNode, "Flame.xml");
			//smoke
			CreateEffect(interactableNode, "SmokeTestingP.xml")->GetNode()->Translate(Vector3::FORWARD);
			//sound
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
		}
	}
	else if (viking->type == VikingType::Heavy) {
		if (interactable->GetNode()->GetName() == "Rock")
		{
			//interactable->GetNode()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			DelayedWin();
			CreateSound("08_heavyhitobject.ogg");
			Lerppa::LerpThis(interactable->GetNode(), Vector2(12, -15), 1);

		}
		else if (interactable->GetNode()->GetName() == "Tree")
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->CreateComponent<Interactable>();
			//viking->GetNode()->RemoveComponent<Viking>();
			CreateSound("08_heavyhitobject.ogg");
		}
	}
	if (viking->type == interactable->weaknessType)
	{
		//node_->SetEnabled(false);

		//orc and sticky
		/*AnimatedSprite2D* anim = interactable->GetComponent<AnimatedSprite2D>();
		if (anim)
		{
			anim->SetAnimation("dead");
		}
		viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
		URHO3D_LOGINFO("calling createcomponent on hitter");
		viking->GetNode()->CreateComponent<Interactable>();
		URHO3D_LOGINFO("calling removecomponent on self");
		interactable->GetNode()->RemoveComponent<Interactable>();
		URHO3D_LOGINFO("calling removecomponent on viking");
		viking->GetNode()->RemoveComponent<Viking>();*/
	}
	//if won return before
	CheckLose();
}
