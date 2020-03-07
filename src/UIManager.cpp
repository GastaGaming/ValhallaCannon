#include "UIManager.h"
#include <string>
#include "Level.h"
#include "Cannon.h"

UIManager* UIManager::instance = 0;
bool UIManager::audioOn = true;
bool UIManager::bgOn = true;
int UIManager::unlockedLevel = 1;
UIManager::UIManager(Context* context) : Object(context)
{
	instance = this;
}
void UIManager::RegisterObject(Context* context)
{
	context->RegisterFactory<UIManager>();
}
/// Original Creator: Karhu
void UIManager::CreateMenus() // Make the menu here
{
	
	screenSize = Vector2(GetSubsystem<Graphics>()->GetWidth(), GetSubsystem<Graphics>()->GetHeight());
	screenMult = 1080 / screenSize.y_;
	currentSize = screenMult;
	UIElement* root_ = GetSubsystem<UI>()->GetRoot();
	whoosh = GetSubsystem<UI>()->GetRoot();
	auto* cache = GetSubsystem<ResourceCache>();
	auto* ui = GetSubsystem<UI>();

	// Text will be updated later in the E_UPDATE handler. Keep readin'.
	// If the engine cannot find the font, it comes with Urho3D.
	// Set the environment variables URHO3D_HOME, URHO3D_PREFIX_PATH or
	// change the engine parameter "ResourcePrefixPath" in the Setup method.

	/*text_ = root_->CreateChild<Text>();
	text_->SetText("Keys: tab = toggle mouse, AWSD = move camera, Shift = fast mode, Esc = quit.\nFPS:2000");
	text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20);
	text_->SetColor(Color(.3, 0, .3));
	text_->SetHorizontalAlignment(HA_CENTER);
	text_->SetVerticalAlignment(VA_TOP);*/




	// |------------| MAIN MENU |------------|
#pragma region MAINMENU


	mainMenu_ = root_->CreateChild<Window>();
	mainMenu_->SetSize(screenSize.x_, screenSize.y_);
	mainMenu_->SetPosition(0, 0);
	mainMenu_->SetColor(Color(1, 1, 1, 0)); // sets the window invisible so it doesnt get in the way

	auto* mainMenu = mainMenu_->CreateChild<UIElement>();
	mainMenu->SetSize(screenSize.x_, screenSize.y_);

	auto* bg = mainMenu->CreateChild<Sprite>();
	bg->SetStyleAuto();
	bg->SetSize(screenSize.x_, screenSize.y_);
	bg->SetPosition(0, 0);
	bg->SetTexture(cache->GetResource<Texture2D>("Urho2D/Vikings_SplashScreenBG.png"));

	auto* ship = mainMenu->CreateChild<Sprite>();
	ship->SetStyleAuto();
	ship->SetAlignment(HA_CENTER, VA_BOTTOM);
	ship->SetSize(664 / screenMult * 1.5, 531 / screenMult * 1.5);
	ship->SetPosition(-664 / screenMult * 1.5 / 2, -531 / screenMult * 1.5);
	ship->SetTexture(cache->GetResource<Texture2D>("Urho2D/OnShip.png"));

	auto* mLogo = mainMenu->CreateChild<Sprite>();
	mLogo->SetStyleAuto();
	mLogo->SetSize(screenSize.x_/2, screenSize.y_/2);
	mLogo->SetPosition(0, 0);
	mLogo->SetTexture(cache->GetResource<Texture2D>("Urho2D/valhallacannonlogo.png"));

	auto* startButton = mainMenu->CreateChild<Button>("Start button");
	startButton->SetSize(407 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	startButton->SetAlignment(HA_RIGHT, VA_CENTER);
	startButton->SetPosition(-100 / screenMult, -200 / screenMult);
	startButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_PlayButton.png"));
	SubscribeToEvent(startButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* optionsButton = mainMenu->CreateChild<Button>("Options button");
	optionsButton->SetSize(407 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	optionsButton->SetAlignment(HA_RIGHT, VA_CENTER);
	optionsButton->SetPosition(-100 / screenMult, 100 / screenMult);
	optionsButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonOptions.png"));
	SubscribeToEvent(optionsButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* quitButton = mainMenu->CreateChild<Button>("Quit button");
	quitButton->SetSize(244 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	quitButton->SetAlignment(HA_RIGHT, VA_BOTTOM);
	quitButton->SetPosition(-5, -5);
	quitButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonExit.png"));
	SubscribeToEvent(quitButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));
#pragma endregion
	// |------------| MAIN MENU END |------------|



	// |------------| SETTINGS MENU |------------|
#pragma region SETTINGMENU


	settingsMenu_ = root_->CreateChild<Window>();
	settingsMenu_->SetSize(screenSize.x_, screenSize.y_);
	settingsMenu_->SetPosition(0, 0);
	settingsMenu_->SetColor(Color(1, 1, 1, 0));

	auto* settingsMenu = settingsMenu_->CreateChild<UIElement>();
	settingsMenu->SetSize(screenSize.x_, screenSize.y_);

	auto* img = settingsMenu->CreateChild<Sprite>();
	img->SetStyleAuto();
	img->SetSize(screenSize.x_, screenSize.y_);
	img->SetPosition(0, 0);
	img->SetTexture(cache->GetResource<Texture2D>("Urho2D/Vikings_SplashScreenBG.png"));

	auto* settingsBG = settingsMenu->CreateChild<Sprite>();
	settingsBG->SetStyleAuto();
	settingsBG->SetSize(screenSize.x_, screenSize.y_);
	settingsBG->SetPosition(0, 0);
	settingsBG->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_MenuBlackBG.png"));
	settingsBG->SetColor(Color(1, 1, 1, 0.99));

	auto* credits = settingsBG->CreateChild<Sprite>("Credits");
	credits->SetSize(1272/screenMult,644/screenMult);
	credits->SetAlignment(HA_CENTER, VA_CENTER);
	credits->SetPosition(-50 / screenMult, -200/screenMult);
	credits->SetTexture(cache->GetResource<Texture2D>("Urho2D/Credits.png"));
	credits->SetHotSpot(credits->GetSize() / 2);

	auto* creditsHeader = credits->CreateChild<Text>("Credits header");
	creditsHeader->BringToFront();
	creditsHeader->SetText("Credits");
	creditsHeader->SetTextAlignment(HA_CENTER);
	creditsHeader->SetAlignment(HA_CENTER, VA_TOP);
	creditsHeader->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 50 / screenMult);
	creditsHeader->SetPosition(0,0);

	auto* creditsProducerNDesign = credits->CreateChild<Text>("Credits Producer & Design");
	creditsProducerNDesign->BringToFront();
	creditsProducerNDesign->SetMaxWidth(600);
	creditsProducerNDesign->SetText("Producer: Kim Kupiainen, Designer: Marika Mustalahti");
	creditsProducerNDesign->SetTextAlignment(HA_CENTER);
	creditsProducerNDesign->SetAlignment(HA_CENTER, VA_BOTTOM);
	creditsProducerNDesign->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 25 / screenMult);
	creditsProducerNDesign->SetPosition(0, 0);

	String artists = String("Artists: Katja Kaikkonen, Konsta Kantoluoto, Matti Löytönen");
	artists = artists.AppendUTF8(0);

	auto* creditsArtists = credits->CreateChild<Text>("Credits Artists");
	creditsArtists->BringToFront();
	creditsArtists->SetMaxWidth(600);
	creditsArtists->SetText(artists);
	creditsArtists->SetTextAlignment(HA_CENTER);
	creditsArtists->SetAlignment(HA_CENTER, VA_BOTTOM);
	creditsArtists->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 25 / screenMult);
	creditsArtists->SetPosition(0, 50);

	auto* creditsProgrammers1 = credits->CreateChild<Text>("Credits Programmers");
	creditsProgrammers1->BringToFront();
	creditsProgrammers1->SetMaxWidth(600);
	creditsProgrammers1->SetText("Programmers: Joni Mustakangas, Harri Huhta");
	creditsProgrammers1->SetTextAlignment(HA_CENTER);
	creditsProgrammers1->SetAlignment(HA_CENTER, VA_BOTTOM);
	creditsProgrammers1->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 25 / screenMult);
	creditsProgrammers1->SetPosition(0, 100);

	auto* creditsProgrammers2 = credits->CreateChild<Text>("Credits Programmers");
	creditsProgrammers2->BringToFront();
	creditsProgrammers2->SetMaxWidth(600);
	creditsProgrammers2->SetText(" Janne Karhu, Vili Tuppurainen");
	creditsProgrammers2->SetTextAlignment(HA_CENTER);
	creditsProgrammers2->SetAlignment(HA_CENTER, VA_BOTTOM);
	creditsProgrammers2->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 25 / screenMult);
	creditsProgrammers2->SetPosition(0, 150);

	auto* closeSettButton = settingsMenu->CreateChild<Button>("Close settings");
	closeSettButton->SetSize(244 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	closeSettButton->SetAlignment(HA_RIGHT, VA_TOP);
	closeSettButton->SetPosition(-300 / screenMult, 50 / screenMult);
	closeSettButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonExit.png"));
	SubscribeToEvent(closeSettButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* toggleAudioButton = settingsMenu->CreateChild<Button>("Toggle audio");
	toggleAudioButton->SetSize(225 / screenMult, 225 / screenMult);
	toggleAudioButton->SetAlignment(HA_CENTER, VA_CENTER);
	toggleAudioButton->SetPosition(600 / screenMult, 200 / screenMult);
	toggleAudioButton->SetTexture(cache->GetResource<Texture2D>(muteIcon));
	SubscribeToEvent(toggleAudioButton, E_RELEASED, URHO3D_HANDLER(UIManager, ToggleAudio));

	auto* toggleBGButton = settingsMenu->CreateChild<Button>("Toggle background");
	toggleBGButton->SetSize(225 / screenMult, 225 / screenMult);
	toggleBGButton->SetAlignment(HA_CENTER, VA_CENTER);
	toggleBGButton->SetPosition(600 / screenMult, -100 / screenMult);
	toggleBGButton->SetTexture(cache->GetResource<Texture2D>(bgIcon));
	SubscribeToEvent(toggleBGButton, E_RELEASED, URHO3D_HANDLER(UIManager, ToggleBackground));
#pragma endregion
	// |------------| SETTINGS MENU END |------------|



	// |------------| LEVEL SELECTOR |------------|
#pragma region LEVELSELECTOR


	levelSelector_ = root_->CreateChild<Window>();
	levelSelector_->SetSize(screenSize.x_, screenSize.y_);
	levelSelector_->SetPosition(0, 0);
	levelSelector_->SetColor(Color(1, 1, 1, 0));

	auto* levelSelector = levelSelector_->CreateChild<UIElement>();
	levelSelector->SetSize(screenSize.x_, screenSize.y_);

	auto* image = levelSelector->CreateChild<Sprite>();
	image->SetStyleAuto();
	image->SetSize(screenSize.x_, screenSize.y_);
	image->SetPosition(0, 0);
	image->SetTexture(cache->GetResource<Texture2D>("Urho2D/Vikings_SplashScreenBG.png"));

	auto* levelSelectBG = levelSelector->CreateChild<Sprite>();
	levelSelectBG->SetStyleAuto();
	levelSelectBG->SetSize(screenSize.x_ , screenSize.y_);
	levelSelectBG->SetPosition(0, 0);
	levelSelectBG->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_MenuBlackBG.png"));
	levelSelectBG->SetColor(Color(1, 1, 1, 0.99));

	auto* returnButton = levelSelectBG->CreateChild<Button>("MainMenu button");
	returnButton->SetSize(244 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	returnButton->SetAlignment(HA_RIGHT, VA_TOP);
	returnButton->SetPosition(-300 / screenMult, 50 / screenMult);
	returnButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonExit.png"));
	SubscribeToEvent(returnButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* level1 = levelSelector->CreateChild<Button>("Level 1");
	level1->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level1->SetAlignment(HA_CENTER, VA_CENTER);
	level1->SetPosition(-600 / screenMult, -400 / screenMult);
	level1->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level1, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text1 = level1->CreateChild<Text>("Text 1");
	text1->SetTextAlignment(HA_CENTER);
	text1->SetAlignment(HA_CENTER, VA_CENTER);
	text1->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text1->SetText("1");

	auto* level2 = levelSelector->CreateChild<Button>("Level 2");
	level2->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level2->SetAlignment(HA_CENTER, VA_CENTER);
	level2->SetPosition(-400 / screenMult, -400 / screenMult);
	level2->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level2, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));
	
	
	auto* text2 = level2->CreateChild<Text>("Text 2");
	text2->SetTextAlignment(HA_CENTER);
	text2->SetAlignment(HA_CENTER, VA_CENTER);
	text2->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text2->SetText("2");
	level2->SetVisible(unlockedLevel >= 2);

	auto* level3 = levelSelector->CreateChild<Button>("Level 3");
	level3->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level3->SetAlignment(HA_CENTER, VA_CENTER);
	level3->SetPosition(-200 / screenMult, -400 / screenMult);
	level3->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level3, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));


	auto* text3 = level3->CreateChild<Text>("Text 3");
	text3->SetTextAlignment(HA_CENTER);
	text3->SetAlignment(HA_CENTER, VA_CENTER);
	text3->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text3->SetText("3");
	level3->SetVisible(unlockedLevel >= 3);

	auto* level4 = levelSelector->CreateChild<Button>("Level 4");
	level4->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level4->SetAlignment(HA_CENTER, VA_CENTER);
	level4->SetPosition(0 / screenMult, -400 / screenMult);
	level4->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level4, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));


	auto* text4 = level4->CreateChild<Text>("Text 4");
	text4->SetTextAlignment(HA_CENTER);
	text4->SetAlignment(HA_CENTER, VA_CENTER);
	text4->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text4->SetText("4");
	level4->SetVisible(unlockedLevel >= 4);

	auto* level5 = levelSelector->CreateChild<Button>("Level 5");
	level5->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level5->SetAlignment(HA_CENTER, VA_CENTER);
	level5->SetPosition(200 / screenMult, -400 / screenMult);
	level5->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level5, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));


	auto* text5 = level5->CreateChild<Text>("Text 5");
	text5->SetTextAlignment(HA_CENTER);
	text5->SetAlignment(HA_CENTER, VA_CENTER);
	text5->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text5->SetText("5");
	level5->SetVisible(unlockedLevel >= 5);

	auto* level6 = levelSelector->CreateChild<Button>("Level 6");
	level6->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level6->SetAlignment(HA_CENTER, VA_CENTER);
	level6->SetPosition(-600 / screenMult, 0 / screenMult);
	level6->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level6, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text6 = level6->CreateChild<Text>("Text 6");
	text6->SetTextAlignment(HA_CENTER);
	text6->SetAlignment(HA_CENTER, VA_CENTER);
	text6->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text6->SetText("6");
	level6->SetVisible(unlockedLevel >= 6);

	auto* level7 = levelSelector->CreateChild<Button>("Level 7");
	level7->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level7->SetAlignment(HA_CENTER, VA_CENTER);
	level7->SetPosition(-400 / screenMult, 0 / screenMult);
	level7->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level7, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text7 = level7->CreateChild<Text>("Text 7");
	text7->SetTextAlignment(HA_CENTER);
	text7->SetAlignment(HA_CENTER, VA_CENTER);
	text7->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text7->SetText("7");
	level7->SetVisible(unlockedLevel >= 7);

	auto* level8 = levelSelector->CreateChild<Button>("Level 8");
	level8->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level8->SetAlignment(HA_CENTER, VA_CENTER);
	level8->SetPosition(-200 / screenMult, 0 / screenMult);
	level8->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level8, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text8 = level8->CreateChild<Text>("Text 8");
	text8->SetTextAlignment(HA_CENTER);
	text8->SetAlignment(HA_CENTER, VA_CENTER);
	text8->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text8->SetText("8");
	level8->SetVisible(unlockedLevel >= 8);

	auto* level9 = levelSelector->CreateChild<Button>("Level 9");
	level9->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level9->SetAlignment(HA_CENTER, VA_CENTER);
	level9->SetPosition(0 / screenMult, 0 / screenMult);
	level9->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level9, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text9 = level9->CreateChild<Text>("Text 9");
	text9->SetTextAlignment(HA_CENTER);
	text9->SetAlignment(HA_CENTER, VA_CENTER);
	text9->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text9->SetText("9");
	level9->SetVisible(unlockedLevel >= 9);

	auto* level10 = levelSelector->CreateChild<Button>("Level 10");
	level10->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level10->SetAlignment(HA_CENTER, VA_CENTER);
	level10->SetPosition(200 / screenMult, 0 / screenMult);
	level10->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level10, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text10 = level10->CreateChild<Text>("Text 10");
	text10->SetTextAlignment(HA_CENTER);
	text10->SetAlignment(HA_CENTER, VA_CENTER);
	text10->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text10->SetText("10");
	level10->SetVisible(unlockedLevel >= 10);

	auto* level11 = levelSelector->CreateChild<Button>("Level 11");
	level11->SetSize(244 * 0.65 / screenMult, 245 * 0.65 / screenMult);
	level11->SetAlignment(HA_CENTER, VA_CENTER);
	level11->SetPosition(400 / screenMult, 0 / screenMult);
	level11->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonLevelSelector01.png"));
	SubscribeToEvent(level11, E_RELEASED, URHO3D_HANDLER(UIManager, GoToLevel));

	auto* text11 = level11->CreateChild<Text>("Text 11");
	text11->SetTextAlignment(HA_CENTER);
	text11->SetAlignment(HA_CENTER, VA_CENTER);
	text11->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 60 / screenMult);
	text11->SetText("11");
	level11->SetVisible(unlockedLevel >= 11);
