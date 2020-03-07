#pragma once
#include "Level.h"
class Level4 :
	public Level
{
	URHO3D_OBJECT(Level4, Level);
public:
	explicit Level4(Context* context);
	~Level4();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 1,3,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	String ambientSound = "11_wavesambience.ogg";
	Node* seagull;
};

