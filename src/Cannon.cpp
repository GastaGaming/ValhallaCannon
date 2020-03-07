#include "Cannon.h"
#include "Level.h"
#include "UIManager.h"
#include "CameraController.h"
Cannon* Cannon::instance = 0;

Cannon::Cannon(Context* context) : LogicComponent(context)
{
}
Cannon::~Cannon()
{
}
void Cannon::Start() {
	instance = this;
	node_->CreateComponent<RigidBody>();
	SubscribeToEvent(E_TOUCHBEGIN, URHO3D_HANDLER(Cannon, HandleTouchBegin));
	SubscribeToEvent(E_TOUCHEND, URHO3D_HANDLER(Cannon, HandleTouchEnd));
	moveSoundSource = node_->CreateComponent<SoundSource>();
	PlayMoveSound();
	if (Level::levelNumber == 1 || Level::levelNumber == 2)
	{
		helga = Level::currentLevel->helga;
		vikingVisual = helga->CreateChild("cannonVikingVisual");
		vikingVisual->CreateComponent<AnimatedSprite2D>();
		vikingVisual->Scale2D(Vector2(0.6, 0.6));
		vikingVisual->SetPosition2D(0.6f, 0);
		
	}
	else if(Level::levelNumber > 2)
	{
		vikingVisual = node_->CreateChild("cannonVikingVisual");
		vikingVisual->CreateComponent<AnimatedSprite2D>();
		vikingVisual->Scale2D(Vector2(0.1, 0.1));
		vikingVisual->SetPosition2D(0.6f, 0);
	}
	vikingVisual->CreateComponent<StaticSprite2D>()->SetSprite(MyCache->GetResource<Sprite2D>("Ball.png"));
	auto* torch = Level::currentLevel->CreateEffect(vikingVisual,"Torch.xml",1.5)->GetNode();
	torch->SetPosition2D(Vector2::RIGHT * 6.5 + Vector2::UP * 1.3);
	torch->SetEnabled(false);
}
void Cannon::DelayedStart() {
	SetupButtons();
	int* availables = Level::currentLevel->usableVikings;

	//SetAvailableVikings(*availables++, *availables++, *availables);//I am sorry but it works
	//returns indices 1,0,2??
	//apparenty undefined in our cpp version __cplusplus
	//https://en.cppreference.com/w/cpp/language/eval_order

	int* func = Level::currentLevel->GetUsableVikings();
	int arr[3] = { 0,0,0 };
	for (size_t i = 0; i < 3; i++)
	{
		arr[i] = *func;
		func++;
	}
	SetAvailableVikings(arr[0], arr[1], arr[2]);
	aimDirection = Vector3::RIGHT;
	GetComponent<StaticSprite2D>()->SetUseHotSpot(true);
	GetComponent<StaticSprite2D>()->SetHotSpot(Vector2(0.4, 0.5));
}
void Cannon::Update(float deltaTime) {
	UpdateAimRay();
	if (MyTools::instance->drawDebug)
	{
		MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine(node_->GetPosition(), node_->GetPosition() + aimDirection * power, Color::CYAN, false);
	}
	if (MyTime->GetElapsedTime() <= lastShot + recoilTime) {
		float t = (MyTime->GetElapsedTime() - lastShot) / recoilTime;//clamp stuff maybe at some point and check for drift
		float amount = Abs(t - 0.5) * 2;
		Vector2 recoilPos = originalPosition + recoilDirection.Normalized() * recoilDistance * amount;
		node_->SetPosition2D(recoilPos);
	}
	//float t = UIManager::instance->sliderValue / 10;
	//power = Lerp(minPower, maxPower, t);
	float sliderT = (power - minPower) / (maxPower - minPower);
	UIManager::instance->sliderValue = sliderT;
	//URHO3D_LOGINFO(String(sliderT));
	if (moveSoundSource->GetTimePosition() == 0)
	{
		PlayMoveSound();
	}
	if (Level::levelNumber < 3 ) 
	{
		Node* helga = MyTools::currentScene->GetChild("Helga");
		AnimatedSprite2D* helgaAnimSprite2D = helga->GetComponent<AnimatedSprite2D>();
		LoopMode2D loop = LM_FORCE_LOOPED;
		helgaAnimSprite2D->SetAnimation("Swing", loop);
		if (helgaAnimTime > 0)
		{
			helgaAnimSprite2D->SetSpeed(1);
			helgaAnimTime = helgaAnimTime - deltaTime;
			//URHO3D_LOGDEBUG("animTime " + String(helgaAnimTime));
			//URHO3D_LOGDEBUG("animSpeed " + String(helgaAnimSprite2D->GetSpeed()));
		}
		else
		{
			helgaAnimSprite2D->SetSpeed(0);
		}
	}
}
void Cannon::UpdateAimRay() {

	if (MyInput->GetNumTouches() > 0 && !uiFocus)
	{
		if (MyTools::ScreenToViewport(MyInput->GetTouch(0)->position_).y_ < 0.8)
		{
			Vector3 toAim = (Vector3)MyTools::TouchToWorld() - node_->GetPosition();
			float len = Clamp(toAim.Length(), minAimDistance, maxAimDistance);
			float t = (len - minAimDistance) / (maxAimDistance - minAimDistance);
			power = Lerp(minPower, maxPower, t);
			//URHO3D_LOGINFO("aimLenght " + String(toAim.Length()) + " minAim " + String(minAimDistance)+" maxAim " + String(maxAimDistance) + " t " + String(t) + " power " + String(power));
			aimDirection = toAim.Normalized();

		}
		targetRotation = Asin(aimDirection.Normalized().y_);
	}
	float nextRotation = Lerp(node_->GetRotation2D(), targetRotation, 0.005);//illegal as fuck make it non fps dependent
	float toTarget = targetRotation - node_->GetRotation2D();
	//moveSoundSource->SetGain(Abs(toTarget) > 1 ? 1: 0);
	float deltaRotation = toTarget * MyTime->GetTimeStep()*rotationSpeed;
	nextRotation = node_->GetRotation2D() + deltaRotation;
	node_->SetRotation2D(nextRotation);
	//cannon move volume based on rotation amount
	float vol = Abs(toTarget) / 10;
	vol = Clamp(vol, (float)0, (float)1);
	moveSoundSource->SetGain(vol);
	//URHO3D_LOGINFO("vol "+String(MyTools::Round(vol, 2)) + " toTarget " + String(MyTools::Round(toTarget,2)));
}
void Cannon::HandleTouchBegin(StringHash eventType, VariantMap& eventData) {
	if (MyInput->GetNumTouches() > 0)
	{
		IntVector2 touchPos = MyInput->GetTouch(0)->position_;
		String asd = String(touchPos); //"touchy at x" + touchPos.x_ + " y" + touchPos.y_ + "";
		//URHO3D_LOGINFO("touchy at " + asd + " viewPort " + String(MyTools::ScreenToViewport(touchPos)));
		//URHO3D_LOGINFO(String(MyTools::instance->TouchToWorld()));
	}
}
void Cannon::HandleTouchEnd(StringHash eventType, VariantMap& eventData) {
	if (!uiFocus && vikingType != -1)
	{
		ShootViking();
	}
	uiFocus = false;
}
void Cannon::ShootViking() {
	float currentTime = GetSubsystem<Time>()->GetElapsedTime();
	if (currentTime - switchTime < 0.1 || vikingType == -1 || availableVikings[vikingType] < 1)
	{
		return;
	}
	Node* viking = MyTools::currentScene->CreateChild("viikinki");
	RigidBody2D* rigidBody = viking->CreateComponent<RigidBody2D>();
	rigidBody->SetBodyType(BodyType2D::BT_DYNAMIC);
	Vector2 aimVelocity = Vector2(aimDirection.x_, aimDirection.y_)*power;
	rigidBody->SetLinearVelocity(aimVelocity);
	viking->SetRotation2D(Asin(aimVelocity.Normalized().y_));

	AnimatedSprite2D* sprite = viking->CreateComponent<AnimatedSprite2D>();
	auto* cache = GetSubsystem<ResourceCache>();
	sprite->SetLayer(-1);
	viking->SetPosition(node_->GetPosition());

	CollisionCircle2D* collider = viking->CreateComponent<CollisionCircle2D>();
	collider->SetRadius(2.5);
	Viking* asd = viking->CreateComponent<Viking>();
	float scale = 5;
	if (vikingType == VikingType::Rogue)
	{
		scale = 3;
		collider->SetRadius(3.5);
	}
	viking->SetScale2D(scale, scale);
	//collider->SetDensity(.1);//required for collision rotations
	asd->SetType(vikingType);
	if (vikingType == VikingType::Heavy)
	{
		collider->SetRadius(3.5);
		collider->SetCenter(3,1.5);
	}
	if (vikingType == VikingType::Torcher)
	{
		Node* torch = Level::currentLevel->CreateEffect(viking, "Torch.xml")->GetNode();
		
		torch->SetPosition2D(Vector2::RIGHT*6);
		torch->SetScale2D(Vector2::ONE * 1.5);
	}
	//check viking availability
	availableVikings[vikingType] -= 1;
	vikingType = -1;
	CheckAvailableVikings();

	originalPosition = node_->GetPosition2D();
	recoilDirection = -aimVelocity.Normalized();
	lastShot = MyTime->GetElapsedTime();

	//Helga
	if (Level::levelNumber < 3)
	{
		helgaAnimTime = 1;
	}
	else
	{
		Level::currentLevel->CreateSound("02_cannonfiresound.ogg");
	}
	vikingVisual->SetEnabled(false);
	auto* torch = vikingVisual->GetChild("Torch.xml");
	if (torch)
	{
		torch->SetEnabled(false);
	}
	//Shake cam
	CameraController* controll = MyTools::currentScene->GetComponent<CameraController>(true);
	if (!controll->GetComponent<CameraController>()) {
		URHO3D_LOGINFO("camcontroller is null");
	}
	else
	{
		if (Level::levelNumber > 2)
		{
			controll->CameraShake(0.5, 1, 1.15f);
		}
	}
}
void Cannon::CheckAvailableVikings() {
	for (size_t i = 0; i < availableVikingsSize; i++)
	{
		SetButton(i, availableVikings[i] > 0);
		Text* amountText = static_cast<Text*>(MyUI->GetRoot()->GetChild(Viking::TypeToName(i) + " number", true));
		amountText->SetText(String(availableVikings[i]));
		Button* vikingButton = static_cast<Button*>(MyUI->GetRoot()->GetChild(GetVikingButtonName(i), true));
		//URHO3D_LOGINFO("CHECKING VIKINGS, type is " + String(vikingType) + " current i is " + String(i));
		vikingButton->SetColor(i == vikingType ? Color::WHITE : Color::GRAY);
	}
}
void Cannon::SetupButtons() {
	UI* ui = GetSubsystem<UI>();
	Button* buttonHeavy = static_cast<Button*>(ui->GetRoot()->GetChild(GetVikingButtonName(0), true));
	SubscribeToEvent(buttonHeavy, E_PRESSED, URHO3D_HANDLER(Cannon, SelectHeavy));
	Button* buttonRogue = static_cast<Button*>(ui->GetRoot()->GetChild(GetVikingButtonName(1), true));
	SubscribeToEvent(buttonRogue, E_PRESSED, URHO3D_HANDLER(Cannon, SelectRogue));
	Button* buttonTorcher = static_cast<Button*>(ui->GetRoot()->GetChild(GetVikingButtonName(2), true));
	SubscribeToEvent(buttonTorcher, E_PRESSED, URHO3D_HANDLER(Cannon, SelectTorcher));
	//Slider* powerSlider = static_cast<Slider*>(ui->GetRoot()->GetChild("powerSlider", true));
	//SubscribeToEvent(powerSlider, E_SLIDERPAGED, URHO3D_HANDLER(Cannon, UIFocus));//PERKELE, only gets called when the background of the slider is clicked -.-
	//SubscribeToEvent(powerSlider, E_SLIDERCHANGED, URHO3D_HANDLER(Cannon, UIFocus));
	buttonHeavy->SetVisible(true);
	buttonHeavy->SetEnabled(true);
	buttonRogue->SetVisible(true);
	buttonRogue->SetEnabled(true);
	buttonTorcher->SetVisible(true);
	buttonTorcher->SetEnabled(true);
	CheckAvailableVikings();
}
void Cannon::SelectHeavy(StringHash eventType, VariantMap& eventData) {
	SetVikingType(0);
	//URHO3D_LOGINFO("selected heavy");
}
void Cannon::SelectRogue(StringHash eventType, VariantMap& eventData) {
	SetVikingType(1);
	//URHO3D_LOGINFO("selected rogue");
}
void Cannon::SelectTorcher(StringHash eventType, VariantMap& eventData) {
	SetVikingType(2);
	//URHO3D_LOGINFO("selected torcher");
}
void Cannon::SelectShoot(StringHash eventType, VariantMap& eventData) {
	ShootViking();
	SetSwitchTime();
}
void Cannon::SetAvailableVikings(int heavys, int rogues, int torchers) {
	availableVikings[0] = heavys;
	availableVikings[1] = rogues;
	availableVikings[2] = torchers;
	CheckAvailableVikings();
}
void Cannon::SetVikingType(int type) {
	//check if any available before switching instead of when shooting
	//set visuals etc
	//update switchtime
	vikingType = type;
	SetSwitchTime();

	//visual
	vikingVisual->RemoveComponent<AnimatedSprite2D>();
	auto* anim = vikingVisual->CreateComponent<AnimatedSprite2D>();
	vikingVisual->SetEnabled(true);
	auto* torch = vikingVisual->GetChild("Torch.xml");
	if (torch)
	{
		torch->SetEnabled(type == VikingType::Torcher);
	}
	String sprites[] = { "Anim_Heavy/Heavy_anims.scml" ,"Anim_Rogue/Anim_Rogue.scml","Anim_Torcher/TorcherAnimation.scml" };
	anim->SetAnimationSet(MyCache->GetResource<AnimationSet2D>("Urho2D/" + sprites[type]));
	if (Level::levelNumber < 3) 
	{
		anim->SetAnimation("HelgaHold");
	}
	anim->SetAnimation("Flight");
	anim->SetSpeed(0.3f);
	anim->SetLayer(0);
	anim->SetOrderInLayer(-2);
	Level::currentLevel->CreateSound("PlayerFistHit.wav", 0.4);
	CheckAvailableVikings();
}
void Cannon::SetSwitchTime() {
	float currentTime = GetSubsystem<Time>()->GetElapsedTime();
	switchTime = currentTime;
	uiFocus = true;
}
void Cannon::SetButton(int type, bool enabled) {
	auto* ui = GetSubsystem<UI>();
	String name = GetVikingButtonName(type);
	ui->GetRoot()->GetChild(name, true)->SetEnabled(enabled);
	ui->GetRoot()->GetChild(name, true)->SetVisible(enabled);
}
