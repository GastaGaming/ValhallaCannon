#pragma once
#include "../Sample.h"
#include <string>
#include <sstream>
#include "UIManager.h"
#include <Urho3D/Urho3DAll.h>
#include "Cannon.h"
#include "MyTools.h"
#include "Viking.h"
#include "Interactable.h"
#include "LevelParser.h"
#include "Level.h"
#include "CameraController.h"// #include <Urho3D/Urho3DAll.h>
#include "Lerppa.h"
#include "MyAnimator.h"
#include "Printable.h"

class CrashCourse : public Sample
{
	URHO3D_OBJECT(CrashCourse, Sample);
public:
	explicit CrashCourse(Context* context);
	SharedPtr<Text> text_;
	SharedPtr<Scene> scene_;
	SharedPtr<Node> boxNode_;
	//SharedPtr<Node> cameraNode_;
	WeakPtr<UIManager> ui_;
	SharedPtr<CameraController> cam_;
	SharedPtr<Node> camera_;
	int totalFrames = 0;
	
	void Setup() override;
	void Start() override;

protected:
	/// Return XML patch instructions for screen joystick layout for a specific sample app, if any.
	String GetScreenJoystickPatchString() const override {
		return
			"<patch>"
			"    <add sel=\"/element/element[./attribute[@name='Name' and @value='Hat0']]\">"
			"        <attribute name=\"Is Visible\" value=\"false\" />"
			"    </add>"
			"</patch>";
	}
private:
	bool createdProto = false;
	/// Construct a new Text instance, containing the 'Hello World' String, and add it to the UI root element.
	void CreateText();
	/// Subscribe to application-wide logic update events.
	void SubscribeToEvents();
	/// Handle the logic update event.
	void HandleUpdate(StringHash eventType, VariantMap& eventData);

	void DoUI();

	void HandleResize(StringHash eventType, VariantMap& eventData);
	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
	void HandleClosePressed(StringHash eventType, VariantMap& eventData);
	void MakeProtoScene();
};
