#include "Level.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"
#include "Level9.h"
#include "Level10.h"
#include "Level11.h"
#include <Urho3D/Urho3DAll.h>
#include "MyTools.h"
#include "Cannon.h"
#include "UIManager.h"
#include "MyTimer.h"
#include "CameraController.h"
using namespace Urho3D;
Level* Level::currentLevel = 0;
int Level::levelNumber = 0;

Level::Level(Context* context) : LogicComponent(context)
{
}


Level::~Level()
{
}
void Level::RegisterLevels(Context* context_) {
	context_->RegisterFactory<Level1>();
	context_->RegisterFactory<Level2>();
	context_->RegisterFactory<Level3>();
	context_->RegisterFactory<Level4>();
	context_->RegisterFactory<Level5>();
	context_->RegisterFactory<Level6>();
	context_->RegisterFactory<Level7>();
	context_->RegisterFactory<Level8>();
	context_->RegisterFactory<Level9>();
	context_->RegisterFactory<Level10>();
	context_->RegisterFactory<Level11>();
}
Level* Level::NewLevel(int n) {
	Level* level;
	switch (n)
	{
	case 1:
		level = MyTools::currentScene->CreateComponent<Level1>();
		break;
	case 2:
		level = MyTools::currentScene->CreateComponent<Level2>();
		break;
	case 3:
		level = MyTools::currentScene->CreateComponent<Level3>();
		break;
	case 4:
		level = MyTools::currentScene->CreateComponent<Level4>();
		break;
	case 5:
		level = MyTools::currentScene->CreateComponent<Level5>();
		break;
	case 6:
		level = MyTools::currentScene->CreateComponent<Level6>();
		break;
	case 7:
		level = MyTools::currentScene->CreateComponent<Level7>();
		break;
	case 8:
		level = MyTools::currentScene->CreateComponent<Level8>();
		break;
	case 9:
		level = MyTools::currentScene->CreateComponent<Level9>();
		break;
	case 10:
		level = MyTools::currentScene->CreateComponent<Level10>();
		break;
	case 11:
		level = MyTools::currentScene->CreateComponent<Level11>();
		break;
	default:
		URHO3D_LOGERROR("LEVEL" + String(n) + " not implemented in Newlevel");
		return NewLevel(1);
		break;
	}
	currentLevel = level;
	levelNumber = n;
	return level;
}
void Level::Init() {
	//URHO3D_LOGINFO("LVLBASE Init");

	auto* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	//Helga
	helga = scene_->CreateChild("Helga");
	AnimatedSprite2D * helgaSprite = helga->CreateComponent<AnimatedSprite2D>();
	helgaSprite->SetAnimationSet(cache->GetResource<AnimationSet2D>("Urho2D/Characters/Helga_animation/Helga_anim.scml"));
	helgaSprite->SetOrderInLayer(-2);
	//Cannon
	cannon = scene_->CreateChild("Cannon");
	cannon->CreateComponent<Cannon>();
	StaticSprite2D * sprite = cannon->CreateComponent<StaticSprite2D>();
	sprite->SetSprite(cache->GetResource<Sprite2D>("Urho2D/Common/Spr_Cannon01.png"));

	cannon->SetScale2D(3.5, 3.5);

	sprite->SetOrderInLayer(-1);
	if (levelNumber == 1 || levelNumber == 2)
	{
		//helga->CreateComponent<AnimatedSprite2D>()->SetAnimationSet(cache->GetResource<AnimationSet2D>("Urho2D/Characters/Helga_animation/Helga_anim.scml"));
		//helga->GetComponent<AnimatedSprite2D>()->SetAnimation("");
		helga->SetPosition2D(-16.61, -4.61);
		helga->SetScale2D(0.5, 0.5);
		sprite->SetAlpha(0);
	}
	cannon->SetPosition2D(-16.61, -5.73);
	won = false;
	//load ui
	Cannon::instance->SetupButtons();
	MyTools::currentScene->SetUpdateEnabled(true); MyTools::currentScene->SetTimeScale(2);
}
void Level::Unload() {
	MyTools::currentScene->SetUpdateEnabled(true);
	Vector<SharedPtr<Node>> children = node_->GetChildren();
	for (size_t i = 0; i < children.Size(); i++)
	{
		Node* child = children[i].Get();
		if (child->GetComponent<Camera>())
		{
			continue;
		}
		if (child->GetComponent<CameraController>())
		{
			continue;
		}
		node_->RemoveChild(child);
	}
	UIManager::instance->HideAll();
	currentLevel->background = NULL;
}
void Level::Win() {
	URHO3D_LOGINFO("WIN");
	UIManager::instance->HideAll();
	UIManager::instance->GetOne(5);
}
void Level::DelayedWin() {
	currentLevel->won = true;
	//currentLevel->Win();

	MyTimer::New([=]() {currentLevel->Win(); }, currentLevel->winDelay);
	if (Level::levelNumber == 1) 
	{
		UIManager::instance->comicHandler(false,currentLevel->winDelay);
	}
}
void Level::DelayedLose() {
	//currentLevel->Win();
	MyTimer::New([=]() {if (currentLevel->won == false) { currentLevel->Lose(); }}, currentLevel->loseDelay);
}

