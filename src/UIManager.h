#pragma once
/// Original Creator: Karhu
#include <Urho3D/Urho3DAll.h>
#include <string>
#include <sstream>
#include <locale>
#include <clocale>
namespace Urho3D
{
	class Button;
	class UIElement;
}

class UIManager : public Object
{
	URHO3D_OBJECT(UIManager, Object);
public:
	static UIManager* instance;
	//UIElement root_;
	UIElement* whoosh;
	String lastEvent;
	explicit UIManager(Context* context);
	void CreateMenus();
	void HideAll();
	void GetOne(int i);
	void ToggleAudio(StringHash eventType, VariantMap& eventData);
	void ToggleBackground(StringHash eventType, VariantMap& eventData);
	void OpenOne(StringHash eventType, VariantMap& eventData);
	void GoToLevel(StringHash eventType, VariantMap& eventData);
	void RestartLevel(StringHash eventType, VariantMap& eventData);
	void ChooseLevel(int i);
	static void RegisterObject(Context* context);
	float screenMult;
	void Update();
	int whatUI;
	void FPS();
	float currentSize;
	int framecount_;
	float time_ = 0;
	void ButtonHandler();
	static bool audioOn;
	static bool bgOn;
	static int unlockedLevel;
	void UIReset();
	float sliderValue =0;
	void comicHandler(bool prelevel,int i);
private:
	Vector2 screenSize;
	WeakPtr<UIElement> ui_;
	SharedPtr<UIElement> mainMenu_;
	SharedPtr<UIElement> settingsMenu_;
	SharedPtr<UIElement> levelSelector_;
	SharedPtr<UIElement> pauseMenu_;
	SharedPtr<UIElement> levelLose_;
	SharedPtr<UIElement> levelWin_;
	SharedPtr<UIElement> gameUI_;
	SharedPtr<UIElement> splash_;
	SharedPtr<UIElement> startComic_;
	SharedPtr<UIElement> comic_;
	SharedPtr<Sprite> comic;
	SharedPtr<Sprite> powerSlider_;
	SharedPtr<Text> text_;
	String muteIcon = "Urho2D/Spr_ButtonMuteOff.png";
	String bgIcon = "Urho2D/Spr_ButtonBgToggle.png";
};

