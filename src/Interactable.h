#pragma once
#include "Urho3D/Urho3DAll.h"
#include "Viking.h"
#include "MyTools.h"

enum Interaction { None, Lock, Birb, Tree,Enemy };
class Interactable :
	public LogicComponent
{
	URHO3D_OBJECT(Interactable, LogicComponent);
public:
	explicit Interactable(Context* context);
	~Interactable();
	void HandleCollisionBegin(StringHash eventType, VariantMap& eventData);
	void OnNodeSet(Node* node) override;
	void Update(float deltaTime) override;
	int weaknessType = VikingType::Heavy;
	int defaultInteraction = Interaction::None;
	bool winCondition = false;
	bool heavySticks = false;
};