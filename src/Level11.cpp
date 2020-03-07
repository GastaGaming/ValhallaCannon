#include "Level11.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level11::Level11(Context* context) :Level(context)
{
}

Level11::~Level11()
{
}
void Level11::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL11 Init");
	UI* ui = GetSubsystem<UI>();
	auto* cache = GetSubsystem<ResourceCache>();

	tick1 = static_cast<Sprite*>(ui->GetRoot()->GetChild("tick1",true));
	tick1->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOff.png"));

	tick1->SetVisible(true);
	tick2 = static_cast<Sprite*>(ui->GetRoot()->GetChild("tick2", true));
	tick2->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOff.png"));

	tick2->SetVisible(true);
	tick3 = static_cast<Sprite*>(ui->GetRoot()->GetChild("tick3", true));
	tick3->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOff.png"));

	tick3->SetVisible(true);
	//creating the level, setting dependencies etc..
	burned = false;
	watered = false;
	smashed = false;
	//pasta from main
	Level::Init();
	Scene* scene_ = MyTools::currentScene;

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_011/Level_011.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale(1.74);
	tileMapNode->SetPosition2D(-33.55, -14.41);
	SetLayer(tileMapNode, 0, 0, true);
	// bg
	background = CreateSprite("background", "Levels/Level_011/Level_011_BgLayer.png", 0, 0, 2.14);
	SetLayer(background, -5, 214);
	background->SetEnabled(UIManager::bgOn);
	roofRack = CreateInteractable("RoofRack", "Levels/Level_011/Objects/Spr_RoofRack.png", 0, 5.94, 1.98, 0);
	CreateInteractable("Dinner", "Levels/Level_011/Objects/Spr_Dinner.png", 4.19, -3.99, 1.97, -0);
	scene_->GetChild("Dinner")->GetComponent<CollisionBox2D>()->SetSize(2, 1);
	chain = CreateSprite("chain", "Levels/Level_008/Objects/Spr_DrawbridgeChain01.png", -12, 12.83, 2.07, 126.71);
	CreateInteractable("Sink", "Levels/Level_011/Objects/Spr_Sink01.png", -5.76, -5.49, 2.03,0);
	scene_->GetChild("Sink")->GetComponent<CollisionBox2D>()->SetSize(3.4, 1.5);
	scene_->GetChild("Sink")->GetComponent<CollisionBox2D>()->SetCenter(0, -0.75);
	stove = CreateInteractable("Stove", "Levels/Level_011/Objects/Spr_Stove01.png", 8.2, -5.98, 1.62,0);
	scene_->GetChild("Stove")->GetComponent<CollisionBox2D>()->SetSize(2, 5);
	CreateEffect(stove, "Flame.xml",1);
	SetLayer(chain, 0, -1);
	pan1 = CreateInteractable("Pan1", "Levels/Level_011/Objects/Spr_Pan.png", -5.46, 2.41, 1.21, 0);
	pan2 = CreateInteractable("Pan2", "Levels/Level_011/Objects/Spr_Pan02.png", -3.69, 2.08, 1.42, 0);
	SetLayer(pan1, 0, -1);
	SetLayer(pan2, 0, -1);
	cook1 = CreateAnimatedInteractable("Cook1", "Characters/Cook/CookAnimations.scml", "AngryIdle1", 11.88, -5.82, 1);
	cook2 = CreateAnimatedInteractable("Cook2", "Characters/Cook/CookAnimations.scml", "AngryIdle2", 14.11, -5.82, 1);
	cook3 = CreateAnimatedInteractable("Cook3", "Characters/Cook/CookAnimations.scml", "AngryIdle3", 16.49, -5.82, 1);

	lock = CreateInteractable("lock", "Levels/Level_008/Objects/Spr_Lock.png", -7.87, 6.21, 1.36, 0);
	SetLayer(lock, 0, 1);

	rack1 = CreateSprite("RackHolder1", "Levels/Level_011/Objects/Spr_RackHolder.png", -5.63, 3.86, 1, 0);
	rack2 = CreateSprite("RackHolder2", "Levels/Level_011/Objects/Spr_Rackholder.png", -3.83, 3.86, 1, 0);

	SetLayer(roofRack, 0, 1);
	CreateShip(-16.88, -3.58);
	//MyTools::currentScene->SetTimeScale(0.2f);
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
	//levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_), 6);
}
void Level11::HandleInteraction(Viking* viking, Interactable* interactable) {
	Node* other = interactable->GetNode();
	String oName = other->GetName();
	auto* cache = GetSubsystem<ResourceCache>();
	//URHO3D_LOGINFO("AVAILABLE VIKINGS " + String(Cannon::instance->availableVikings[0]));
	switch (viking->type)
	{
	case VikingType::Heavy:
		if (oName == "Pan1" || oName == "Pan2")
		{
			Lerppa::LerpThis(pan1, Vector2(-5.46, -10), 1);
			pan1->GetComponent<RigidBody2D>()->SetEnabled(false);
			Lerppa::LerpThis(pan2, Vector2(-3.69, -10), 1);
			pan2->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			watered = true;
			tick2->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOn.png"));
			cook1->GetComponent<AnimatedSprite2D>()->SetAnimation("SadIdle1");
		}
		else if(oName == "Dinner")
		{
			viking->GetNode()->SetEnabled(false);
		}
		else if (oName == "Stove")
		{
			CreateSound("07_torcherfire.ogg");
			viking->GetNode()->SetEnabled(false);
		}
		break;
	case VikingType::Rogue:
		if (oName == "lock")
		{
			lock->SetEnabled(false);
			Lerppa::LerpThis(chain, Vector2(-15, 20), 1);
			//unlock drawbridge animation
			smashed = true;
			tick3->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOn.png"));

			cook3->GetComponent<AnimatedSprite2D>()->SetAnimation("SadIdle3");
			Lerppa::LerpThis(roofRack, Vector2(-0.3, -3.56), 0.25);
			Lerppa::LerpThis(rack1, Vector2(-5.46, -3.56), 0.25);
			Lerppa::LerpThis(rack2, Vector2(-3.69, -3.56), 0.25);
			Lerppa::LerpThis(pan1, Vector2(-5.46, -10), 0.25);
			Lerppa::LerpThis(pan2, Vector2(-3.69, -10), 0.25);


		}
		else if (oName == "Stove")
		{
			CreateSound("07_torcherfire.ogg");
			viking->GetNode()->SetEnabled(false);
		}
		break;
	case VikingType::Torcher:
		if (oName == "Dinner")
		{
			// Add burn tickbox
			CreateEffect(other, "Flame.xml");
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			viking->GetNode()->SetEnabled(false);
			burned = true;
			tick1->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOn.png"));
			cook2->GetComponent<AnimatedSprite2D>()->SetAnimation("SadIdle2");
		}
		else if (oName == "Stove") 
		{
			CreateSound("07_torcherfire.ogg");
			viking->GetNode()->SetEnabled(false);
		}
		break;
	default:
		break;
	}
	if (burned == true && watered == true && smashed == true) 
	{
		cook1->GetComponent<AnimatedSprite2D>()->SetAnimation("SadWalk1");
		Lerppa::LerpThis(cook1, Vector2(20, -5.82), 1);
		cook2->GetComponent<AnimatedSprite2D>()->SetAnimation("SadWalk2");
		Lerppa::LerpThis(cook2, Vector2(22, -5.82), 1);
		cook3->GetComponent<AnimatedSprite2D>()->SetAnimation("SadWalk3");
		Lerppa::LerpThis(cook3, Vector2(24, -5.82), 1);
		tick1->SetVisible(false);
		tick2->SetVisible(false);
		tick3->SetVisible(false);
		DelayedWin();
	}
	CheckLose();
}