#include "Level9.h"
#include "Cannon.h"
#include "UIManager.h"
#include "LevelParser.h"

Level9::Level9(Context* context) :Level(context)
{
}

Level9::~Level9()
{
}
void Level9::Init() {
	//Level::Init(context);
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;

	SharedPtr<Node> tileMapNode(scene_->CreateChild("TileMap")); // Create a standard Urho3D node
	TileMap2D* tileMap = tileMapNode->CreateComponent<TileMap2D>(); // Create the TileMap2D component
	tileMap->SetTmxFile(cache->GetResource<TmxFile2D>("Urho2D/Levels/Level_009/Level_009.tmx")); // Assign tmx resource file to component
	tileMapNode->SetScale(1.74);
	tileMapNode->SetPosition2D(-32.19, -14.41);
	SetLayer(tileMapNode, 0, 0, true);

	// bg
	background = CreateSprite("background", "Levels/Level_009/Level_009_BgLayer.png", 0, 0, 2.14);
	SetLayer(background, -5, 214);
	background->SetEnabled(UIManager::bgOn);

	//fg
	Node* foreground = CreateSprite("foreground", "Levels/Level_009/Objects/Spr_ForeGround02.png", 1.76, 8.6, 2.31, 0);
	SetLayer(foreground, 0, 1);

	merchant = CreateAnimatedInteractable("merchant", "Characters/Merchant/EnemyAnimation2.scml","Idle", 13.78, -6.1, 0.77);
	SetInteraction(merchant, Interaction::Enemy);
	SetLayer(merchant, 0, 1);

	boothLeft = CreateInteractable("boothLeft", "Levels/Level_009/Objects/Spr_MerchantBooth.png", -3.08, -5.61, 2.32);
	Node* boothBGL = CreateSprite("boothBg", "Levels/Level_009/Objects/Spr_MerchantBoothBG.png", 0, 0, 2.32);
	boothBGL->SetParent(boothLeft);
	boothBGL->SetPosition2D(-0.08, 0.18);

	boothRight = CreateInteractable("boothRight", "Levels/Level_009/Objects/Spr_MerchantBoothFront.png", 13.8, -5.38, 2.32);
	Node* boothBGR = CreateSprite("boothBg", "Levels/Level_009/Objects/Spr_MerchantBoothBG.png", 0, 0, 2.32);
	boothBGR->SetParent(boothRight);
	boothBGR->SetPosition2D(-0.08, 0.18);

	SetLayer(boothRight, 0, 1);
	SetLayer(boothLeft, 0, 1);
	boxLeft = CreateInteractable("boxLeft", "Levels/Level_009/Objects/Spr_Crate01.png", 1.33, -7.02, 0.31);
	boxRight = CreateInteractable("boxRight", "Levels/Level_009/Objects/Spr_Crate01.png", 7.99, -6.91, 0.32);
	SetInteraction(boxLeft, Interaction::Enemy);
	SetInteraction(boxRight, Interaction::Enemy);
	chest = CreateInteractable("chest", "Levels/Level_009/Objects/Spr_Chest01.png", 4.69, -6.94, 0.51);
	SetSticky({boothLeft,boothRight});
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

	CreateShip(-16.88, -3.58);
}
void Level9::HandleInteraction(Viking* viking, Interactable* interactable) {
	Node* other = interactable->GetNode();
	String oName = other->GetName();
	//URHO3D_LOGINFO("AVAILABLE VIKINGS " + String(Cannon::instance->availableVikings[0]));
	switch (viking->type)
	{
	case VikingType::Heavy:
		break;
	case VikingType::Rogue:
		if (other == chest)
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
			viking->SetEnabled(false);
			viking->GetNode()->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->SetRotation2D(0);
			viking->GetNode()->SetPosition2D(chest->GetPosition2D()+Vector2::LEFT*2+Vector2::UP*2);
			if (merchant->IsEnabled())
			{
				//lerp merchant to box
				Lerppa::LerpThis(merchant, chest->GetPosition2D() + Vector2::LEFT*3+Vector2::UP, 0.5);
				SetAnimation(merchant,"Walk");
				MyTimer::New([=]() {
					DelayedLose();
					SetAnimation(merchant, "Idle");
				},0.5);
			}
			else
			{
				CreateSound("06_roguelockopen.ogg");
				DelayedWin();
			}
		}
		if (other == boxLeft || other == boxRight)
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Collision");
			MyTimer::New([=]() {
				viking->GetNode()->SetEnabled(false);
			},1);
		}
		if (other == boothLeft || other == boothRight)
		{
			MyTimer::New([=]() {
				viking->GetNode()->SetEnabled(false);
			}, 1);
		}
		break;
	case VikingType::Torcher:
		if (other == boxLeft || other == boxRight || other == boothLeft || other == boothRight)
		{
			StaticSprite2D* sprite = GetSprite(other);
			float size = 1;
			float yOffset = 0;
			if (sprite)
			{
				size = sprite->GetWorldBoundingBox().Size().Length();
				yOffset = sprite->GetWorldBoundingBox().Size().y_ / 3;
			}
			size *= 0.35;
			size /= other->GetWorldScale2D().y_;
			CreateEffect(other, "Flame.xml", size)->GetNode()->Translate2D(Vector2::UP*yOffset, TransformSpace::TS_WORLD);
		}
		if (other == boothRight)
		{
			Lerppa::LerpThis(merchant, Vector2(-7.58, -6.32), 1.5);
			SetAnimation(merchant,"Walk");
			MyTimer::New([=]() {
				SetAnimation(merchant, "Idle");
			},1.5);
		}
		viking->GetNode()->SetEnabled(false);
		break;
	default:
		break;
	}
	CheckLose();
}