#pragma endregion
	// |------------| LEVEL SELECTOR END |------------|



	// |------------| PAUSE MENU |------------|
#pragma region PAUSEMENU

	pauseMenu_ = root_->CreateChild<Window>();
	pauseMenu_->SetSize(screenSize.x_, screenSize.y_);
	pauseMenu_->SetPosition(0, 0);
	pauseMenu_->SetColor(Color(1, 1, 1, 0));

	auto* pauseMenu = pauseMenu_->CreateChild<UIElement>();
	pauseMenu->SetSize(screenSize.x_, screenSize.y_);

	auto* pauseBG = pauseMenu->CreateChild<Sprite>();
	pauseBG->SetStyleAuto();
	pauseBG->SetSize(screenSize.x_, screenSize.y_);
	pauseBG->SetPosition(0, 0);
	pauseBG->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_MenuBlackBG.png"));
	pauseBG->SetColor(Color(1, 1, 1, 0.99));

	auto* closePauseButton = pauseMenu->CreateChild<Button>("Close pause");
	closePauseButton->SetSize(225 *0.75 / screenMult, 225*0.75 / screenMult);
	closePauseButton->SetAlignment(HA_RIGHT, VA_TOP);
	closePauseButton->SetPosition(-300 / screenMult, 50 / screenMult);
	closePauseButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonExit.png"));
	SubscribeToEvent(closePauseButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* RestartButton = pauseMenu->CreateChild<Button>("Restart level");
	RestartButton->SetSize(225 / screenMult, 225 / screenMult);
	RestartButton->SetAlignment(HA_CENTER, VA_CENTER);
	RestartButton->SetPosition(-300 / screenMult, -200 / screenMult);
	RestartButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonRepeatLevel.png"));
	SubscribeToEvent(RestartButton, E_RELEASED, URHO3D_HANDLER(UIManager, RestartLevel));

	auto* mainMenuButton = pauseMenu->CreateChild<Button>("MainMenu button");
	mainMenuButton->SetSize(225 / screenMult, 225 / screenMult);
	mainMenuButton->SetAlignment(HA_CENTER, VA_CENTER);
	mainMenuButton->SetPosition(-300 / screenMult, 200 / screenMult);
	mainMenuButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonMenu.png"));
	SubscribeToEvent(mainMenuButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* pauseAudioButton = pauseMenu->CreateChild<Button>("Pause toggle audio");
	pauseAudioButton->SetSize(225 / screenMult, 225 / screenMult);
	pauseAudioButton->SetAlignment(HA_CENTER, VA_CENTER);
	pauseAudioButton->SetPosition(250 / screenMult, 200 / screenMult);
	pauseAudioButton->SetTexture(cache->GetResource<Texture2D>(muteIcon));
	SubscribeToEvent(pauseAudioButton, E_RELEASED, URHO3D_HANDLER(UIManager, ToggleAudio));

	auto* pauseBGButton = pauseMenu->CreateChild<Button>("Pause toggle background");
	pauseBGButton->SetSize(225 / screenMult, 225 / screenMult);
	pauseBGButton->SetAlignment(HA_CENTER, VA_CENTER);
	pauseBGButton->SetPosition(250 / screenMult, -200 / screenMult);
	pauseBGButton->SetTexture(cache->GetResource<Texture2D>(bgIcon));
	SubscribeToEvent(pauseBGButton, E_RELEASED, URHO3D_HANDLER(UIManager, ToggleBackground));
#pragma endregion
	// |------------| PAUSE MENU END |------------|



	// |------------| LEVEL WIN |------------|
#pragma region LEVELWIN

	levelWin_ = root_->CreateChild<Window>();
	levelWin_->SetSize(screenSize.x_, screenSize.y_);
	levelWin_->SetPosition(0, 0);
	levelWin_->SetColor(Color(1, 1, 1, 0));

	auto* levelWin = levelWin_->CreateChild<UIElement>();
	levelWin->SetSize(screenSize.x_, screenSize.y_);

	auto* winBG = levelWin->CreateChild<Sprite>();
	winBG->SetStyleAuto();
	winBG->SetSize(screenSize.x_, screenSize.y_);
	winBG->SetPosition(0, 0);
	winBG->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_MenuBlackBG.png"));
	winBG->SetColor(Color(1, 1, 1, 0.99));

	auto* winRestartButton = levelWin->CreateChild<Button>("Restart level");
	winRestartButton->SetSize(307 * 0.75 / screenMult, 307 * 0.75 / screenMult);
	winRestartButton->SetAlignment(HA_CENTER, VA_CENTER);
	winRestartButton->SetPosition(-300 / screenMult, 200 / screenMult);
	winRestartButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonRepeatLevel.png"));
	SubscribeToEvent(winRestartButton, E_RELEASED, URHO3D_HANDLER(UIManager, RestartLevel));

	auto* nextLevelButton = levelWin->CreateChild<Button>("Next level");
	nextLevelButton->SetSize(307 * 0.75 / screenMult, 307 * 0.75 / screenMult);
	nextLevelButton->SetAlignment(HA_CENTER, VA_CENTER);
	nextLevelButton->SetPosition(300 / screenMult, 200 / screenMult);
	nextLevelButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonNextLevel.png"));
	SubscribeToEvent(nextLevelButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* winReturnButton = levelWin->CreateChild<Button>("MainMenu button");
	winReturnButton->SetSize(244 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	winReturnButton->SetAlignment(HA_RIGHT, VA_TOP);
	winReturnButton->SetPosition(-300 / screenMult, 50 / screenMult);
	winReturnButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonExit.png"));
	SubscribeToEvent(winReturnButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* winText = levelWin->CreateChild<Sprite>();
	winText->SetSize(512 * 1.5 / screenMult, 128 *1.5 / screenMult);
	winText->SetAlignment(HA_CENTER, VA_TOP);
	winText->SetPosition(-winText->GetWidth() / 2, 200 / screenMult);
	winText->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_PillagedText.png"));
#pragma endregion
	// |------------| LEVEL WIN END |------------|



	// |------------| LEVEL LOSE |------------|
#pragma region LEVELLOSE

	levelLose_ = root_->CreateChild<Window>();
	levelLose_->SetSize(screenSize.x_, screenSize.y_);
	levelLose_->SetPosition(0, 0);
	levelLose_->SetColor(Color(1, 1, 1, 0));

	auto* levelLose = levelLose_->CreateChild<UIElement>();
	levelLose->SetSize(screenSize.x_, screenSize.y_);

	auto* loseBG = levelLose->CreateChild<Sprite>();
	loseBG->SetStyleAuto();
	loseBG->SetSize(screenSize.x_, screenSize.y_);
	loseBG->SetPosition(0, 0);
	loseBG->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_MenuBlackBG.png"));
	loseBG->SetColor(Color(1, 1, 1, 0.99));

	auto* loseRestartButton = levelLose->CreateChild<Button>("Restart level");
	loseRestartButton->SetSize(307 * 0.75 / screenMult, 307 * 0.75 / screenMult);
	loseRestartButton->SetAlignment(HA_CENTER, VA_CENTER);
	loseRestartButton->SetPosition(0, 200 / screenMult);
	loseRestartButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonRepeatLevel.png"));
	SubscribeToEvent(loseRestartButton, E_RELEASED, URHO3D_HANDLER(UIManager, RestartLevel));

	auto* loseReturnButton = levelLose->CreateChild<Button>("MainMenu button");
	loseReturnButton->SetSize(244 * 0.75 / screenMult, 245 * 0.75 / screenMult);
	loseReturnButton->SetAlignment(HA_RIGHT, VA_TOP);
	loseReturnButton->SetPosition(-300 / screenMult, 50 / screenMult);
	loseReturnButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonExit.png"));
	SubscribeToEvent(loseReturnButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* loseText = levelLose->CreateChild<Sprite>();
	loseText->SetSize(512 *1.5 / screenMult, 128 * 1.5 / screenMult);
	loseText->SetAlignment(HA_CENTER, VA_TOP);
	loseText->SetPosition(-loseText->GetWidth()/2, 200 / screenMult);
	loseText->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_BotchedText.png"));
#pragma endregion
	// |------------| LEVEL LOSE END |------------|



	// |------------| INGAME INTERFACE |------------|
#pragma region INGAMEINTERFACE



	gameUI_ = root_->CreateChild<Window>();
	gameUI_->SetSize(screenSize.x_, screenSize.y_);
	gameUI_->SetPosition(0, 0);
	gameUI_->SetColor(Color(1, 1, 1, 0));

	auto* gameUI = gameUI_->CreateChild<UIElement>();
	gameUI->SetSize(screenSize.x_, screenSize.y_);

	auto* pauseButton = gameUI->CreateChild<Button>("Pause button");
	pauseButton->SetSize(295 * 0.50 / screenMult, 295 * 0.50 / screenMult);
	pauseButton->SetAlignment(HA_LEFT, VA_TOP);
	pauseButton->SetPosition(5, 5);
	pauseButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonPauseLogo.png"));
	SubscribeToEvent(pauseButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

	auto* heavyButton = gameUI->CreateChild<Button>("Heavy button");
	heavyButton->SetSize(175 / screenMult, 175 / screenMult);
	heavyButton->SetAlignment(HA_RIGHT, VA_BOTTOM);
	heavyButton->SetPosition(-350 / screenMult, 0);
	heavyButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonHeavyViking.png"));

	auto* rogueButton = gameUI->CreateChild<Button>("Rogue button");
	rogueButton->SetSize(175 / screenMult, 175 / screenMult);
	rogueButton->SetAlignment(HA_RIGHT, VA_BOTTOM);
	rogueButton->SetPosition(-175 / screenMult, 0);
	rogueButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonRogue.png"));

	auto* torcherButton = gameUI->CreateChild<Button>("Torcher button");
	torcherButton->SetSize(175 / screenMult, 175 / screenMult);
	torcherButton->SetAlignment(HA_RIGHT, VA_BOTTOM);
	torcherButton->SetPosition(0 / screenMult, 0);
	torcherButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonTorcher.png"));

	auto* heavyNumber = heavyButton->CreateChild<Text>("Heavy number");
	heavyNumber->SetText("5");
	heavyNumber->BringToFront();
	heavyNumber->SetTextAlignment(HA_CENTER);
	heavyNumber->SetAlignment(HA_RIGHT, VA_BOTTOM);
	heavyNumber->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 50 / screenMult);

	auto* rogueNumber = rogueButton->CreateChild<Text>("Rogue number");
	rogueNumber->SetText("5");
	rogueNumber->BringToFront();
	rogueNumber->SetTextAlignment(HA_CENTER);
	rogueNumber->SetAlignment(HA_RIGHT, VA_BOTTOM);
	rogueNumber->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 50 / screenMult);

	auto* torcherNumber = torcherButton->CreateChild<Text>("Torcher number");
	torcherNumber->SetText("5");
	torcherNumber->BringToFront();
	torcherNumber->SetTextAlignment(HA_CENTER);
	torcherNumber->SetAlignment(HA_RIGHT, VA_BOTTOM);
	torcherNumber->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 50 / screenMult);

	/*auto* shootButton = gameUI->CreateChild<Button>("Shoot button");
	shootButton->SetSize(175 / screenMult, 175 / screenMult);
	shootButton->SetAlignment(HA_LEFT, VA_BOTTOM);
	shootButton->SetPosition(5 / screenMult, 0);
	shootButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonTorcher.png"));
	shootButton->SetColor(Color(1, 0, 0));*/
	
	auto* tick1 = gameUI->CreateChild<Sprite>("tick1");
	tick1->SetSize(256 * 0.5 / screenMult, 256 * 0.5 / screenMult);
	tick1->SetAlignment(HA_RIGHT, VA_TOP);
	tick1->SetPosition(-256 * 0.5 / screenMult, 256 * 0.5 / screenMult);
	tick1->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOff.png"));
	tick1->SetVisible(false);
	auto* tick1Text = tick1->CreateChild<Text>("tick text 1");
	tick1Text->BringToFront();
	tick1Text->SetText("Burn the dinner");
	tick1Text->SetTextAlignment(HA_RIGHT);
	tick1Text->SetAlignment(HA_RIGHT,VA_CENTER);
	tick1Text->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20 / screenMult);
	tick1Text->SetPosition(-256 * 0.5 / screenMult, 0);

	auto* tick2 = gameUI->CreateChild<Sprite>("tick2");
	tick2->SetSize(256 *0.5 / screenMult, 256 * 0.5 / screenMult);
	tick2->SetAlignment(HA_RIGHT, VA_TOP);
	tick2->SetPosition(-256 * 0.5 / screenMult, (256 * 0.5 / screenMult )*2);
	tick2->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOff.png"));
	tick2->SetVisible(false);
	auto* tick2Text = tick2->CreateChild<Text>("tick text 2");
	tick2Text->BringToFront();
	tick2Text->SetText("Water the dinner");
	tick2Text->SetTextAlignment(HA_RIGHT);
	tick2Text->SetAlignment(HA_RIGHT, VA_CENTER);
	tick2Text->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20 / screenMult);
	tick2Text->SetPosition(-256 * 0.5 / screenMult, 0);


	auto* tick3 = gameUI->CreateChild<Sprite>("tick3");
	tick3->SetSize(256 * 0.5 / screenMult, 256 * 0.5 / screenMult);
	tick3->SetAlignment(HA_RIGHT, VA_TOP);
	tick3->SetPosition(-256 * 0.5 / screenMult, (256 * 0.5 / screenMult )*3);
	tick3->SetTexture(cache->GetResource<Texture2D>("Urho2D/Levels/Level_011/Objects/Spr_TickBoxOff.png"));
	tick3->SetVisible(false);
	auto* tick3Text = tick3->CreateChild<Text>("tick text 3");
	tick3Text->BringToFront();
	tick3Text->SetText("Smash the dinner");
	tick3Text->SetTextAlignment(HA_RIGHT);
	tick3Text->SetAlignment(HA_RIGHT, VA_CENTER);
	tick3Text->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20 / screenMult);
	tick3Text->SetPosition(-256 * 0.5 / screenMult, 0);


	powerSlider_ = gameUI->CreateChild<Sprite>("powerSlider");
	powerSlider_->SetSize(600/screenMult,100/screenMult);
	powerSlider_->SetAlignment(HA_LEFT, VA_TOP);
	powerSlider_->SetPosition(200/screenMult,30/screenMult);
	powerSlider_->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_SliderBG.png"));
	
	

	/*powerSlider_->SetRange(10);
	powerSlider_->SetValue(0);*/
	/*auto* powerKnob = powerSlider_->GetKnob();
	powerKnob->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_SliderButton.png"));*/
#pragma endregion
	// |------------| INGAME INTERFACE END |------------|

#pragma region SPLASHSCREEN


	splash_ = root_->CreateChild<Window>();
	splash_->SetSize(screenSize.x_, screenSize.y_);
	splash_->SetPosition(0, 0);
	splash_->SetColor(Color(1, 1, 1, 0));

	auto* splashScreen = splash_->CreateChild<UIElement>();
	splashScreen->SetSize(screenSize.x_, screenSize.y_);

	auto* ss = splashScreen->CreateChild<Sprite>();
	ss->SetStyleAuto();
	ss->SetSize(screenSize.x_, screenSize.y_);
	ss->SetPosition(0, 0);
	ss->SetTexture(cache->GetResource<Texture2D>("Urho2D/Vikings_SplashScreenBG.png"));

	auto* sLogo = splashScreen->CreateChild<Sprite>();
	sLogo->SetStyleAuto();
	sLogo->SetSize(screenSize.x_, screenSize.y_);
	sLogo->SetPosition(0, 0);
	sLogo->SetTexture(cache->GetResource<Texture2D>("Urho2D/valhallacannonlogo.png"));

	auto* splashButton = ss->CreateChild<Button>("Splash button");
	splashButton->SetSize(200 * 0.75 / screenMult, 200 * 0.75 / screenMult);
	splashButton->SetAlignment(HA_RIGHT, VA_BOTTOM);
	splashButton->SetPosition(-50 / screenMult, -50 / screenMult);
	splashButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonNextLevel.png"));
	SubscribeToEvent(splashButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));