void Level::CheckLose() {
	if (won)
	{
		return;
	}
	for (size_t i = 0; i < Cannon::instance->availableVikingsSize; i++)
	{
		if (Cannon::instance->availableVikings[i] > 0)
		{
			return;
		}
	}
	DelayedLose();

}
void Level::Lose() {
	MyTools::currentScene->SetUpdateEnabled(false);
	URHO3D_LOGINFO("LOST base");
	UIManager::instance->HideAll();
	UIManager::instance->GetOne(6);
}
Node* Level::CreateSprite(String name, String sprite, float x, float y, float scale, float rotation) {
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	Node* newNode = scene_->CreateChild(name);
	Sprite2D* newSprite = cache->GetResource<Sprite2D>("Urho2D/" + sprite);
	newNode->CreateComponent<StaticSprite2D>()->SetSprite(newSprite);
	newNode->SetScale2D(Vector2::ONE * scale);
	newNode->SetRotation2D(rotation);
	newNode->SetPosition2D(x, y);
	return newNode;
}
Node* Level::CreateObject(String name, String sprite, float x, float y, float scale, float rotation) {
	Node* newNode = CreateSprite(name, sprite, x, y, scale, 0);
	RigidBody2D* newRigid = newNode->CreateComponent<RigidBody2D>();
	newRigid->SetBodyType(BodyType2D::BT_STATIC);

	StaticSprite2D* newSprite = newNode->GetComponent<StaticSprite2D>();
	BoundingBox bounds = newSprite->GetWorldBoundingBox();
	Vector3 position = Vector3(x, y, 0);
	if (newSprite->GetSprite() == NULL)
	{
		bounds.min_ = position - Vector3::ONE;
		bounds.max_ = position + Vector3::ONE;
	}
	//bounds.Size = newNode->WorldToLocal(bounds.Size);

	CollisionBox2D* collider = newNode->CreateComponent<CollisionBox2D>();
	collider->SetSize(MyTools::ToVector2(bounds.Size() / scale));
	collider->SetCenter(MyTools::ToVector2(newNode->WorldToLocal(bounds.Center())));
	newNode->SetRotation2D(rotation);//rotate after making collider to avoid wrong collider size from sprite bounds
	return newNode;
}

