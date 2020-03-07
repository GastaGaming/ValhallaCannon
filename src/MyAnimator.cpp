#include "MyAnimator.h"

MyAnimator::MyAnimator(Context* context) :LogicComponent(context) {

}
MyAnimator::~MyAnimator()
{

}
void MyAnimator::OnNodeSet(Node* node) {
	SetInitalTransform();
}
void MyAnimator::SetInitalTransform() {
	positions.Push(node_->GetPosition2D());
	posTimes.Push(0);
	startTime = MyTime->GetElapsedTime();
}

void MyAnimator::Update(float deltaTime) {//dependent on timeScale, wont run on timeScale 0
	float playTime = MyTime->GetElapsedTime() - startTime;
	int posFrame = 0;
	for (size_t i = 0; i < posTimes.Size(); i++)
	{
		if (posTimes[i] > playTime)
		{
			break;
		}
		posFrame = i;
	}
	Vector2 position = positions[positions.Size()-1];
	if (posFrame < posTimes.Size() - 1)
	{
		float frameDuration = posTimes[posFrame + 1] - posTimes[posFrame];
		float frameTime = playTime - posTimes[posFrame];
		float t = frameTime / frameDuration;
		Vector2 lerpPos = positions[posFrame];
		Vector2 toNext = positions[posFrame + 1];
		lerpPos += toNext * t;
		position = lerpPos;
		URHO3D_LOGINFO("frame " +String(posFrame) + " time " + String(posTimes[posFrame]) + " pos " + String(positions[posFrame]) );
	}
	node_->SetPosition2D(position);
}
void MyAnimator::AddPosition(float x, float y, float time) {
	Vector2 position(x, y);
	positions.Push(position);
	posTimes.Push(time);
}

