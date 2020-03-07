#include "Lerppa.h"
Lerppa::Lerppa(Context* context) :LogicComponent(context)
{

}
Lerppa::~Lerppa()
{

}
void Lerppa::Update(float deltaTime)
{
	float t = (MyTime->GetElapsedTime() - startTime) / duration;
	t = Clamp(t, 0.0f, 1.0f);
	thing->SetPosition2D(Lerp(startPos.x_, destination.x_, t), Lerp(startPos.y_, destination.y_, t));
	if (rotate)
	{
		thing->SetRotation2D(Lerp(startRot, destinationRot, t));
	}
	if (t >= 1)
	{
		SetEnabled(false);
	}
}
void Lerppa::LerpThis(Node* object, Vector2 there, float when, bool rotate, float rotation)
{
	Lerppa* newLerppa = object->CreateComponent<Lerppa>();
	newLerppa->thing = object;
	newLerppa->destination = there;
	newLerppa->duration = when;
	newLerppa->startTime = MyTime->GetElapsedTime();
	newLerppa->startPos = object->GetPosition2D();
	newLerppa->rotate = rotate;
	newLerppa->startRot = object->GetRotation2D();
	newLerppa->destinationRot = rotation;
}