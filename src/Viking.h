#pragma once
#include <Urho3D/Urho3DAll.h>
#include "MyTools.h"
using namespace Urho3D;

	enum VikingType { Heavy = 0, Rogue = 1, Torcher = 2 };
class Viking :
	public LogicComponent
{
	URHO3D_OBJECT(Viking, LogicComponent);
public:
	int type = VikingType::Heavy;

	explicit Viking(Context* context);
	~Viking();
	void SetType(int type);
	//startspeed multiplier?
	static int NameToType(String name);
	static String TypeToName(int type);
	void Update(float deltaTime) override;
protected:
	void OnNodeSet(Node* node) override;
private:
	void HandleNodeCollision(StringHash eventType, VariantMap& eventData);
	float oldVelocity;
	float velocityDelay = 0.1;
};

