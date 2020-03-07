#pragma once
#include "Level.h"
class Level10 :
	public Level
{
	URHO3D_OBJECT(Level10, Level);
public:
	explicit Level10(Context* context);
	~Level10();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 2,3,2 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	Node* owl = 0;
	Node* owl2 = 0;
	Node* booth = 0;
	Node* windowCol = 0;
	Node* princess = 0;
	Node* lock = 0;
	String ambientSound = "11_wavesambience.ogg";

};

