#include "MyTools.h"
#include "Cannon.h"
#include "Interactable.h"
#include <iostream>
#include <string>
//the holy sea of pointers
MyTools* MyTools::instance = 0;
Scene* MyTools::currentScene = 0;
Vector<String> MyTools::keys;
Vector<String> MyTools::values;

Input* MyInput = 0;
Time* MyTime = 0;
WorkQueue* MyWorkQueue = 0;
FileSystem* MyFileSystem = 0;
Log* MyLog = 0;
ResourceCache* MyCache = 0;
UI* MyUI = 0;
Audio* MyAudio = 0;
Engine* MyEngine = 0;
Camera* MyCamera = 0;
Graphics* MyGraphics = 0;
Renderer* MyRenderer = 0;
Node* selectedNode = 0;

MyTools::MyTools(Context* context) : LogicComponent(context)
{
	instance = this;
	MyInput = GetSubsystem<Input>();
	MyTime = GetSubsystem<Time>();
	MyWorkQueue = GetSubsystem<WorkQueue>();
	MyFileSystem = GetSubsystem<FileSystem>();
	MyLog = GetSubsystem<Log>();
	MyCache = GetSubsystem<ResourceCache>();
	MyUI = GetSubsystem<UI>();
	MyAudio = GetSubsystem<Audio>();
	MyEngine = GetSubsystem<Engine>();
	MyGraphics = GetSubsystem<Graphics>();
	MyRenderer = GetSubsystem<Renderer>();
}


MyTools::~MyTools()
{
}
void MyTools::OnSceneSet(Scene* scene) {
	if (scene == NULL)//cause apparently this gets called with null when the program closes :D	
	{
		return;
	}
	MyCamera = scene->GetComponent<Camera>(true);
	currentScene = scene;
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyTools, HandleUpdate));
}

Ray MyTools::ScreenToRay(IntVector2 point) {
	IntVector2 resolution(MyGraphics->GetWidth(), MyGraphics->GetHeight());
	Vector2 viewPort((float)point.x_ / resolution.x_, (float)point.y_ / resolution.y_);
	Ray screenRay = MyCamera->GetScreenRay(viewPort.x_, viewPort.y_);
	return screenRay;
}
Ray MyTools::TouchToRay() {
	//store last touch position and have it as optional bool?
	IntVector2 mouse = IntVector2::ZERO;
	if (MyInput->GetNumTouches() > 0)
	{
		mouse = MyInput->GetTouch(0)->position_;
	}
	else
	{
		mouse = MyInput->GetMousePosition();//mostly for level editor to work
	}
	return ScreenToRay(mouse);
}
Vector2 MyTools::ScreenToWorld(IntVector2 point) {//RETURNS SHITE AT FIRST FRAME, resizing from defalt aspect ratio happens after first frame :/
	Ray screenRay = ScreenToRay(point);
	return Vector2(screenRay.origin_.x_, screenRay.origin_.y_);
}
Vector2 MyTools::ScreenToWorld(int x, int y) {
	return ScreenToWorld(IntVector2(x, y));
}
Vector2 MyTools::ViewportToWorld(Vector2 point) {

	IntVector2 resolution = GetResolution();
	IntVector2 screenCoord = IntVector2(RoundToInt(point.x_*resolution.x_), RoundToInt(point.y_*resolution.y_));
	return ScreenToWorld(screenCoord);
}
Vector2 MyTools::ViewportToWorld(float x, float y) {
	return ViewportToWorld(Vector2(x, y));
}
Vector2 MyTools::TouchToWorld() {
	Node* ins = instance->GetNode();
	Input* input = ins->GetSubsystem<Input>();
	IntVector2 mouseVec = IntVector2::ZERO;
	if (MyInput->GetNumTouches() > 0)
	{
		mouseVec = MyInput->GetTouch(0)->position_;
	}
	else
	{
		mouseVec = MyInput->GetMousePosition();//mostly for level editor to work
	}
	//URHO3D_LOGINFO("touch at " + String(mouseVec));
	//URHO3D_LOGINFO("screen at " + String(ScreenToWorld(mouseVec)));
	return ScreenToWorld(mouseVec);
}

