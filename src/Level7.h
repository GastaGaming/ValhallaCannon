#pragma once
#include "Level.h"
class Level7 :
	public Level
{
	URHO3D_OBJECT(Level7, Level);
public:
	explicit Level7(Context* context);
	~Level7();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 9,0,0 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	Node* target;
	String ambientSound = "12_forestambience";
};

