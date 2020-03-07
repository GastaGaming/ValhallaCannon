#pragma once
#include <Urho3D/Urho3DAll.h>
#include "MyTools.h"
#include <vector>
#include <stdarg.h>
#include "Viking.h"

using namespace Urho3D;

class Lerppa: public LogicComponent
{
	URHO3D_OBJECT(Lerppa,LogicComponent)
public:
	explicit Lerppa(Context* context);
	~Lerppa();
	static void LerpThis(Node* object, Vector2 there, float when, bool rotate = false, float rotation = 0);
	void Update(float deltaTime) override;
	Node* thing = 0;
	Vector2 destination;
	float duration = 0;
	float startTime;
	Vector2 startPos;
	float startRot;
	bool rotate = false;
	float destinationRot;
private:
	
};

