

#include "LevelParser.h"
#include "MyTools.h"
//T‰‰ file on periaatteessa stripped down versio Urhon 2d peliesimerkin "Sample2D" tiedostosta. T‰t‰ k‰ytet‰‰n TMX filest‰ juttujen spawnaamiseen peliin

LevelParser::LevelParser(Context* context) : Object(context)
{
}
void LevelParser::RegisterObject(Context* context)
{
	context->RegisterFactory<LevelParser>();
}
void LevelParser::CreateCollisionShapesFromTMXObjects(Node* tileMapNode, TileMapLayer2D* tileMapLayer, TileMapInfo2D info)
{
	// Create rigid body to the root node
	auto* body = tileMapNode->CreateComponent<RigidBody2D>();
	body->SetBodyType(BT_STATIC);

	// Generate physics collision shapes and rigid bodies from the tmx file's objects located in "Physics" layer
	for (int i = 0; i < tileMapLayer->GetNumObjects(); ++i)
	{
		TileMapObject2D* tileMapObject = tileMapLayer->GetObject(i); // Get physics objects

		// Create collision shape from tmx object
		switch (tileMapObject->GetObjectType())
		{
		case OT_RECTANGLE:
		{
			CreateRectangleShape(tileMapNode, tileMapObject, tileMapObject->GetSize(), info);
		}
		break;

		case OT_ELLIPSE:
		{
			CreateCircleShape(tileMapNode, tileMapObject, tileMapObject->GetSize().x_ / 2, info); // Ellipse is built as a Circle shape as it doesn't exist in Box2D
		}
		break;

		case OT_POLYGON:
		{
			CreatePolygonShape(tileMapNode, tileMapObject);
		}
		break;

		case OT_POLYLINE:
		{
			CreatePolyLineShape(tileMapNode, tileMapObject);
		}
		break;
		}
	}
}

CollisionBox2D* LevelParser::CreateRectangleShape(Node* node, TileMapObject2D* object, Vector2 size, TileMapInfo2D info)
{
	auto* shape = node->CreateComponent<CollisionBox2D>();
	shape->SetSize(size);
	if (info.orientation_ == O_ORTHOGONAL)
		shape->SetCenter(object->GetPosition() + size / 2);
	else
	{
		shape->SetCenter(object->GetPosition() + Vector2(info.tileWidth_ / 2, 0.0f));
		shape->SetAngle(45.0f); // If our tile map is isometric then shape is losange
	}
	shape->SetFriction(0.8f);
	if (object->HasProperty("Friction"))
		shape->SetFriction(ToFloat(object->GetProperty("Friction")));
	return shape;
}

CollisionCircle2D* LevelParser::CreateCircleShape(Node* node, TileMapObject2D* object, float radius, TileMapInfo2D info)
{
	auto* shape = node->CreateComponent<CollisionCircle2D>();
	Vector2 size = object->GetSize();
	if (info.orientation_ == O_ORTHOGONAL)
		shape->SetCenter(object->GetPosition() + size / 2);
	else
	{
		shape->SetCenter(object->GetPosition() + Vector2(info.tileWidth_ / 2, 0.0f));
	}

	shape->SetRadius(radius);
	shape->SetFriction(0.8f);
	if (object->HasProperty("Friction"))
		shape->SetFriction(ToFloat(object->GetProperty("Friction")));
	return shape;
}

CollisionPolygon2D* LevelParser::CreatePolygonShape(Node* node, TileMapObject2D* object)
{
	auto* shape = node->CreateComponent<CollisionPolygon2D>();
	int numVertices = object->GetNumPoints();
	shape->SetVertexCount(numVertices);
	for (int i = 0; i < numVertices; ++i)
		shape->SetVertex(i, object->GetPoint(i));
	shape->SetFriction(0.8f);
	if (object->HasProperty("Friction"))
		shape->SetFriction(ToFloat(object->GetProperty("Friction")));
	return shape;
}

CollisionChain2D* LevelParser::CreatePolyLineShape(Node* node, TileMapObject2D* object)
{
	auto* shape = node->CreateComponent<CollisionChain2D>();
	int numVertices = object->GetNumPoints();
	shape->SetVertexCount(numVertices);
	for (int i = 0; i < numVertices; ++i)
		shape->SetVertex(i, object->GetPoint(i));
	shape->SetFriction(0.8f);
	if (object->HasProperty("Friction"))
		shape->SetFriction(ToFloat(object->GetProperty("Friction")));
	return shape;
}

