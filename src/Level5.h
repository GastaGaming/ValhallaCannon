#pragma once
#include "Level.h"
class Level5 :
	public Level
{
	URHO3D_OBJECT(Level5, Level);
public:
	explicit Level5(Context* context);
	~Level5();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 2,1,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	Node* lock = 0;
	String ambientSound = "10_frenchmarketambience.ogg";
};

