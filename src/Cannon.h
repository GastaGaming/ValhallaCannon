#pragma once
#include <Urho3D/Urho3DAll.h>
#include "MyTools.h"
#include <vector>
#include <stdarg.h>
#include "Viking.h"
using namespace Urho3D;


class Cannon :
	public LogicComponent
{
	URHO3D_OBJECT(Cannon, LogicComponent);
public:
	static Cannon* instance;
	explicit Cannon(Context* context);
	~Cannon();
	void Update(float deltaTime) override;
	void Start() override;
	void DelayedStart() override;
	void HandleTouchBegin(StringHash eventType, VariantMap& eventData);
	void HandleTouchEnd(StringHash eventType, VariantMap& eventData);
	void UpdateAimRay();
	Ray aimRay_;
	void ShootViking();
	void SelectHeavy(StringHash eventType, VariantMap& eventData);
	void SelectRogue(StringHash eventType, VariantMap& eventData);
	void SelectTorcher(StringHash eventType, VariantMap& eventData);
	void SelectShoot(StringHash eventType, VariantMap& eventData);
	void SetupButtons();
	void CheckAvailableVikings();
	int availableVikingsSize = 3;//fuck you vectors
	int availableVikings[3] = {1,1,1};
	void SetAvailableVikings(int heavys, int rogues, int torchers);
	void SetVikingType(int type);
	void SetButton(int type, bool enabled);
	static String GetVikingButtonName(int type) { return Viking::TypeToName(type) + " button"; }
	
	void SetSwitchTime();
	void UIFocus(StringHash eventType, VariantMap& eventData) { SetSwitchTime(); }
private:
	float recoilDistance = 0.2;
	float recoilTime = 0.2;
	float lastShot = -99;
	Vector2 recoilDirection;
	Vector2 originalPosition;
	float targetRotation = 0;
	float rotationSpeed = 5;
	Vector3 aimDirection = Vector3::RIGHT;
	float minPower = 13.75;
	float maxPower = 20.0;
	float power = minPower;
	int vikingType = -1;
	float switchTime = -99.0;
	bool uiFocus = false;//checks for cannon buttons only
	float minAimDistance = 5;
	float maxAimDistance = 15;
	//set to true on button press and false on touchend
	SoundSource* moveSoundSource = 0;
	void PlayMoveSound(){ moveSoundSource->Play(MyCache->GetResource<Sound>("Sounds/01_cannonmovesound.ogg")); };
	Node* vikingVisual = 0;
	Node* helga = 0;
	float helgaAnimTime = 0;
};