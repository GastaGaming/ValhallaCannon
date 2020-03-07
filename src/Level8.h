#pragma once
#include "Level.h"
class Level8 :
	public Level
{
	URHO3D_OBJECT(Level8, Level);
public:
	explicit Level8(Context* context);
	~Level8();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 1,1,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	Node* launder = 0;
	Node* launderer = 0;
	Node* lock = 0;
	Node* bridge = 0;
	Node* chain = 0;
	Node* croc1 = 0;
	Node* croc2 = 0;
	String ambientSound = "11_wavesambience.ogg";
	void CrocEat(Node* target, Vector2 offset = Vector2::ZERO);
};

