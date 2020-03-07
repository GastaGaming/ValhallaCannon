#include "CameraController.h"
#include "MyTools.h"
#include <Urho3D/Urho3DAll.h>
#include "math.h"
//CameraController* CameraController::instance = 0;
CameraController::CameraController(Context* context) : LogicComponent(context)
{
	// Only the physics update event is needed: unsubscribe from the rest for optimization
	//SetUpdateEventMask(USE_FIXEDUPDATE);
}
CameraController::~CameraController() {

}
void CameraController::RegisterObject(Context* context)
{
	context->RegisterFactory<CameraController>();
}
void CameraController::Init(Scene* scene)
{
	//instance = this;
	// This function is called only from the main program when initially creating the vehicle, not on scene load
	//First we check witch platform we are
	if (GetPlatform() == "Android" || GetPlatform() == "iOS") 
	{
		InitTouchInput();
	}
	else if (GetSubsystem<Input>()->GetNumJoysticks() == 0)
	{
		// On desktop ww, do not detect touch when we already got a joystick
		windows = true;
	    //SubscribeToEvent(E_TOUCHBEGIN, URHO3D_HANDLER(CameraController, HandleTouchBegin));
	}
	Scene* scene_ = scene;
	cameraNode_ = scene_->CreateChild("Camera");
	//cameraNode_ = crea
	//cameraNode_ = scene_->GetChild("Camera");
	//cameraNode_->SetPosition(Vector3(0, 0, -200));
	Camera* camera = cameraNode_->CreateComponent<Camera>();
	//cameraNode_->CreateComponent<CameraController>();
	camera->SetOrthoSize(20);
	//camera->SetFov(90);
	camera->SetOrthographic(true);
	//camera->SetViewMask(5);
	// Now we setup the viewport. Of course, you can have more than one!
	Renderer* renderer = GetSubsystem<Renderer>();
	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
	renderer->SetViewport(0, viewport);
	cameraStartPosition = cameraNode_->GetPosition();

	//DebugSetup
}
void CameraController::InitTouchInput()
{
	touchEnabled_ = true;

	ResourceCache* cache = GetSubsystem<ResourceCache>();
	Input* input = GetSubsystem<Input>();
	XMLFile* layout = cache->GetResource<XMLFile>("UI/ScreenJoystick_Samples.xml");
	const String& patchString = GetScreenJoystickPatchString();
	if (!patchString.Empty())
	{
		// Patch the screen joystick layout further on demand
		SharedPtr<XMLFile> patchFile(new XMLFile(context_));
		if (patchFile->FromString(patchString))
			layout->Patch(patchFile);
	}
	screenJoystickIndex_ = (unsigned)input->AddScreenJoystick(layout, cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
	input->SetScreenJoystickVisible(screenJoystickSettingsIndex_, false);
}
void CameraController::HandleTouchBegin(StringHash /*eventType*/, VariantMap& eventData)
{
	// On some platforms like Windows the presence of touch input can only be detected dynamically
	InitTouchInput();
	UnsubscribeFromEvent("TouchBegin");
}

void CameraController::Update(float timeStep) {
	if (MyInput->GetNumTouches() == 1)
	{
		if (windows == false)
		{
			URHO3D_LOGINFO("kOSKETETTIIN KERRAN ANDROID");
		}

	}
	if (MyInput->GetNumTouches() == 2)
	{
		if (windows == true) {
			//URHO3D_LOGINFO("kOSKETETTIIN KERRAN windows");
			//SingleTouch(0, timeStep);
		}
		else {
			URHO3D_LOGINFO("kOSKETETTIIN kahdesti ANDROID");
			MultiTouch(0, 1, timeStep); //Multitouch(touchID 1, touchID 2, timestep)
		}
	}
	if (MyInput->GetNumTouches() == 3)
	{
		//URHO3D_LOGINFO("kOSKETETTIIN kahdesti windows");
		MultiTouch(1, 2, timeStep); //Multitouch(touchID 1, touchID 2, timestep)
	}
	//Palautetaan camera alkuperäseen postioon
	float orthoSize = cameraNode_->GetComponent<Camera>()->GetOrthoSize();
	float zoomSize = cameraNode_->GetComponent<Camera>()->GetZoom();
	//if (orthoSize < 20)//orthoSize = orthoSize + (timeStep) * 5;//cameraNode_->GetComponent<Camera>()->SetOrthoSize(orthoSize);
	if (zoomSize > 1 && shaking == false) {
		zoomSize = zoomSize - (timeStep) * 0.5;
		URHO3D_LOGINFO(String(zoomSize));
		cameraNode_->GetComponent<Camera>()->SetZoom(zoomSize);
	}
	else
	{
		//orthoSize = 20;
		//cameraNode_->GetComponent<Camera>()->SetOrthoSize(orthoSize);
		zoomSize = 1;
		cameraNode_->GetComponent<Camera>()->SetZoom(zoomSize);
	}
	if (cameraNode_->GetPosition() != cameraStartPosition && shaking == false)
	{
		Vector3 currenPos = cameraNode_->GetWorldPosition();
		cameraNode_->SetWorldPosition(Lerp(currenPos, cameraStartPosition, 0.1f));
	}
	//CameraShake
	if (shakeDuration > 0 && shaking == true ) 
	{
		if (nextRnd > shakeDuration)
		{
			nextRnd = nextRnd - 0.2f;
		    int RandIndex = rand() % shakeAmmount;
			float posXx = shakeValues[RandIndex];
			if (int randNum = rand() % 2 == 1) { posXx = -posXx; }
			//URHO3D_LOGINFO("randindex "+String(RandIndex));
			RandIndex = rand() % shakeAmmount;
			float posYy = shakeValues[RandIndex];
			if (int randNum = rand() % 2 == 1) { posYy = -posYy; }
			//URHO3D_LOGINFO("randindex " + String(RandIndex));
			cameraTargetPosition = Vector3(posXx, posYy, 0);
		   // Vector2 clamped = MyTools::ClampMagnitude(Vector2(newCamPos.x_,newCamPos.y_),4);
			cameraNode_->SetWorldPosition(cameraTargetPosition);
		}
		cameraNode_->SetWorldPosition(Lerp(cameraNode_->GetPosition(), cameraTargetPosition, 0.05f));
		shakeDuration -= timeStep;
		//URHO3D_LOGERROR("Shake duration " + String(shakeDuration));
		//URHO3D_LOGERROR("TimeStep " + String(timeStep));
	}
	else 
	{
		Vector3 cameraCurrentPos = cameraNode_->GetPosition();
		cameraNode_->SetWorldPosition(Lerp(cameraCurrentPos, cameraStartPosition, 0.05f));
		shaking = false;
	}
	//URHO3D_LOGERROR(String(shakeDuration));
	//URHO3D_LOGINFO(String(zoomSize));
}
void CameraController::MultiTouch(int One, int Second,float timeStep) {
	Input* input = GetSubsystem<Input>();
	TouchState* touch1 = input->GetTouch(One);
	TouchState* touch2 = input->GetTouch(Second);
	float targetPositionX = 0;
	float targetPositionY = 0;
	targetPosition = MyTools::ScreenToWorld(touch1->delta_.x_, 0);
	Vector3 targetPos = Vector3(targetPosition.x_, targetPosition.y_, 0);
	// Check for zoom pattern (touches moving in opposite directions)
	if (touch1->touchedElement_ == NULL && touch2->touchedElement_ == NULL && ((touch1->delta_.y_ > 0 && touch2->delta_.y_ < 0) || (touch1->delta_.y_ < 0 && touch2->delta_.y_ > 0)))
		zoom = true;
	else
		zoom = false;
	if (zoom == true)
	{
		int sens = 0;
		// Check for zoom direction (in/out)
		if (abs(touch1->position_.y_ - touch2->position_.y_) > abs(touch1->lastPosition_.y_ - touch2->lastPosition_.y_))
			sens = -1;
		else
			sens = 1;

		float touchSense = 2.0f;
		//float orthoSize = cameraNode_->GetComponent<Camera>()->GetOrthoSize();
		float zoomSize = cameraNode_->GetComponent<Camera>()->GetZoom();
		//float cameraLocationX = cameraNode_->GetPosition();
		//Check if ortho size is smaler than 20
		//if (orthoSize <= 20 ) //orthoSize += abs(touch1->delta_.y_ - touch2->delta_.y_) * sens * touchSense / 50;
		if (zoomSize >= 1) {
			zoomSize -= abs(touch1->delta_.y_ - touch2->delta_.y_) * sens * touchSense / 500;
		} //TOUCH_SENSITIVITY
		//if (orthoSize > 20)//orthoSize = 19.999f;
		if (zoomSize <= 1)
		{
			zoomSize = 1;
		}
		else if (zoomSize >= 2.5)
		{
			zoomSize = 2.5;
		}
		//orthoSize = Clamp(cameraDistance, CAMERA_MIN_DIST, CAMERA_MAX_DIST); // Restrict zoom range to [1;20]
		//cameraNode_->GetComponent<Camera>()->SetOrthoSize(orthoSize);
		cameraNode_->GetComponent<Camera>()->SetZoom(zoomSize);
	}
}
void CameraController::SingleTouch(int One, float timeStep) {
	Input* input = GetSubsystem<Input>();
	TouchState* touch1 = input->GetTouch(One);
	String X_touch = "";
	String Y_touch = "";
	int windownHeight = GetSubsystem<Graphics>()->GetHeight() / 2;
	int windownWidth = GetSubsystem<Graphics>()->GetWidth() / 2;
	int posX = 0;
	int posY = 0;
	//Kosketus oikealla puolella ikkunaa
	if (touch1->position_.x_ > GetSubsystem<Graphics>()->GetWidth()/2)
	{
		posX = -windownWidth;
		posX += touch1->position_.x_;
		X_touch = "Oikealla puolella ";
		//URHO3D_LOGERROR(X_touch+ String(kohta));
	}
	else //Vasemmalla puolella ikkunaakn
	{
		posX = windownWidth - touch1->position_.x_;
		posX = -posX;
		X_touch = "Vasemmalla puolella ";
		//URHO3D_LOGERROR(X_touch + String(kohta));
	}
	//Alapuolelle
	if (touch1->position_.y_ > GetSubsystem<Graphics>()->GetHeight() / 2)
	{
		posY = -windownHeight;
		posY += touch1->position_.y_;
		posY = -posY;
		Y_touch = "Alapuolella ";
		//URHO3D_LOGERROR(Y_touch + String(kohta));
	}
	//Yläpuolelle
	else
	{
		posY = windownHeight - touch1->position_.y_;
		Y_touch = "Ylapuolella ";
		//URHO3D_LOGERROR(Y_touch + String(kohta));
	}
	//targetPosition = MyTools::ScreenToWorld(touch1->position_.x_ / 300000000, 0);
	targetPosition = Vector2(posX/100, posY/100);
	Vector3 targetPos = Vector3(targetPosition.x_, targetPosition.y_, 0);
	cameraNode_->SetWorldPosition(Lerp(targetPos, cameraStartPosition, 0.5f));
	URHO3D_LOGERROR(X_touch + String(posX) + Y_touch + String(posY));

}
void CameraController::CameraShake(float duration, int ammount, float inZoom) {
	//shakeAmmount = duration;
	shakeDuration = duration;
	shakeAmmount = ammount;
	cameraNode_->GetComponent<Camera>()->SetZoom(inZoom);
	nextRnd = shakeDuration;
	shaking = true;
	//zoomSize = inZoom;
	//URHO3D_LOGINFO("ME SET FUCKING THING " + String(shakeDuration));
}