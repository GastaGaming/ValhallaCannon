#pragma once
#include "Level.h"
class Level1 :
	public Level
{
	URHO3D_OBJECT(Level1, Level);
public:
	explicit Level1(Context* context);
	~Level1();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 1,0,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	String ambientSound = "12_forestambience";
};

