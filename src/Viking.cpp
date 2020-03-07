#include "Viking.h"
#include "Level.h"
#include "CameraController.h"
Viking::Viking(Context* context) : LogicComponent(context)//type(ptype)
{

}
Viking::~Viking()
{
}
void Viking::HandleNodeCollision(StringHash eventType, VariantMap& eventData) {
	//URHO3D_LOGINFO("Handled collision");
	//using namespace NodeCollision;
	Node* otherNode = static_cast<Node*>(eventData["OtherNode"].GetPtr());
	RigidBody2D* otherBody = static_cast<RigidBody2D*>(eventData["OtherBody"].GetPtr());
	VectorBuffer contacts = static_cast<VectorBuffer>(eventData["Contacts"].GetBuffer());
	if (otherNode/* && otherNode->GetPosition().y_ > node_->GetPosition().y_*/)
	{
		//node_->SetEnabled(false);
		//otherBody->SetBodyType(BodyType2D::BT_STATIC);
	}
	if (type == VikingType::Torcher)
	{
		Node* torch = node_->GetChild("Torch.xml");
		if (torch)
		{
			torch->SetEnabled(false);
		}
	}
	MyPrint("oldVel ", oldVelocity);
	CameraController* controll = MyTools::currentScene->GetComponent<CameraController>(true);
	if (controll)
	{
		float minVel = 5;
		if (oldVelocity > minVel)
		{
			if (VikingType::Heavy == type)
			{
				controll->CameraShake(0.8f, 6, 1.2f);
			}
			if (VikingType::Rogue == type)
			{
				controll->CameraShake(0.3f, 2, 1.2f);
			}
			if (VikingType::Torcher == type)
			{
				controll->CameraShake(0.4f, 3, 1.2f);
			}
		}
	}
	//check after collision for disable
	MyTimer::New([=]() {
		if (node_)
		{
			auto* rb = GetComponent<RigidBody2D>();
			if (rb && rb->GetBodyType() != BodyType2D::BT_STATIC)
			{
				node_->SetEnabled(false);
			}
		}
	}, 2.5);
}
void Viking::OnNodeSet(Node* node) {
	if (node) {
		SubscribeToEvent(node_, E_NODEBEGINCONTACT2D, URHO3D_HANDLER(Viking, HandleNodeCollision));
		//URHO3D_LOGINFO("Subscribed to collision");
	}

}
void Viking::Update(float deltaTime) {

	//align rotation to match velocity (maybe do in fixedupdate instead)

	//disable if goes out of view
	if (MyTools::DistanceFromView(node_->GetPosition2D()) > 5)
	{
		node_->SetEnabled(false);
		Level::currentLevel->CheckLose();
	}
	RigidBody2D* rigidBody = GetComponent<RigidBody2D>();
	if (rigidBody)
	{
		node_->SetRotation2D(Asin(rigidBody->GetLinearVelocity().Normalized().y_));
	}
	MyTimer::New([=]() {
		oldVelocity = rigidBody->GetLinearVelocity().Length();
	}, velocityDelay);
}
void Viking::SetType(int type) {
	auto* cache = GetSubsystem<ResourceCache>();

	this->type = type;
	//could have a path for each viking types animations and sprites separated by name_asset to assign them easier?
	String names[] = { "Heavy","Rogue","Torcher" };
	String sprites[] = { "Anim_Heavy/Heavy_anims.scml" ,"Anim_Rogue/Anim_Rogue.scml","Anim_Torcher/TorcherAnimation.scml" };
	node_->SetName(names[type]);
	node_->GetComponent<AnimatedSprite2D>()->SetAnimationSet(cache->GetResource<AnimationSet2D>("Urho2D/" + sprites[type]));
	node_->GetComponent<AnimatedSprite2D>()->SetAnimation("Flight");
	node_->Scale2D(Vector2(0.1, 0.1));

	String soundName = "";
	switch (type)
	{
	case VikingType::Heavy:
		soundName = "05_heavy.ogg";
		break;
	case VikingType::Rogue:
		soundName = "03_roque.ogg";
		break;
	case VikingType::Torcher:
		soundName = "04_torcher.ogg";
		break;
	default:
		break;
	}
	Level::currentLevel->CreateSound(soundName);
}
int Viking::NameToType(String name) {
	if (name == "Heavy")
	{
		return 0;
	}
	else if (name == "Rogue")
	{
		return 1;
	}
	else if (name == "Torcher")
	{
		return 2;
	}
	return -1;
}
String Viking::TypeToName(int type) {
	switch (type)
	{
	case 0:
		return "Heavy";
		break;
	case 1:
		return "Rogue";
		break;
	case 2:
		return "Torcher";
		break;
	default:
		return "";
		break;
	}
}