#pragma endregion

#pragma region STARTCOMIC 

	startComic_ = root_->CreateChild<Window>();
	startComic_->SetSize(screenSize.x_, screenSize.y_);
	startComic_->SetPosition(0, 0);
	startComic_->SetColor(Color(1, 1, 1, 0));

	auto* startComicScreen = startComic_->CreateChild<UIElement>();
	startComicScreen->SetSize(screenSize.x_, screenSize.y_);

	auto* startComic = startComicScreen->CreateChild<Sprite>();
	startComic->SetStyleAuto();
	startComic->SetSize(screenSize.x_, screenSize.y_);
	startComic->SetPosition(0, 0);
	startComic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Viikingi_sarjakuva_osa_1.png"));

	auto* comicButton = startComic->CreateChild<Button>("Comic button");
	comicButton->SetSize(200 * 0.75 / screenMult, 200 * 0.75 / screenMult);
	comicButton->SetAlignment(HA_RIGHT, VA_BOTTOM);
	comicButton->SetPosition(-50 / screenMult, -50 / screenMult);
	comicButton->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_ButtonNextLevel.png"));
	SubscribeToEvent(comicButton, E_RELEASED, URHO3D_HANDLER(UIManager, OpenOne));

#pragma endregion

#pragma region COMIC

	comic_ = root_->CreateChild<Window>();
	comic_->SetSize(screenSize.x_, screenSize.y_);
	comic_->SetPosition(0, 0);
	comic_->SetColor(Color(1, 1, 1, 0));

	auto* comicHolder = comic_->CreateChild<UIElement>("Holder");
	comicHolder->SetStyleAuto();
	comicHolder->SetSize(screenSize.x_, screenSize.y_);
	comicHolder->SetPosition(0, 0);
	auto comicBG = comic_->CreateChild<Sprite>("Comic");
	comicBG->SetStyleAuto();
	comicBG->SetSize(1920 / screenMult, 1371 / screenMult);
	comicBG->SetPosition(0, 0);
	comicBG->SetTexture(cache->GetResource<Texture2D>("Urho2D/Spr_MenuBlackBG.png"));

	comic = comic_->CreateChild<Sprite>("Comic");
	comic->SetStyleAuto();
	comic->SetSize(screenSize.x_, screenSize.y_);
	comic->SetPosition(0, 0);
	comic->BringToFront();
	comic_->SetVisible(false);