//screenhits func necessary?
PODVector<RigidBody2D*> MyTools::MouseHits() {
	Node* ins = instance->GetNode();
	PhysicsRaycastResult2D hit2;
	Vector2 screen = TouchToWorld();
	ins->GetComponent<PhysicsWorld2D>()->RaycastSingle(hit2, screen, screen + Vector2::UP);

	Vector2 rectExtents = Vector2::ONE / 2;
	Rect rect = Rect(screen - rectExtents, screen + rectExtents);
	PODVector<RigidBody2D*> collidingBodies;
	ins->GetComponent<PhysicsWorld2D>()->GetRigidBodies(collidingBodies, rect);
	return collidingBodies;
}
Vector3 MyTools::ClampMagnitude(Vector3 vec, float maxMagnitude) {
	if (vec.Length() <= maxMagnitude)
	{
		return vec;
	}
	return vec.Normalized()*maxMagnitude;
}
Vector2 MyTools::ClampMagnitude(Vector2 vec, float maxMagnitude) {
	if (vec.Length() <= maxMagnitude)
	{
		return vec;
	}
	return vec.Normalized()*maxMagnitude;
}
float MyTools::ViewportToWorldDistance(float x, float y) {
	Vector2 start = ViewportToWorld(0.0, 0.0);
	Vector2 end = ViewportToWorld(x, y);
	float distance = (end - start).Length();
	return distance;
}
float MyTools::ViewportToWorldDistance(Vector2 vec) {
	return ViewportToWorldDistance(vec.x_, vec.y_);
}

