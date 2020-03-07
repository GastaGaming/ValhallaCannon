#pragma once
#include <vector>
#include "MyTools.h"
#include <algorithm>
#include <future>
#include <chrono>
#include <thread>
#include <Urho3D/Urho3DAll.h>
#include <initializer_list>

using namespace Urho3D;

class MyAnimator : public LogicComponent
{
	URHO3D_OBJECT(MyAnimator, LogicComponent);
	
public:
	explicit MyAnimator(Context* context);
	~MyAnimator();
	void Update(float deltaTime) override;
	float startTime = 0;
	void AddPosition(float x, float y, float time);
	void AddPosition(Vector3 pos, float time) { AddPosition(pos.x_,pos.y_,time); }
	void AddRotation(float rot, float time);
	void AddScale(float scale, float time);
	void OnNodeSet(Node* node) override;
	void SetInitalTransform();
private:
	Vector<Vector2> positions;
	Vector<float> posTimes;
};