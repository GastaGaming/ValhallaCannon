#pragma once
#include <Urho3D/Urho3DAll.h>

namespace Urho3D
{
	class Node;
	class TileMapObject2D;
	class TileMapLayer2D;
}

//class Character2D;

using namespace Urho3D;

class LevelParser : public Object
{

	URHO3D_OBJECT(LevelParser, Object);

public:
	explicit LevelParser(Context* context);
	~LevelParser() override = default;

	// Generate physics collision shapes from the tmx file's objects located in tileMapLayer.
	void CreateCollisionShapesFromTMXObjects(Node* tileMapNode, TileMapLayer2D* tileMapLayer, TileMapInfo2D info);
	/// Build collision shape from Tiled 'Rectangle' objects.
	CollisionBox2D* CreateRectangleShape(Node* node, TileMapObject2D* object, Vector2 size, TileMapInfo2D info);
	/// Build collision shape from Tiled 'Ellipse' objects.
	CollisionCircle2D* CreateCircleShape(Node* node, TileMapObject2D* object, float radius, TileMapInfo2D info);
	/// Build collision shape from Tiled 'Polygon' objects.
	CollisionPolygon2D* CreatePolygonShape(Node* node, TileMapObject2D* object);
	/// Build collision shape from Tiled 'Poly Line' objects.
	CollisionChain2D* CreatePolyLineShape(Node* node, TileMapObject2D* object);
	/// Create a coin (will be cloned at each tmx placeholder).
	Node* CreateCoin();
	/// Instantiate coins to pick at each placeholder.
	void PopulateCoins(TileMapLayer2D* coinsLayer, Vector2 levelPosition, float level);
	/// The scene.
	Scene* scene_{};

	//
	static void RegisterObject(Context* context);
};

