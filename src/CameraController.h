#pragma once
#pragma once
#include <Urho3D/Urho3DAll.h>
#include <string>
using namespace Urho3D;

class CameraController : public LogicComponent
{
	URHO3D_OBJECT(CameraController, LogicComponent)
public:
	/// Construct.
	explicit CameraController(Context* context);
	~CameraController();
	//static CameraController* instance;
	float camZoom;
	TouchState touch;
	SharedPtr<Node> cameraNode_;
	/// Register object factory and attributes.
	static void RegisterObject(Context* context);
	/// Perform post-load after deserialization. Acquire the components from the scene nodes.
	//void ApplyAttributes() override;
	/// Handle physics world update. Called by LogicComponent base class.
	//void FixedUpdate(float timeStep) override;
	virtual void Init(Scene* scene);
	void Update(float timeStep) override;
	void InitTouchInput();
	/// Movement controls.
	Controls controls_;
	bool touchEnabled_ = false;
	//CamerShake
	float shakeValues[21] = {0.2,0.4,0.5,0.6,0.8,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4};
	void CameraShake(float duration,int ammount, float zoom);
	float shakeDuration = 0;
	float nextRnd;
	bool shaking = false;
	int shakeAmmount;
private:
	bool zoom = false;
	bool windows = false;
	Vector2 targetPosition;
	Vector3 cameraStartPosition;
	Vector3 cameraTargetPosition;
	void MultiTouch(int One, int Second, float timeStep);
	void SingleTouch(int One, float timeStep);
	virtual String GetScreenJoystickPatchString() const { return String::EMPTY; }
	/// Screen joystick index for navigational controls (mobile platforms only).
	unsigned screenJoystickIndex_;
	/// Screen joystick index for settings (mobile platforms only).
	unsigned screenJoystickSettingsIndex_;
	void HandleTouchBegin(StringHash eventType, VariantMap& eventData);
};