#pragma endregion
}
void UIManager::FPS()
{
	float timeStep = GetSubsystem<Time>()->GetTimeStep();
	framecount_++;
	time_ += timeStep;


	if (time_ >= 1)
	{
		std::string str;
		{
			std::ostringstream ss;
			ss << framecount_;
			std::string s(ss.str());
			str.append(s.substr(0, 6));
		}
		str.append(" frames in ");
		{
			std::ostringstream ss;
			ss << time_;
			std::string s(ss.str());
			str.append(s.substr(0, 6));
		}
		str.append(" seconds = ");
		{
			std::ostringstream ss;
			ss << (float)framecount_ / time_;
			std::string s(ss.str());
			str.append(s.substr(0, 6));
		}
		str.append(" fps");
		String s(str.c_str(), str.size());
		text_->SetText(s);
		//URHO3D_LOGINFO(s);     // this show how to put stuff into the log
		framecount_ = 0;
		time_ = 0;
	}
	
}
void UIManager::ButtonHandler()
{
	String buttonPressed = lastEvent;
	if (buttonPressed == "Start button")
	{
		HideAll();
		GetOne(3);
		lastEvent = "Level Selector Open";
	}
	if (buttonPressed == "Options button")
	{
		HideAll();
		GetOne(2);
		lastEvent = "Options Menu Open";
	}
	if (buttonPressed == "Quit button")
	{
		GetSubsystem<Engine>()->Exit();
		lastEvent = "Quitted";
	}
	if (buttonPressed == "Close settings")
	{
		HideAll();
		GetOne(1);
		lastEvent = "Closed Settings";
	}
	if (buttonPressed == "Pause button")
	{
		HideAll();
		GetOne(4);
		lastEvent = "Pause Menu Open";
		MyTools::currentScene->SetUpdateEnabled(false);
	}
	if (buttonPressed == "MainMenu button")
	{
		HideAll();
		GetOne(1);
		lastEvent = "Return to Main";
	}
	if (buttonPressed == "Close pause")
	{
		HideAll();
		GetOne(7);
		lastEvent = "Pause Menu Closed";
		MyTools::currentScene->SetUpdateEnabled(true);
	}
	if (buttonPressed == "Next level")
	{
		Level::currentLevel->Unload();
		HideAll();


		if (Level::levelNumber < 11)
		{
			GetOne(7);
			Level::NewLevel(Level::levelNumber+1);
			comicHandler(true, Level::levelNumber);

			Level::currentLevel->Init();
			lastEvent = "Chose next level";
		}
		else
		{
			lastEvent = "Options button";
		}
	}
	if (buttonPressed == "Splash button")
	{
		HideAll();
		GetOne(9);
		lastEvent = "Story time";
	}
	if (buttonPressed == "Comic button")
	{
		HideAll();
		GetOne(1);
		lastEvent = "Return to Main";
	}
	// --- LEVELS ---
	for (size_t i = 0; i < 12; i++)
	{
		if (buttonPressed == "Level " + String(i))
		{
			HideAll();
			GetOne(7);
			Level::NewLevel(i);
			comicHandler(true,i);
			Level::currentLevel->Init();
			lastEvent = "Chose Level " + String(i);
		}
	}
}
void UIManager::Update()
{

	screenSize = Vector2(GetSubsystem<Graphics>()->GetWidth(), GetSubsystem<Graphics>()->GetHeight());
	screenMult = 1080 / screenSize.y_;

	if (screenMult != currentSize)
	{
		UIReset();
		if (whatUI == 7)
		{
			Cannon::instance->SetupButtons();
		}
	}
	//FPS();
	ButtonHandler();
	if (powerSlider_ != NULL)
	{
		//sliderValue = 1;
		powerSlider_->SetSize(600 * sliderValue / screenMult, 100/screenMult);
		Color currentColor = Lerp(Color(0, 1, 0), Color(1, 0, 0), sliderValue);
		powerSlider_->SetColor(currentColor);
		//URHO3D_LOGINFO(String(powerSlider_->GetPosition()));
	}
	if (Level::levelNumber != 11)
	{
		gameUI_->GetChild("tick1", true)->SetVisible(false);
		gameUI_->GetChild("tick2", true)->SetVisible(false);
		gameUI_->GetChild("tick3", true)->SetVisible(false);
	}
}
void UIManager::UIReset()
{
	whoosh->RemoveAllChildren();
	CreateMenus();
	FPS();
	HideAll();
	GetOne(whatUI);

}
/// Original Creator: Karhu
void UIManager::HideAll() // Hides all the UI/Menu stuff
{
	mainMenu_->SetVisible(false);
	settingsMenu_->SetVisible(false);
	levelSelector_->SetVisible(false);
	pauseMenu_->SetVisible(false);
	levelWin_->SetVisible(false);
	levelLose_->SetVisible(false);
	gameUI_->SetVisible(false);
	splash_->SetVisible(false);
	startComic_->SetVisible(false);
	
}

