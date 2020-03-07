#include "MyTimer.h"
Vector<MyTimer*>  MyTimer::timers;
MyTimer::MyTimer(Context* context) :LogicComponent(context) {

}
MyTimer::~MyTimer()
{

}
void MyTimer::New(std::function<void()> f, float delay, float repeatDelay, bool persistent) {
	MyTimer* paskvitu = 0;
	if (persistent)//creating persistent timers on scene so they stay between level loads, otherwise they get destroyed as child of scene
	{
		paskvitu = MyTools::currentScene->CreateComponent<MyTimer>();
	}
	else
	{
		paskvitu = MyTools::currentScene->CreateChild("timer")->CreateComponent<MyTimer>();
	}
	if (repeatDelay > 0)
	{
		paskvitu->func = [=]() {
			f();
			MyTimer::New([=]() {
				f();
			}, repeatDelay, repeatDelay,persistent);
		};
	}
	else
	{
		paskvitu->func = f;
	}
	paskvitu->triggerTime = MyTime->GetElapsedTime() + delay;
}
void MyTimer::Update(float deltaTime) {//dependent on timeScale, wont run on timeScale 0
	if (MyTime->GetElapsedTime() >= triggerTime && done == false)
	{
		done = true;
		func();
	}
}