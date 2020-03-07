#include "Level8.h"
#include "Cannon.h"
#include "UIManager.h"
#include "MyAnimator.h"
#include "LevelParser.h"

Level8::Level8(Context* context) :Level(context)
{
}

Level8::~Level8()
{
}
void Level8::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL5 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_008/Level_008_animwater.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale(1.74);
	tileMapNode->SetPosition2D(-32.19, -14.41);
	SetLayer(tileMapNode, 0, 0, true);

	//setting tilemap on multiple layers for water
	Node* tileRoot = tileMapNode->GetChildren()[0];
	SetLayer(tileRoot->GetChildren()[0], 0, 2, true);
	SetLayer(tileRoot->GetChildren()[1], 0, 4, true);
	SetLayer(tileRoot->GetChildren()[2], 0, 6, true);
	SetLayer(tileRoot->GetChildren()[3], 0, 8, true);

	// bg
	background = CreateSprite("background", "Levels/Level_008/Level_008_BgLayer.png", 0, 0, 2.14);
	SetLayer(background, -5, 214);
	background->SetEnabled(UIManager::bgOn);

	bridge = CreateInteractable("bridge", "Levels/Level_008/Objects/Spr_DrawBridge.png", 9.69, -1.08, 2.44, -61.57);
	//Node* bridgeVisual = CreateAnimatedSprite("bridgeVisual", "Levels/Level_008/Objects/Drawbridge.scml", "NewAnimation", 0, 0);
	chain = CreateSprite("chain", "Levels/Level_008/Objects/Spr_DrawbridgeChain01.png", 15.63, 4.25, 1.8, 8.4);
	SetLayer(chain, 0, -1);
	launder = CreateSprite("launder", "Levels/Level_008/Objects/Spr_Launder.png", -4.57, -6.57, 1.43, 0);
	launderer = CreateAnimatedInteractable("launderer", "Characters/Launderer/EnemyAnimation2.scml", "Idle", -1.1, -5.82, 0.88, 0);
	SetLayer(launderer, 0, 3);
	lock = CreateInteractable("lock", "Levels/Level_008/Objects/Spr_Lock.png", 9.9, 2.6, 1.36, 0);
	SetLayer(lock, 0, 1);
	Node* flag = CreateSprite("flag", "Levels/Level_008/Objects/Spr_SmallFlag01.png", 13.88, 8.69, 1.84, 0);
	Node* flag2 = CreateSprite("flag", "Levels/Level_008/Objects/Spr_SmallFlag01.png", 16.13, 6.44, 1.84, 0);
	Node* banner = CreateSprite("banner", "Levels/Level_008/Objects/Spr_WallBanner01.png", 15.41, 0.78, 1.8, 0);
	SetLayer(banner, 0, 215);
	Node* wallgate = CreateSprite("wallgate", "Levels/Level_008/Objects/Spr_WallGate01.png", 11.74, -0.33, 1.84, 0);
	CreateShip(-16.88, -3.58);
	Node* foreground = CreateSprite("foreground", "Levels/Level_008/Objects/Spr_ForeGround03.png", 1.24, 9.41, 2.47, 0);
	SetLayer(foreground, 0, 1);
	CreateEffect(launder, "SoapBubble.xml");
	Node* castle = CreateObject("castle", "Levels/Level_008/Objects/Spr_Launder.png", 20.13, -2.13, 6.11, 0);
	castle->RemoveComponent<StaticSprite2D>();
	Node* castle2 = CreateObject("castle2", "Levels/Level_008/Objects/Spr_Launder.png", 13.52, 6.75, 0.81, 0);
	castle2->RemoveComponent<StaticSprite2D>();
	CreateSound(ambientSound);
	//CreateAnimatedInteractable("animBridge", "Levels/Level_008/Objects/Drawbridge/Drawbridge.scml", "NewAnimation", 0, 0);
	croc1 = CreateAnimatedInteractable("croc1", "Characters/Crocodile/Crocodile_anims.scml", "Idle", 11.61, -7.66, 0.5);
	croc2 = CreateAnimatedInteractable("croc2", "Characters/Crocodile/Crocodile_anims.scml", "Idle", 5.16, -8.02, 0.5);//flipped
	croc2->SetScale2D(-0.5, 0.5);
	SetLayer(croc2, 0, 3);
	SetLayer(croc1, 0, 3);
	SetSticky({ bridge });
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
	levelParser_->PopulateCoins(coinsLayer, Vector2(tileMapNode->GetPosition().x_, tileMapNode->GetPosition().y_), 8);

}
void Level8::HandleInteraction(Viking* viking, Interactable* interactable) {
	Node* other = interactable->GetNode();
	String oName = other->GetName();
	//URHO3D_LOGINFO("AVAILABLE VIKINGS " + String(Cannon::instance->availableVikings[0]));
	switch (viking->type)
	{
	case VikingType::Heavy:
		if (oName == "launderer")
		{
			//kill the crocs with soap
			//other->SetEnabled(false);
			launder->SetEnabled(false);
			launder->SetPosition2D(Vector2::DOWN*100);
			//bubble effect into pit
			CreateEffect(Vector3(7.65, -7.7), "SoapBubble.xml");
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
			CreateSound("08_heavyhitobject.ogg");
			viking->GetNode()->SetEnabled(false);
			SetAnimation(launderer, "Fall");
			float sideTime = 0.5;
			float interval = 0.2;
			float downTime = 0.2;
			Lerppa::LerpThis(launderer, Vector2(6.89, -5.76), sideTime);
			MyTimer::New([=]() {
				Lerppa::LerpThis(launderer, Vector2(6.89, -10.76), downTime);
				MyTimer::New([=]() {
					Lerppa::LerpThis(croc1, croc1->GetPosition2D() + Vector2::DOWN * 5, 1);
					Lerppa::LerpThis(croc2, croc2->GetPosition2D() + Vector2::DOWN * 5, 1);
					MyTimer::New([=]() {
						croc1->SetEnabled(false);
						croc2->SetEnabled(false);
						launderer->SetEnabled(false);
					}, 1);
				}, sideTime + interval + 0.3);
			}, sideTime + interval);
		}
		if (other == lock)
		{
			viking->GetNode()->SetEnabled(false);
		}
		if (other == bridge)
		{
			CrocEat(viking->GetNode());
		}
		break;
	case VikingType::Rogue:
		if (oName == "lock")
		{
			SetAnimation(viking->GetNode(), "Collision");
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->SetPosition2D(8.22, 2.69);
			MyTimer::New([=]() {
				viking->GetNode()->SetEnabled(false);
			}, 0.3);
			if (croc1->IsEnabled())
			{
				CrocEat(viking->GetNode());
				/*SetAnimation(croc2, "Snap");
				float snapTime = 1;
				Vector2 snapPos = Vector2(8.2, -2.21);
				Vector2 originalPos = croc2->GetPosition2D();
				Lerppa::LerpThis(croc2, snapPos, 0.2);
				MyTimer::New([=]() {
					SetAnimation(croc2, "Withdraw");
					Lerppa::LerpThis(croc2, originalPos, 1);
					MyTimer::New([=]() {
						SetAnimation(croc2, "Idle");
					}, 1);
				}, 1);*/
			}
			else
			{
				lock->SetEnabled(false);
				//unlock drawbridge animation
				CreateSound("06_roguelockopen.ogg");
				viking->GetNode()->SetEnabled(false);
				DelayedWin();
				Lerppa::LerpThis(bridge,Vector2(6.91, -6.19),0.2,true,10.75);
				Lerppa::LerpThis(chain, Vector2(8.01, -1.57), 0.2, true, 41.83);
			}
		}
		break;
	case VikingType::Torcher:
		if (oName == "launderer" || oName == "launder")
		{
			//burn launderers & soap
			CreateEffect(launder, "Flame.xml", 1.5);
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			viking->GetNode()->SetEnabled(false);
			DelayedLose();
		}
		if (other == bridge)
		{
			//burn bridge & lose
			CreateEffect(bridge, "Flame.xml", 1.5);
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			viking->GetNode()->SetEnabled(false);
			DelayedLose();

		}
		if (other == lock)
		{
			viking->GetNode()->SetEnabled(false);
		}
		break;
	default:
		break;
	}
	if (oName == "castle" || oName == "castle2")
	{
		viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
	}
	CheckLose();
}
void Level8::CrocEat(Node* target, Vector2 offset) {
	offset += Vector2::DOWN * 4.48;
	SetAnimation(croc2, "Snap");
	float snapTime = 1;
	Vector2 snapPos = target->GetPosition2D()+offset;
	Vector2 originalPos = croc2->GetPosition2D();

	//lerp the croc into snap position
	Lerppa::LerpThis(croc2, snapPos, 0.2);
	//lerp it back after snap
	MyTimer::New([=]() {
		SetAnimation(croc2, "Withdraw");
		Lerppa::LerpThis(croc2, originalPos, 1);
		//start idling after withdraw
		MyTimer::New([=]() {
			SetAnimation(croc2, "Idle");
		}, 1);
	}, 1);
	//disable the target because it gets eaten
	MyTimer::New([=]() {
		target->SetEnabled(false);
	}, 0.3);
}