#pragma once
#include "Level.h"
class Level9 :
	public Level
{
	URHO3D_OBJECT(Level9, Level);
public:
	explicit Level9(Context* context);
	~Level9();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 3,2,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	Node* boothLeft = 0;
	Node* boothRight = 0;
	Node* boxLeft = 0;
	Node* boxRight = 0;
	Node* chest = 0;
	Node* merchant = 0;
	String ambientSound = "10_frenchmarketambience.ogg";

};

