#include "Level6.h"
#include "Cannon.h"
#include "UIManager.h"

Level6::Level6(Context* context) :Level(context)
{
}

Level6::~Level6()
{
}

void Level6::Init() {
	//Level::Init(context);
	URHO3D_LOGINFO("LVL6 Init");
	//creating the level, setting dependencies etc..

	//pasta from main
	Level::Init();
	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	
	// bg
	background = scene_->CreateChild("background");
	StaticSprite2D* bgSprite = background->CreateComponent<StaticSprite2D>();
	bgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/Levels/Level_006/Level_006_BgLayer.png"));
	background->SetScale2D(2.14, 2.14);
	bgSprite->SetLayer(-10);
	bgSprite->SetOrderInLayer(214);
	background->SetEnabled(UIManager::bgOn);

	//fg
	foreground = scene_->CreateChild("foreground");
	StaticSprite2D* fgSprite = foreground->CreateComponent<StaticSprite2D>();
	fgSprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/Levels/Level_006/Objects/Spr_ForeGround02.png"));
	foreground->SetPosition(Vector3(0, 7.44, 0));
	foreground->SetScale2D(1.88, 1.88);
	fgSprite->SetLayer(5);
	fgSprite->SetOrderInLayer(214);
	foreground->SetEnabled(UIManager::bgOn);

	Node* ship = CreateSprite("ship", "Spr_ShipSailsDown.png", -19.19, -3.47, 0.85);

	Node* trees = CreateSprite("trees", "Levels/Level_006/Objects/Spr_Trees.png", -0.2, -0.63, 1.88);
	Node* treeRight = CreateSprite("treeRight", "Levels/Level_006/Objects/Spr_TreeRightSide.png", -1.8, 0.03, 1.88);
	SetLayer(trees, -10, 215);
	SetLayer(treeRight,0,0);
	/*Node* treeHoleSmall = CreateSprite("treeHoleSmall", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -3.61, 5.42, 2.28);
	Node* treesHoleBig = CreateSprite("treesHoleBig", "Levels/Level_006/Objects/Spr_TreeHoleBig.png", -3.61, -2.52, 2.13);*/
	
	Node* treeColBot = CreateInteractable("TreeBottom", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -4, -8.7, 1);
	treeColBot->GetComponent<CollisionBox2D>()->SetSize(1.8,5);
	SetLayer(treeColBot, -10, 213);
	Node* treeColMid = CreateInteractable("TreeMiddle", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -4, 2, 1);
	treeColMid->GetComponent<CollisionBox2D>()->SetSize(1.8, 2.5);
	SetLayer(treeColMid, -10, 213);
	Node* treeColTop = CreateInteractable("TreeTop", "Levels/Level_006/Objects/Spr_TreeHoleSmall.png", -4, 9.52, 1);
	treeColTop->GetComponent<CollisionBox2D>()->SetSize(1.8, 4.5);
	SetLayer(treeColTop, -10, 213);

	Node* treeHouse = CreateInteractable("treeHouse", "Levels/Level_006/Objects/Spr_Treehouse.png", 5.94, -7.99, 1.9);
	auto* treeBox = treeHouse->GetComponent<CollisionBox2D>();
	Vector2 treeScale = treeBox->GetSize();
	treeBox->SetSize(treeScale.x_, treeScale.y_/2.2);


	Node* robin = CreateAnimatedInteractable("Robin", "Characters/Robin/EnemyAnimation.scml","Idle", 8.7, -5);
	SetLayer(robin, 0, 1);
	Node* john = CreateAnimatedInteractable("John", "Characters/John/EnemyAnimation.scml","Idle", 2.81, -5.13);
	SetLayer(john, 0, 1);
	Node* burb = CreateAnimatedInteractable("burb", "Owl/Anim_Owl01.scml","Idle", -3.88, -5.4, 1.44, 1.44);
	SetLayer(burb, 0, 1);
	CreateSound(ambientSound);
}
void Level6::HandleInteraction(Viking* viking, Interactable* interactable) {
	
	Node* otherNode = interactable->GetNode();
	if (viking->type == VikingType::Rogue)
	{
		if (interactable->GetNode()->GetName() == "burb")
		{
			viking->GetNode()->SetEnabled(false);
			CreateSound(otherNode, "BigExplosion.wav");
			interactable->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
			Lerppa::LerpThis(interactable->GetNode(), Vector2(-3.88, 3.9), 1);
			MyTimer::New([=]() {interactable->GetComponent<AnimatedSprite2D>()->SetAnimation("Idle"); }, 1);
		}
	}
	if (viking->type == VikingType::Heavy)
	{
		if (interactable->GetNode()->GetName() == "John")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			//CreateSound(otherNode, "BigExplosion.wav");
			CreateSound("08_heavyhitobject.ogg");

			GetSprite(interactable->GetNode())->SetEnabled(false);

		}
		else if (interactable->GetNode()->GetName() == "Robin")
		{
			interactable->GetNode()->GetComponent<RigidBody2D>()->SetEnabled(false);
			viking->GetNode()->SetEnabled(false);
			//CreateSound(otherNode, "BigExplosion.wav");
			CreateSound("08_heavyhitobject.ogg");

			GetSprite(interactable->GetNode())->SetEnabled(false);
			DelayedWin();
		}
		else if (interactable->GetNode()->GetName() == "burb")
		{
			viking->GetNode()->SetEnabled(false);
		}
		else
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->CreateComponent<Interactable>();
			interactable->GetNode()->RemoveComponent<Interactable>();
			//viking->GetNode()->RemoveComponent<Viking>();

		}
	}
	if (viking->type == VikingType::Torcher)
	{
		if (interactable->GetNode()->GetName() == "treeHouse" || interactable->GetNode()->GetName() == "TreeBottom"|| interactable->GetNode()->GetName() == "TreeMiddle" || interactable->GetNode()->GetName() == "TreeTop"|| interactable->GetNode()->GetName() == "burb")
		{
			viking->GetNode()->SetEnabled(false);
			//fire pls
			CreateEffect(otherNode, "Flame.xml", 9);
			//smoke
			CreateEffect(otherNode, "SmokeStack.xml", 3)->GetNode()->Translate(Vector3::FORWARD);

			//sound
			CreateSound("07_torcherfire.ogg");
			CreateSound("09_torcherfireambience.ogg");
			DelayedLose();
		}
	}
	CheckLose();
}