Vector2 MyTools::ToVector2(Vector3 vec) {
	Vector2 vec2 = Vector2(vec.x_, vec.y_);
	return vec2;
}
IntVector2 MyTools::GetResolution() {
	return  IntVector2(instance->GetSubsystem<Graphics>()->GetWidth(), instance->GetSubsystem<Graphics>()->GetHeight());
}
Vector2 MyTools::ScreenToViewport(IntVector2 point) {
	IntVector2 resolution = GetResolution();
	return Vector2((float)point.x_ / resolution.x_, (float)point.y_ / resolution.y_);
}
void MyTools::SetEditMode(bool enabled) {
	editMode = enabled;
	if (enabled)
	{
		SubscribeToEvent(E_TOUCHBEGIN, URHO3D_HANDLER(MyTools, HandleTouchBegin));
		SubscribeToEvent(E_TOUCHEND, URHO3D_HANDLER(MyTools, HandleTouchEnd));
		return;
	}
	UnsubscribeFromEvent(E_TOUCHBEGIN);
	UnsubscribeFromEvent(E_TOUCHEND);
}
void MyTools::HandleUpdate(StringHash eventType, VariantMap& eventData) {
	//MyTimer::EvaluateAll();
	if (MyInput->GetKeyPress(EditKeys::toggleDebugColliders))
	{
		drawDebug = !drawDebug;
	}
	if (drawDebug)
	{
		currentScene->GetComponent<PhysicsWorld2D>()->DrawDebugGeometry(MyTools::currentScene->GetComponent<DebugRenderer>(), false);
	}
	if (MyInput->GetKeyPress(EditKeys::resetValues))
	{
		//New(PrintValues, 5.0);
		/*MyTimer::New(PrintValues, 5.0);
		MyTimer::New(test), 5.0);*/
		//New(PrintValues, 5.0);
		/*URHO3D_LOGINFO("deleting values");
		PrintValues();
		ClearValues();
		PrintValues();
		SaveValues();*/
	}
	if (MyInput->GetKeyPress(EditKeys::saveScene))
	{
		//scene
		File saveScene(context_, MyFileSystem->GetProgramDir() + "Data/Scenes/sceneSave.xml", FILE_WRITE);
		MyTools::currentScene->SaveXML(saveScene);
		//UI
		File saveUI(context_, MyFileSystem->GetProgramDir() + "Data/UI/UIsave.xml", FILE_WRITE);
		MyUI->SaveLayout(saveUI, MyUI->GetRoot());
	}
	//use at least for key input checking for toggles & such?
	//e to toggle edit mode
	if (MyInput->GetKeyPress(EditKeys::toggleEdit))
	{
		SetEditMode(!editMode);
	}
	if (editMode == false || selectedNode == NULL)
	{
		return;
	}
	Vector2 touchPos = TouchToWorld();
	if (MyInput->GetNumTouches())
	{
		selectedNode->SetWorldPosition((Vector3)TouchToWorld() + touchOffset);
		//Rect rect = selectedNode->GetComponent<StaticSprite2D>()->GetDrawRect();
		//DrawRect(ToVector2(selectedNode->GetWorldPosition())+rect.Min()*3, ToVector2(selectedNode->GetWorldPosition()) + rect.Max()*3);

		if (MyInput->GetKeyPress(EditKeys::selectParent))
		{
			Node* parentNode = selectedNode->GetParent();
			if (parentNode && parentNode != currentScene->GetNode(0))//is node 0 scene node?
			{
				//set new offsets etc
				selectedNode->SetPosition(parentOffset);
				selectedNode = parentNode;
				parentOffset = parentNode->GetPosition();
				touchOffset = parentNode->GetWorldPosition() - (Vector3)TouchToWorld();
				originalScale = parentNode->GetScale();
			}
		}
	}
	else if (MyInput->GetKeyPress(EditKeys::print))
	{
		//round
		size_t decimals = 2;
		Vector2 roundPos = Round(ToVector2(selectedNode->GetWorldPosition()), decimals);
		Vector2 roundLocal = Round(ToVector2(selectedNode->GetPosition()), decimals);
		Vector2 roundScale = Round(selectedNode->GetScale2D(), decimals);
		float roundRot = Round(selectedNode->GetRotation2D(), decimals);

		//print
		String name = selectedNode->GetName();
		String position = " at " + String(roundPos.x_) + ", " + String(roundPos.y_);
		String localPosition = " | (local " + String(roundLocal.x_) + ", " + String(roundLocal.y_) + " )";
		String scale = " scale " + String(roundScale.x_) + ", " + String(roundScale.y_);
		String rotation = " rotation " + String(roundRot);
		String pastable = name + " " + String(roundPos.x_) + ", " + String(roundPos.y_) + ", " + String(roundScale.x_) + ", " + String(roundRot);
		URHO3D_LOGINFO(name + position + localPosition + scale + rotation);
		//URHO3D_LOGINFO("THE 3D SCALE IS " + String(selectedNode->GetScale()));
		//URHO3D_LOGINFO(pastable);

		//component specific code
		StaticSprite2D* sprite = selectedNode->GetComponent<StaticSprite2D>();
		if (!sprite)
		{
			sprite = selectedNode->GetComponent<AnimatedSprite2D>();
		}
		Interactable* interactable = selectedNode->GetComponent<Interactable>();
		RigidBody2D* rigidBody = selectedNode->GetComponent<RigidBody2D>();
		String creation = R"(Create$type("$name", "$sprite", $x, $y, $scale, $rotation);)";
		if (roundRot == 0)
		{
			creation.Replace(", $rotation","");
			if (roundScale.x_ == 1)
			{
				creation.Replace(", $scale","");
			}
		}
		
		creation.Replace("$name", name);
		String spriteName = sprite ? sprite->GetSprite()->GetName() : "";
		spriteName.Replace("Urho2D/", "");
		creation.Replace("$sprite", spriteName);
		creation.Replace("$x", String(roundPos.x_));
		creation.Replace("$y", String(roundPos.y_));
		creation.Replace("$scale", String(roundScale.x_));
		creation.Replace("$rotation", String(roundRot));
		//creation.Replace("sprite",);
		if (interactable)
		{
			//URHO3D_LOGINFO("CreateInteractable(\"" + "\", \"Spr_Tree01.png\", 5.16, -3.44);");
			creation.Replace("$type", "Interactable");
		}
		else if (rigidBody)
		{
			creation.Replace("$type", "Object");
		}
		else if (sprite)
		{
			creation.Replace("$type", "Sprite");
		}
		String pointer = "Node* " + name.ToLower() + " = ";
		URHO3D_LOGINFO(pointer + creation);
		if (sprite)
		{
			String setLayer = sprite ? "SetLayer(" + name.ToLower() + ", " + String(sprite->GetLayer()) + ", " + String(sprite->GetOrderInLayer()) + ");" : "";
			URHO3D_LOGINFO(setLayer);
		}
		MyPrint("");
	}
	else if (MyInput->GetKeyPress(EditKeys::scale))//check for all anchored editkeys on first press here?
	{
		editAnchor = TouchToWorld();
		originalScale = selectedNode->GetScale();
		anchorDistance = (touchPos - ToVector2(selectedNode->GetWorldPosition())).Length();
		//URHO3D_LOGINFO("anchordistance " + String(anchorDistance));
	}
	else if (MyInput->GetKeyDown(EditKeys::scale))
	{
		float currentDistance = (TouchToWorld() - ToVector2(selectedNode->GetWorldPosition())).Length();
		float multiplier = currentDistance / anchorDistance;
		Vector3 newScale = originalScale * multiplier;
		newScale.z_ = originalScale.z_;//avoid scaling on z axis in 2d
		selectedNode->SetScale(newScale);
		//URHO3D_LOGINFO("currentDistance " + String(currentDistance));
	}
	if (MyInput->GetKeyPress(EditKeys::undo))
	{
		//set last moved node to original pos
		selectedNode->SetPosition(parentOffset);
		selectedNode->SetScale(originalScale);
	}
	if (MyInput->GetKeyPress(EditKeys::moveFront))
	{
		MoveLayer(1);
	}
	if (MyInput->GetKeyPress(EditKeys::moveBack))
	{
		MoveLayer(-1);
	}
	/*float mult = 1.1f;
	if (MyInput->GetKeyPress(Key::KEY_UP))
	{
		MyCamera->SetOrthoSize(MyCamera->GetOrthoSize()*mult);
	}
	if (MyInput->GetKeyPress(Key::KEY_DOWN))
	{
		MyCamera->SetOrthoSize(MyCamera->GetOrthoSize() / mult);
	}*/
	if (MyInput->GetKeyPress(EditKeys::rotate))//check for all anchored editkeys on first press here?
	{
		editAnchor = TouchToWorld();
		originalRotation = selectedNode->GetRotation2D();
	}
	else if (MyInput->GetKeyDown(EditKeys::rotate))
	{
		Vector2 newDir = (TouchToWorld() - selectedNode->GetWorldPosition2D()).Normalized();
		/*Vector2 originalDir = (editAnchor - selectedNode->GetWorldPosition2D()).Normalized();
		Vector2 toNew = newDir - originalDir;
		Vector3 newOrtho(newDir.y_,-newDir.x_,0);
		float offset = 0;
		float sign = 1;
		if (toNew.y_ < 0)
		{
			if (toNew.x_ > 0)
			{
				sign = -1;
			}
			else
			{
				offset = 180;
			}
		}
		float dot = originalDir.DotProduct(newDir);
		float angle = Acos(dot);*/
		//would have used my own rotation 

		//selectedNode->SetRotation2D(angle*sign + offset);
		selectedNode->LookAt(selectedNode->GetPosition() + Vector3::FORWARD * 5, (Vector3)newDir);
	}
	
}
void MyTools::HandleTouchBegin(StringHash eventType, VariantMap& eventData) {
	//URHO3D_LOGINFO("touchbegin editor");

	/*PODVector<RayQueryResult> results;
	for (size_t i = 0; i < 4; i++)
	{
		RayOctreeQuery query(results, TouchToRay(), (RayQueryLevel)i, M_INFINITY, DRAWABLE_ANY, M_INFINITY);
		currentScene->GetComponent<Octree>()->RaycastSingle(query);
		if (results.Size())
		{
			URHO3D_LOGINFO(results[0].node_->GetName());
		}
	}
	Vector3 cannonPos = Cannon::instance->GetNode()->GetPosition();
	Vector3 offset = Vector3::BACK;
	Ray hitRay(cannonPos + offset, -offset);
	for (size_t i = 0; i < 4; i++)
	{
		RayOctreeQuery query2(results, hitRay, (RayQueryLevel)i, M_INFINITY, DRAWABLE_ANY, M_INFINITY);
		currentScene->GetComponent<Octree>()->RaycastSingle(query2);
		if (results.Size())
		{
			URHO3D_LOGINFO(results[0].node_->GetName());
		}
	}*/
	Vector2 touchPos = TouchToWorld();
	PODVector<Node*> allNodes = currentScene->GetChildren(true);
	float minDistance = 9999999;
	StaticSprite2D* closestSprite = 0;
	Vector3 closestTouchOffset = Vector3::ZERO;
	Vector3 closestParentOffset = Vector3::ZERO;
	int layer = -99999;
	//URHO3D_LOGINFO("touch at " + String(touchPos));

	for (size_t i = 0; i < allNodes.Size(); i++)
	{
		StaticSprite2D* sprite = allNodes[i]->GetComponent<StaticSprite2D>();
		if (!sprite)
		{
			sprite = allNodes[i]->GetComponent<AnimatedSprite2D>();
		}
		if (sprite)
		{
			BoundingBox bounds = sprite->GetWorldBoundingBox();//ALT TRY DRAWRECT
			//URHO3D_LOGINFO(sprite->GetNode()->GetName() + " min "+ String(bounds.min_) + " max " + String(bounds.max_) );
			if (bounds.min_.x_ < touchPos.x_ && bounds.max_.x_ > touchPos.x_ &&
				bounds.min_.y_ < touchPos.y_ && bounds.max_.y_ > touchPos.y_)//is touch within sprite bounds?
			{
				Vector2 spriteOffset = (ToVector2(bounds.Center()) - touchPos);
				float distance = spriteOffset.Length();
				int newLayer = sprite->GetLayer() * 1000 + sprite->GetOrderInLayer();
				//URHO3D_LOGINFO("sprite " + String(sprite->GetNode()->GetName()) + " at layer " + String(sprite->GetLayer()) +" order " + String(sprite->GetOrderInLayer())+ "");
				//if (distance < minDistance)
				if (newLayer > layer)//fancy stuff would check for opacity from texture or some spaghetti shit but lets just take the top layer
				{
					layer = newLayer;
					closestSprite = sprite;
					closestTouchOffset = sprite->GetNode()->GetWorldPosition() - (Vector3)touchPos;
					minDistance = distance;
					closestParentOffset = sprite->GetNode()->GetPosition();
				}
			}
		}
	}
	if (closestSprite)
	{
		selectedNode = closestSprite->GetNode();
		touchOffset = closestTouchOffset;
		parentOffset = closestParentOffset;//should probably use structs here for all the selected editItem variables
		originalScale = selectedNode->GetScale();
		//URHO3D_LOGINFO(String(closestSprite->GetDrawRect().Min()));
	}
	else
	{
		selectedNode = NULL;
	}
}
void MyTools::MoveLayer(int direction) {
	StaticSprite2D* selectedSprite = selectedNode->GetComponent<StaticSprite2D>();
	if (!selectedSprite)
	{
		selectedSprite = selectedNode->GetComponent<AnimatedSprite2D>();
	}
	int dir = Sign(direction);
	if (selectedNode == NULL || selectedSprite == NULL || dir == 0)
	{
		return;
	}
	int oldLayer = selectedSprite->GetLayer();
	int oldOrder = selectedSprite->GetOrderInLayer();
	int oldCombined = oldLayer * 1000 + oldOrder;
	PODVector<Node*> allNodes = currentScene->GetChildren(true);
	StaticSprite2D* nextSprite = 0;
	const int maxOrder = 1000;
	int nextLayer = 9999 * dir;
	int nextOrder = 9999 * dir;
	int nextCombined = 9999999 * dir;
	//URHO3D_LOGINFO("selected " + String(selectedNode->GetName()) + " at layer " + String(selectedSprite->GetLayer()) + " order " + String(selectedSprite->GetOrderInLayer()) + " combined " +String(oldCombined));

	for (size_t i = 0; i < allNodes.Size(); i++)
	{
		if (allNodes[i] == selectedNode)
		{
			continue;
		}
		StaticSprite2D* sprite = allNodes[i]->GetComponent<StaticSprite2D>();
		if (!sprite)
		{
			sprite = allNodes[i]->GetComponent<AnimatedSprite2D>();
		}
		if (sprite)
		{
			BoundingBox bounds = sprite->GetWorldBoundingBox();//ALT TRY DRAWRECT
			if (selectedSprite->GetWorldBoundingBox().IsInside(bounds))
			{
				//URHO3D_LOGINFO(sprite->GetNode()->GetName() + " is inside " + selectedNode->GetName());
				int newLayer = sprite->GetLayer();
				int newOrder = sprite->GetOrderInLayer();
				int newCombined = newLayer * maxOrder + newOrder;
				//URHO3D_LOGINFO("overlapped " + String(sprite->GetNode()->GetName()) + " at layer " + String(newLayer) + " order " + String(newOrder) + " combined " + String(newCombined));
				if (dir > 0)
				{
					if (newCombined <= nextCombined && newCombined >= oldCombined)
					{
						nextSprite = sprite;
						nextCombined = newCombined;
					}
				}
				else
				{
					if (newCombined >= nextCombined && newCombined <= oldCombined)
					{
						nextSprite = sprite;
						nextCombined = newCombined;
					}
				}
			}
		}
	}
	if (nextSprite)
	{
		selectedSprite->SetLayer(nextSprite->GetLayer());
		selectedSprite->SetOrderInLayer(nextSprite->GetOrderInLayer() + dir);
		//URHO3D_LOGINFO("new layer " + String(nextSprite->GetLayer()) + " order " + String(nextSprite->GetOrderInLayer() + dir));
	}
}
void MyTools::HandleTouchEnd(StringHash eventType, VariantMap& eventData) {
	//URHO3D_LOGINFO("touchend editor");
}
void MyTools::DrawRect(Vector2 min, Vector2 max) {
	MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine((Vector3)min, (Vector3)min + Vector3::UP, Color::RED, false);
	MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine((Vector3)min, (Vector3)min + Vector3::RIGHT, Color::RED, false);
	MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine((Vector3)max, (Vector3)max + Vector3::LEFT, Color::RED, false);
	MyTools::currentScene->GetComponent<DebugRenderer>()->AddLine((Vector3)max, (Vector3)max + Vector3::DOWN, Color::RED, false);
}
void MyTools::SelectNode(Node* node) {

}
float MyTools::Round(float value, size_t decimals) {
	float remainder = Mod(value, (float)1 / Pow((size_t)10, decimals));
	return value - remainder;
}
Vector2 MyTools::Round(Vector2 value, size_t decimals) {
	value.x_ = Round(value.x_, decimals);
	value.y_ = Round(value.y_, decimals);
	return value;
}
Vector3 MyTools::Round(Vector3 value, size_t decimals) {
	value.x_ = Round(value.x_, decimals);
	value.y_ = Round(value.y_, decimals);
	value.z_ = Round(value.z_, decimals);
	return value;
}
String MyTools::GetValue(String key) {
	for (size_t i = 0; i < keys.Size(); i++)
	{
		if (keys[i] == key)
		{
			return values[i];
		}
	}
	return "";
}
int MyTools::GetInt(String key, int defaultValue) {
	String stringValue = GetValue(key);
	int value = -1;
	try
	{
		value = std::stoi(stringValue.CString());
	}
	catch (const std::exception& e)
	{
		URHO3D_LOGERROR("GetInt " + key + " failed: " + e.what());
		return defaultValue;
	}
	return value;
}
void MyTools::SetValue(String key, String value) {
	URHO3D_LOGINFO("Set value " + key + " at " + value);
	for (size_t i = 0; i < keys.Size(); i++)
	{
		if (keys[i] == key)
		{
			values[i] = value;//key alredy exists, setting it to
			return;
		}
	}
	//adding new key & value
	keys.Push(key);
	values.Push(value);
}
void MyTools::LoadValues() {
	URHO3D_LOGINFO("Loading values NEWBUILD");
	File loadFile(instance->context_, MyFileSystem->GetUserDocumentsDir() + "myvalues.txt", FileMode::FILE_READWRITE);
	//need to check null?
	//dont need to clear because setvalue checks for existing values?
	String line = loadFile.ReadLine();
	String key = "";
	String value = "";
	while (line != "")
	{
		StringVector parts = line.Split(':');
		if (parts.Size() == 2)
		{
			SetValue(parts[0], parts[1]);
		}
		line = loadFile.ReadLine();
	}
}
void MyTools::SaveValues() {
	URHO3D_LOGINFO("Saving values");
	MyFileSystem->Delete(MyFileSystem->GetUserDocumentsDir() + "myvalues.txt");//delete old file
	File file(instance->context_, MyFileSystem->GetUserDocumentsDir() + "myvalues.txt", FileMode::FILE_READWRITE);
	//URHO3D_LOGINFO(MyFileSystem->GetUserDocumentsDir() + "myvalues.txt");
	URHO3D_LOGINFO("THIS IS A NEW LOG IN THE NEW THING");
	for (size_t i = 0; i < keys.Size(); i++)
	{
		file.WriteLine(keys[i] + ":" + values[i]);
	}
}
void MyTools::PrintValues() {
	URHO3D_LOGINFO("Printing all values:");
	for (size_t i = 0; i < keys.Size(); i++)
	{
		URHO3D_LOGINFO(keys[i] + ":" + values[i]);
	}
}
void MyTools::ClearValues() {
	keys.Clear();
	values.Clear();
}
bool MyTools::InView(Vector2 point) {
	Vector2 min = ViewportToWorld(0, 1);
	Vector2 max = ViewportToWorld(1, 0);
	if (Between(point.x_, min.x_, max.x_) &&
		Between(point.y_, min.y_, max.y_))
	{
		return true;
	}
	return false;
}
float MyTools::DistanceFromView(Vector2 point) {
	if (InView(point))
	{
		return 0;
	}
	Vector2 min = ViewportToWorld(0, 1);
	Vector2 max = ViewportToWorld(1, 0);
	float xDistance = 0;
	if (point.x_ < min.x_)
	{
		xDistance = min.x_ - point.x_;
	}
	if (point.x_ > max.x_)
	{
		xDistance = point.x_ - max.x_;
	}
	float yDistance = 0;
	if (point.y_ < min.y_)
	{
		yDistance = min.y_ - point.y_;
	}
	if (point.y_ > max.y_)
	{
		yDistance = point.y_ - max.y_;
	}
	float distance = Max(xDistance, yDistance);
	return distance;
}