/// Original Creator: Karhu
void UIManager::OpenOne(StringHash eventType, VariantMap& eventData)	 // Opens a Menu/UI using an id or something, dont know yet.
{
	auto* button = static_cast<Button*>(GetEventSender());
	if (button)
	{
		URHO3D_LOGINFO(button->GetName());
		lastEvent = button->GetName();
	}
}

void UIManager::GetOne(int i) // 1 = main, 2 = settings, 3 = levelselect, 4 = pausemenu, 5 = levelWin, 6 = levelLose, 7 = Activate UI
{
	if (i == 1)
	{
		if (Level::currentLevel)
		{
			Level::currentLevel->Unload();
		}
		comic_->SetVisible(false);
		mainMenu_->SetVisible(true);
		whatUI = 1;
	}
	if (i == 2)
	{
		settingsMenu_->SetVisible(true);
		whatUI = 2;
	}
	if (i == 3)
	{
		levelSelector_->SetVisible(true);
		whatUI = 3;
	}
	if (i == 4)
	{
		pauseMenu_->SetVisible(true);
		whatUI = 4;
	}
	if (i == 5)
	{
		levelWin_->SetVisible(true);
		whatUI = 5;
		if (Level::levelNumber >= unlockedLevel)
		{
			unlockedLevel = Level::levelNumber+ 1;
			MyTools::SetValue("unlockedLevel", String(unlockedLevel));
			MyTools::SaveValues();
			UIReset();
		}
	}
	if (i == 6)
	{
		levelLose_->SetVisible(true);
		whatUI = 6;
	}
	if (i == 7)
	{
		gameUI_->SetVisible(true);
		whatUI = 7;
		
	}
	if (i == 8) 
	{
		splash_->SetVisible(true);
		whatUI = 8;
	}
	if (i == 9)
	{
		startComic_->SetVisible(true);
		whatUI = 9;
	}
}

