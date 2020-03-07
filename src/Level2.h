#pragma once
#include "Level.h"
class Level2 :
	public Level
{
	URHO3D_OBJECT(Level2, Level);
public:
	explicit Level2(Context* context);
	~Level2();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 1,2,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	void Update(float deltaTime) override;
	Node* lock = 0;
	Node* gate = 0;
	Node* rope = 0;
	String ambientSound = "11_wavesambience.ogg";

};

