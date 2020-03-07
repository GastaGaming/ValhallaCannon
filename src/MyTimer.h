#pragma once
#include <vector>
#include "MyTools.h"
#include <algorithm>
#include <future>
#include <chrono>
#include <thread>
#include <Urho3D/Urho3DAll.h>
using namespace Urho3D;

class MyTimer : public LogicComponent
{
	URHO3D_OBJECT(MyTimer, LogicComponent);
public:
	explicit MyTimer(Context* context);
	~MyTimer();
	//calls functin with delay of x seconds
	static Vector<MyTimer*> timers;
	bool done = false;
	float triggerTime = -1;
	std::function<void()> func;
	static void New(std::function<void()> f, float delay, float repeatDelay = 0,bool persistent = false);
	void Update(float deltaTime) override;
};