void UIManager::GoToLevel(StringHash eventType, VariantMap& eventData)
{
	auto* button = static_cast<Button*>(GetEventSender());
	if (button)
	{
		URHO3D_LOGINFO(button->GetName());
		lastEvent = button->GetName();
	}
}

void UIManager::ChooseLevel(int i)
{
	// Do thing
}

void UIManager::RestartLevel(StringHash eventType, VariantMap& eventData)
{
	Level::currentLevel->Unload();
	comic_->SetVisible(false);
	Level::currentLevel->Init();
	HideAll();
	GetOne(7);
}

void UIManager::ToggleAudio(StringHash eventType, VariantMap& eventData)
{
	if (audioOn == true) 
	{
		muteIcon = "Urho2D/Spr_ButtonMuteOn.png";
		audioOn = false;

		UIReset();
		
		if (whatUI == 4)
		{
			Cannon::instance->SetupButtons();
		}
		MyAudio->SetMasterGain("Effect",0);
	}
	else 
	{
		muteIcon = "Urho2D/Spr_ButtonMuteOff.png";
		audioOn = true;

		UIReset();

		if (whatUI == 4)
		{
			Cannon::instance->SetupButtons();
		}
		MyAudio->SetMasterGain("Effect", 1);
	}
}

void UIManager::ToggleBackground(StringHash eventType, VariantMap& eventData)
{
	if (bgOn == true)
	{	
		if (Level::currentLevel != NULL && Level::currentLevel->background != NULL) 
		{
			Level::currentLevel->background->SetEnabled(false);
			if (Level::currentLevel->foreground != NULL) 
			{
				Level::currentLevel->foreground->SetEnabled(false);
			}
		}
		bgIcon = "Urho2D/Spr_ButtonBgToggleOn.png";
		bgOn = false;

		UIReset();

		if (whatUI == 4)
		{
			Cannon::instance->SetupButtons();
		}
	}
	else
	{
		if (Level::currentLevel != NULL && Level::currentLevel->background != NULL)
		{
			Level::currentLevel->background->SetEnabled(true);
			if (Level::currentLevel->foreground != NULL)
			{
				Level::currentLevel->foreground->SetEnabled(true);
			}
		}
		bgIcon = "Urho2D/Spr_ButtonBgToggle.png";
		bgOn = true;

		UIReset();

		if (whatUI == 4)
		{
			Cannon::instance->SetupButtons();
		}
	}
}