//T‰‰ spawmaa vesitilei
Node* LevelParser::CreateCoin()
{
	auto* cache = GetSubsystem<ResourceCache>();
	Node* node = MyTools::currentScene->CreateChild("Coin"); //t‰m‰ tulisi olla vesitile.png
	node->SetScale(1);
	auto* animatedSprite = node->CreateComponent<AnimatedSprite2D>();
	auto* animationSet = cache->GetResource<AnimationSet2D>("Urho2D/Aaltoanim/Aalto_Animation.scml"); //vesitilen animaatio t‰h‰n scml - filen‰
	animatedSprite->SetAnimationSet(animationSet); // Get scml file and Play "idle" anim
	animatedSprite->SetAnimation("NewAnimation");
	animatedSprite->SetLayer(16);
	//auto* body = node->CreateComponent<RigidBody2D>();
	//body->SetBodyType(BT_STATIC);
	//auto* shape = node->CreateComponent<CollisionCircle2D>(); // Create circle shape
	//shape->SetRadius(0.32f); // Set radius
	//shape->SetTrigger(true);
	return node;
}

//Liittyy ylemp‰‰n funktioon
void LevelParser::PopulateCoins(TileMapLayer2D* coinsLayer, Vector2 levelPosition, float level)
{
	// Create coin (will be cloned at each placeholder)
	Node* coinNode = CreateCoin();
	
	// Instantiate coins to pick at each placeholder
	//float nextOfset = 10.0f;
	for (int i = 0; i < coinsLayer->GetNumObjects(); ++i)
	{
		if (level == 2)
		{
			TileMapObject2D* coinObject = coinsLayer->GetObject(i); // Get placeholder object
			Node* coinClone = coinNode->Clone();
			//nextOfset = nextOfset + 5.0f;
			//coinClone->SetPosition2D(coinObject->GetPosition() + coinObject->GetSize() + Vector2(0 + nextOfset, 0.0f));
			//coinClone->SetPosition2D(coinObject->GetPosition() * coinObject->GetSize() + Vector2(0.0f, 1.9f));
			coinClone->SetScale2D(2.83, 2.83);
			coinClone->SetPosition2D(Vector2(levelPosition.x_, levelPosition.y_) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			coinClone->SetPosition2D(Vector2(41.30, -8.4) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			URHO3D_LOGINFO(String(coinClone->GetPosition().x_));
			URHO3D_LOGINFO(String(coinClone->GetPosition().y_));
		}
		if (level == 3)
		{
			TileMapObject2D* coinObject = coinsLayer->GetObject(i); // Get placeholder object
			Node* coinClone = coinNode->Clone();
			//nextOfset = nextOfset + 5.0f;
			//coinClone->SetPosition2D(coinObject->GetPosition() + coinObject->GetSize() + Vector2(0 + nextOfset, 0.0f));
			//coinClone->SetPosition2D(coinObject->GetPosition() * coinObject->GetSize() + Vector2(0.0f, 1.9f));
			coinClone->SetScale2D(2, 2);
			coinClone->SetPosition2D(Vector2(levelPosition.x_, levelPosition.y_) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			coinClone->SetPosition2D(Vector2(20, -6.0) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			URHO3D_LOGINFO(String(coinClone->GetPosition().x_));
			URHO3D_LOGINFO(String(coinClone->GetPosition().y_));
		}
		if (level == 4)
		{
			TileMapObject2D* coinObject = coinsLayer->GetObject(i); // Get placeholder object
			Node* coinClone = coinNode->Clone();
			//nextOfset = nextOfset + 5.0f;
			//coinClone->SetPosition2D(coinObject->GetPosition() + coinObject->GetSize() + Vector2(0 + nextOfset, 0.0f));
			//coinClone->SetPosition2D(coinObject->GetPosition() * coinObject->GetSize() + Vector2(0.0f, 1.9f));
			coinClone->SetScale2D(2, 2);
			coinClone->SetPosition2D(Vector2(levelPosition.x_, levelPosition.y_) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			coinClone->SetPosition2D(Vector2(-11.95, -7.0) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			URHO3D_LOGINFO(String(coinClone->GetPosition().x_));
			URHO3D_LOGINFO(String(coinClone->GetPosition().y_));
		}
		if (level == 8)
		{
			TileMapObject2D* coinObject = coinsLayer->GetObject(i); // Get placeholder object
			Node* coinClone = coinNode->Clone();
			//nextOfset = nextOfset + 5.0f;
			//coinClone->SetPosition2D(coinObject->GetPosition() + coinObject->GetSize() + Vector2(0 + nextOfset, 0.0f));
			//coinClone->SetPosition2D(coinObject->GetPosition() * coinObject->GetSize() + Vector2(0.0f, 1.9f));
			coinClone->SetScale2D(4, 4);
			coinClone->SetPosition2D(Vector2(levelPosition.x_, levelPosition.y_) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			coinClone->SetPosition2D(Vector2(26.5, -9.8) - coinObject->GetPosition() + coinObject->GetSize() + Vector2(0.0f, -1.9f));
			URHO3D_LOGINFO(String(coinClone->GetPosition().x_));
			URHO3D_LOGINFO(String(coinClone->GetPosition().y_));
		}
	}

	// Remove node used for cloning purpose
	coinNode->Remove();
}
