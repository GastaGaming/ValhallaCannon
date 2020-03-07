#include "Interactable.h"
#include "Level.h"
#include "CameraController.h"
Interactable::Interactable(Context* context) : LogicComponent(context)
{

}
Interactable::~Interactable()
{
}
//check the platforming example for getting the coliders from tiled and make an init func
void Interactable::OnNodeSet(Node* node) {
	//URHO3D_LOGINFO("Created new interactable");
	SubscribeToEvent(node_, E_NODEBEGINCONTACT2D, URHO3D_HANDLER(Interactable, HandleCollisionBegin));
}
void Interactable::HandleCollisionBegin(StringHash eventType, VariantMap& eventData) {
	Node* otherNode = static_cast<Node*>(eventData["OtherNode"].GetPtr());
	RigidBody2D* otherBody = static_cast<RigidBody2D*>(eventData["OtherBody"].GetPtr());
	VectorBuffer contacts = static_cast<VectorBuffer>(eventData["Contacts"].GetBuffer());
	Viking* viking = otherNode->GetComponent<Viking>();
	Interactable* interactable = otherNode->GetComponent<Interactable>();
	//URHO3D_LOGINFO(otherNode->GetName() + " collided with " + node_->GetName());
	if (viking && interactable == NULL && otherBody->GetBodyType() == BodyType2D::BT_DYNAMIC)
	{
		if (defaultInteraction)
		{
			if (winCondition)
			{
				Level::currentLevel->DelayedWin();
			}
			switch (defaultInteraction) {
			case Interaction::Lock:
				break;
			case Interaction::Birb:
				break;
			case Interaction::Tree:
				break;
			case Interaction::Enemy:
				if (viking->type == VikingType::Heavy)
				{
					viking->GetNode()->SetEnabled(false);
					node_->SetEnabled(false);
				}
					break;
			}
		}
		if (viking->type == VikingType::Heavy && heavySticks)
		{
			viking->GetNode()->GetComponent<AnimatedSprite2D>()->SetAnimation("Landing");
			viking->GetComponent<RigidBody2D>()->SetBodyType(BodyType2D::BT_STATIC);
			viking->GetNode()->CreateComponent<Interactable>()->heavySticks = true;
			//viking->GetNode()->RemoveComponent<Viking>();
			Level::currentLevel->CreateSound("08_heavyhitobject.ogg");
		}
		if (Level::currentLevel)
		{
			Level::currentLevel->HandleInteraction(viking, this);
		}
		//Camera* cam = MyCamera;
		//Node camN = cam->GetNode;
		//* cam = MyCamera->GetNode();
		//cam->GetComponent<CameraController>()->CameraShake(4.0f, 3.0f);
		//camN.GetComponent<CameraController>()->CameraShake(4.0f,3.0f);
		//MyCamera->GetComponent<CameraController>()->CameraShake(4.0f, 3.0f);
		CameraController* controll = MyTools::currentScene->GetComponent<CameraController>(true);
		if (!controll->GetComponent<CameraController>()) {
			URHO3D_LOGINFO("camcontroller is null");
		}
		else
		{
			/*if(VikingType::Heavy == viking->type)
			{
				controll->CameraShake(0.8f, 6, 1.2f);
			}
			if (VikingType::Rogue == viking->type)
			{
				controll->CameraShake(0.3f, 2, 1.2f);
			}
			if (VikingType::Torcher == viking->type)
			{
				controll->CameraShake(0.4f, 3, 1.2f);
			}*/
		}
		//cam->GetComponent<CameraController>()->CameraShake(100.0f, 300.0f);
		//CameraController::instance->CameraShake(100.0f, 300.0f);
		//cam->GetNode()->GetComponent<CameraController>()->CameraShake(100.0f, 300.0f);
		
		//cam->CameraShake(300.0f, 400.9f);
		//MyTools::currentScene->GetComponent<CameraController>()->CameraShake(100.0f, 300.0f);
		//cam = CameraController::cameraNode_
	}
}
void Interactable::Update(float deltaTime) {
	//MyTools::currentScene->GetComponent<PhysicsWorld2D>()->DrawDebugGeometry(MyTools::currentScene->GetComponent<DebugRenderer>(), false);
	//CollisionBox2D* box = GetComponent<CollisionBox2D>();
	//box->SetSize(Vector2::ONE*5);
	//MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine((Vector3)box->GetCenter(),(Vector3)(box->GetCenter()+Vector2::UP*5), Color::CYAN, false);
	//MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine(node_->GetPosition(), (node_->GetPosition() + Vector3::UP * 5), Color::CYAN, false);
}