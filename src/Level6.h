#pragma once
#include "Level.h"
class Level6 :
	public Level
{
	URHO3D_OBJECT(Level6, Level);
public:
	explicit Level6(Context* context);
	~Level6();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 2,1,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	String ambientSound = "12_forestambience.ogg";
};

