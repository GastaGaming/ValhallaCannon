#pragma once
#include <Urho3D/Urho3DAll.h>
#include "Viking.h"
#include "Interactable.h"
#include "Lerppa.h"
#include <initializer_list>
//base class for levels
//each level can inherit handleinteraction and implement its own reactions inside its own class
class Level :
	public LogicComponent
{
	URHO3D_OBJECT(Level, LogicComponent);
public:
	explicit Level(Context* context);
	~Level();
	static Level* currentLevel;
	static int levelNumber;
	static void RegisterLevels(Context* context);
	virtual void Init();//create the level here, base Level::Init() creates default cannon & ui
	virtual void HandleInteraction(Viking* viking, Interactable* interactable) {}//called by interactables on collision with a viking, check for interactions, lose & win conditions
	virtual void Unload();//unload this level, by default clears all UI and children except camera
	static Level* NewLevel(int n);
	int usableVikings[3] = { 1,2,3 };//for some reason cannon calls this when getting currentlevel usables from level1, could just setusables from init
	void Win();//activated by HandleInteraction between last obstacle and correct viking
	virtual void CheckLose();
	virtual void Lose();//checked for everytime after handleInteraction?
	virtual int* GetUsableVikings() { return usableVikings; }
	Node* CreateSprite(String name, String sprite, float x, float y,float scale = 1, float rotation = 0);
	Node* CreateObject(String name, String sprite, float x, float y, float scale = 1, float rotation = 0);
	Node* CreateAnimatedSprite(String name, String sprite, String anim, float x, float y, float scale = 1, float rotation = 0);
	Node* CreateAnimatedObject(String name, String sprite, String anim, float x, float y, float scale = 1, float rotation = 0);
	Node* CreateAnimatedInteractable(String name, String sprite,String anim, float x, float y, float scale = 1, float rotation = 0);
	Node* CreateInteractable(String name, String sprite, float x, float y, float scale = 1, float rotation = 0);
	Node* CreateShip(float x, float y,float scale = 0.85);
	ParticleEmitter* CreateEffect(Node* node, String name, float sizeMul = 1, float deleteAfter = -1);
	ParticleEmitter* CreateEffect(Vector3 position, String name, float sizeMul = 1, float deleteAfter = -1);
	SoundSource* CreateSound(Node* node, String name, float volumeMul = 1);
	SoundSource* CreateSound(String name, float volumeMul = 1);
	void SetLayer(Node* node, int layer, int order, bool recursive = false);
	void SetInteraction(Node* node, int interaction);
	void SetSticky(std::initializer_list<Node*> nodes);
	void SetAnimation(Node* node, String animation);
	StaticSprite2D* GetSprite(Node* node);
	Node* background =0;
	Node* foreground = 0;
	Node* cannon = 0;
	Node* helga = 0;
	static void DelayedWin();
	static void DelayedLose();
	bool won = false;
	String ambientSound = "";
	float winDelay = 1.5;
	float loseDelay = 1.5;
};