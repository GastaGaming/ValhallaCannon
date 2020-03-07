#pragma once
#include <Urho3D/Urho3DAll.h>
#include "MyTimer.h"
#include "Printable.h"
using namespace Urho3D;

class MyTools : public LogicComponent
{
	URHO3D_OBJECT(MyTools, LogicComponent);
public:
	static Scene* currentScene;//move somewhere like scenemanager or gamemaster when it exists
	static MyTools* instance;
	explicit MyTools(Context* context);
	~MyTools();
	static Ray ScreenToRay(IntVector2 point);
	static Ray TouchToRay();
	static Vector2 ScreenToWorld(IntVector2 point);//converts a screenpoint in pixels to 2d world space
	static Vector2 ScreenToWorld(int x, int y);
	static Vector2 ViewportToWorld(Vector2 point);//converts a viewport point to 2d world space
	static Vector2 ViewportToWorld(float x, float y);
	static Vector2 TouchToWorld();//gets current touch point in world coordinates
	static PODVector<RigidBody2D*> MouseHits();//gets all rigidbodies hit by current touch
	static Vector3 ClampMagnitude(Vector3 vec,float maxMagnitude);
	static Vector2 ClampMagnitude(Vector2 vec, float maxMagnitude);
	static float ViewportToWorldDistance(float x, float y);//converts a distance from viewport coords to world units
	static float ViewportToWorldDistance(Vector2 vec);
	static Vector2 ToVector2(Vector3 vec);
	static IntVector2 GetResolution();
	static Vector2 ScreenToViewport(IntVector2 point);//converts screenpoint to viewport coords
	void SetEditMode(bool enabled);
	void HandleUpdate(StringHash eventType, VariantMap& eventData);
	void HandleTouchBegin(StringHash eventType, VariantMap& eventData);
	void HandleTouchEnd(StringHash eventType, VariantMap& eventData);
	void OnSceneSet(Scene* scene) override;
	void DrawRect(Vector2 min, Vector2 max);//debug
	void SelectNode(Node* node);
	static float Round(float value, size_t decimals);
	static Vector2 Round(Vector2 value, size_t decimals);
	static Vector3 Round(Vector3 value, size_t decimals);
	void MoveLayer(int direction);
	static Vector<String> keys;
	static Vector<String> values;
	static String GetValue(String key);
	static int GetInt(String key, int defaultValue = -1);
	static void SetValue(String key, String value);
	static void LoadValues();
	static void SaveValues();
	static void PrintValues();
	static void ClearValues();
	bool drawDebug = false;
	void test() { return; }
	static bool InView(Vector2 point);
	static bool InView(Vector3 point) { return InView(Vector2(point.x_,point.y_)); };
	static bool InView(float x, float y) { return InView(Vector2(x,y)); };
	static float DistanceFromView(Vector2 point);
	template <typename T> static bool Between(T value, T min, T max) { return value >= min && value <= max; }
private:
	//editing
	Node* selectedNode = 0;
	Vector3 touchOffset;
	Vector3 parentOffset;
	bool editMode = false;
	Vector2 editAnchor;
	float anchorDistance = 1;
	Vector3 originalScale = Vector3::ONE;
	float originalRotation = 0;
	/*struct EditInfo {
		Node* node;
		Vector3 touchOffset;
		Vector3 parentOffset;
	};*/
	struct EditKeys{
		static const Key toggleEdit = KEY_E;
		static const Key selectParent = KEY_W;
		static const Key scale = KEY_S;
		static const Key rotate = KEY_R;
		static const Key print = KEY_SPACE;
		static const Key undo = KEY_Q;
		static const Key moveFront = KEY_UP;
		static const Key moveBack = KEY_DOWN;
		static const Key resetValues = KEY_P;
		static const Key toggleDebugColliders = KEY_C;
		static const Key saveScene = KEY_PRINTSCREEN;
	};
};
	extern Input* MyInput;
	extern Time* MyTime;
	extern WorkQueue* MyWorkQueue;
	extern FileSystem* MyFileSystem;
	extern Log* MyLog;
	extern ResourceCache* MyCache;
	extern UI* MyUI;
	extern Audio* MyAudio;
	extern Engine* MyEngine;
	extern Camera* MyCamera;
	extern Graphics* MyGraphics;
	extern Renderer* MyRenderer;