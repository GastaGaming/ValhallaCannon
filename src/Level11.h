#pragma once
#include "Level.h"
class Level11 :
	public Level
{
	URHO3D_OBJECT(Level11, Level);
public:
	explicit Level11(Context* context);
	~Level11();
	void Init() override;//scene or context as param?
	//or maybe this should be a component to scene?
	void HandleInteraction(Viking* viking, Interactable* interactable) override;
	int usableVikings[3] = { 1,1,1 };
	//void Lose() override { URHO3D_LOGINFO("LOST lvl1"); };
	int* GetUsableVikings()override { return usableVikings; }
	Node* chain = 0;
	Node* lock = 0;
	Node* stove = 0;
	Node* rack1 = 0;
	Node* rack2 = 0;
	Node* pan1 = 0;
	Node* pan2 = 0;
	String ambientSound = "11_wavesambience.ogg";
	Node* cook1 = 0;
	Node* cook2 = 0;
	Node* cook3 = 0;
	Node* roofRack = 0;
	Sprite* tick1 = 0;
	Sprite* tick2 = 0;
	Sprite* tick3 = 0;
	bool burned = false;
	bool watered = false;
	bool smashed = false;
};