Node* Level::CreateInteractable(String name, String sprite, float x, float y, float scale, float rotation) {
	Node* newNode = CreateObject(name, sprite, x, y, scale, rotation);
	newNode->CreateComponent<Interactable>();
	return newNode;
}
Node* Level::CreateAnimatedSprite(String name, String sprite, String anim, float x, float y, float scale, float rotation) {
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Scene* scene_ = MyTools::currentScene;
	Node* newNode = scene_->CreateChild(name);

	newNode->CreateComponent<AnimatedSprite2D>()->SetAnimationSet(cache->GetResource<AnimationSet2D>("Urho2D/" + sprite));
	newNode->SetScale2D(Vector2::ONE * scale);
	newNode->SetRotation2D(rotation);
	newNode->SetPosition2D(x, y);
	RigidBody2D* newRigid = newNode->CreateComponent<RigidBody2D>();
	newRigid->SetBodyType(BodyType2D::BT_STATIC);

	AnimatedSprite2D* newAnim = newNode->GetComponent<AnimatedSprite2D>();
	newAnim->SetAnimation(anim);
	return newNode;
}
Node* Level::CreateAnimatedObject(String name, String sprite, String anim, float x, float y, float scale, float rotation) {
	Node* newNode = CreateAnimatedSprite(name,sprite,anim,x,y,scale,0);//create with 0 rotation before setting colliders to fix autosize
	auto* newAnim = newNode->GetComponent<AnimatedSprite2D>();
	BoundingBox bounds = newAnim->GetWorldBoundingBox();
	Vector3 position = Vector3(x, y, 0);
	if (newAnim->GetSprite() == NULL)
	{
		bounds.min_ = position - Vector3::ONE;
		bounds.max_ = position + Vector3::ONE;
	}
	bounds.Size() = newNode->WorldToLocal(bounds.Size());

	CollisionBox2D* collider = newNode->CreateComponent<CollisionBox2D>();
	collider->SetSize(MyTools::ToVector2(bounds.Size() / scale));
	collider->SetCenter(MyTools::ToVector2(newNode->WorldToLocal(bounds.Center())));
	newNode->SetRotation2D(rotation);//rotate after making collider to avoid wrong collider size from sprite bounds
	return newNode;
}
Node* Level::CreateAnimatedInteractable(String name, String sprite, String anim, float x, float y, float scale, float rotation)
{
	Node* newNode = CreateAnimatedObject(name,sprite,anim,x,y,scale,rotation);
	newNode->CreateComponent<Interactable>();
	return newNode;
}
ParticleEmitter* Level::CreateEffect(Node* node, String name, float sizeMul, float deleteAfter) {
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	node = node->CreateChild(name);

	auto* emitter = node->CreateComponent<ParticleEmitter>();
	ParticleEffect* effect = cache->GetResource<ParticleEffect>("Particle/" + name);
	//effectscale doesnt scale effect positions
	node->SetScale2D(sizeMul, sizeMul);
	effect->SetMaxTimeToLive(3);
	//effect->SetSizeMul(sizeMul);
	emitter->SetEffect(effect);
	if (deleteAfter > 0)
	{
		MyTimer::New([=]() {node->SetEnabled(false); }, deleteAfter);
	}
	return emitter;
}
ParticleEmitter* Level::CreateEffect(Vector3 position, String name, float sizeMul, float deleteAfter) {
	Node* node = MyTools::currentScene->CreateChild(name);
	node->SetPosition(position);
	return CreateEffect(node, name, sizeMul, deleteAfter);//currently shit because creates a child of child, fix later
}
SoundSource* Level::CreateSound(Node* node, String name, float volumeMul) {
	auto* source = node->CreateComponent<SoundSource>();
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	auto* sound = cache->GetResource<Sound>("Sounds/" + name);
	source->SetGain(volumeMul);
	source->Play(sound);
	return source;
}
SoundSource* Level::CreateSound(String name, float volumeMul) {
	Node* node = MyTools::currentScene->CreateChild(name);
	return CreateSound(node, name, volumeMul);
}
void Level::SetLayer(Node* node, int layer, int order, bool recursive) {
	if (recursive)//set every child as well, mostly for setting the tilemap as a whole
	{
		PODVector<Node*> children;
		node->GetChildren(children, true);
		for (size_t i = 0; i < children.Size(); i++)
		{
			SetLayer(children[i], layer, order, false);
		}
	}
	StaticSprite2D* sprite = node->GetComponent<StaticSprite2D>();
	if (!sprite)
	{
		sprite = node->GetComponent<AnimatedSprite2D>();
	}
	if (sprite)
	{
		sprite->SetLayer(layer);
		sprite->SetOrderInLayer(order);
	}
}
void Level::SetInteraction(Node* node, int interaction) {
	Interactable* interactable = node->GetComponent<Interactable>();
	if (interactable)
	{
		interactable->defaultInteraction = interaction;
	}
}
Node* Level::CreateShip(float x, float y, float scale) {
	Node* ship = CreateSprite("ship", "Common/Spr_ShipSailsDown.png", x, y, scale);
	Node* panel = CreateSprite("panel", "Common/Spr_ShipPanel.png", x + 2.51, y - 2.27, 1);
	SetLayer(panel, 0, -2);
	cannon->SetPosition2D(x + 2.66, y - 2.03);
	return ship;
}
StaticSprite2D* Level::GetSprite(Node* node) {
	StaticSprite2D* sprite = node->GetComponent<StaticSprite2D>();
	if (!sprite)
	{
		sprite = node->GetComponent<AnimatedSprite2D>();
	}
	if (!sprite)
	{
		sprite = node->GetComponent<StretchableSprite2D>();
	}
	if (!sprite)
	{
		URHO3D_LOGERROR("Couldn't find sprite on " + node->GetName());
	}
	return sprite;
}
void Level::SetSticky(std::initializer_list<Node*> nodes) {
	for (auto* n : nodes) {
		auto* i = n->GetComponent<Interactable>();
		if (i)
		{
			i->heavySticks = true;
		}
	}
}
void Level::SetAnimation(Node* node, String animation) {
	auto* anim = node->GetComponent<AnimatedSprite2D>();
	if (anim)
	{
		anim->SetAnimation(animation);
	}
}