void UIManager::comicHandler(bool prelevel,int i) 
{
	auto* cache = GetSubsystem<ResourceCache>();
	//auto* comic = static_cast<Sprite*>(comic_->GetChild("Holder", true)->GetChild("Comic", true));
	
	switch (i)
	{
	case 1:
		if (prelevel == true) 
		{
			MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
			comic->BringToFront();
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level1/prelevel1_frame1.png")); }, 1);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level1/prelevel1_frame2.png")); }, 3);
			MyTimer::New([=]() {comic_->SetVisible(false); }, 5);	

		}
		if (prelevel == false)
		{
			MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
			comic->BringToFront();
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level1/level1_frame1.png")); }, 1);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level1/level1_frame2.png")); }, 3);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level1/level1_frame3.png")); }, 5);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level1/level1_frame4.png")); }, 7);
			MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		}
		break;
	case 2:
			MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level2/level2_frame1.png")); }, 1);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level2/level2_frame2.png")); }, 3);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level2/level2_frame3.png")); }, 5);
			MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level2/level2_frame4.png")); }, 7);
			MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		break;
	case 3:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level3/level3_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level3/level3_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level3/level3_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level3/level3_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		break;
	case 4:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level4/level4_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level4/level4_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level4/level4_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level4/level4_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		break;
	case 5:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level5/level5_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level5/level5_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level5/level5_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level5/level5_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		break;
	case 6:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level6/level6_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level6/level6_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level6/level6_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level6/level6_frame4.png")); }, 7);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level6/level6_frame5.png")); }, 9);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 11);
		break;
	case 7:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level7/level7_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level7/level7_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level7/level7_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level7/level7_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		break;
	case 8:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level8/level8_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level8/level8_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level8/level8_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level8/level8_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);		
		break;
	case 9:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level9/level9_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level9/level9_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level9/level9_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level9/level9_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);	
		break;
	case 10:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level10/level10_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level10/level10_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level10/level10_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level10/level10_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);	
		break;
	case 11:
		MyTimer::New([=]() {comic_->SetVisible(true); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level11/level11_frame1.png")); }, 1);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level11/level11_frame2.png")); }, 3);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level11/level11_frame3.png")); }, 5);
		MyTimer::New([=]() {comic->SetTexture(cache->GetResource<Texture2D>("Urho2D/Cutscenes/level11/level11_frame4.png")); }, 7);
		MyTimer::New([=]() {comic_->SetVisible(false); }, 9);
		break;
	}
}