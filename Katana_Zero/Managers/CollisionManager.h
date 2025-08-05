#pragma once
#include "../Utils/Singleton.h"

class Collider;
class OBBCollider;
class AABBCollider;
class Player;

class CollisionManager : public Singleton<CollisionManager>
{
	friend Singleton<CollisionManager>;
private:
	CollisionManager(){}

	vector<Collider*> _colliderList[(int32)CollisionLayer::END];

	uint8 COLLISION_BIT_MASK_IGNORE[(int32)CollisionLayer::END] = {};
	uint8 COLLISION_BIT_MASK_BLOCK[(int32)CollisionLayer::END] = {};

	void SetBitFlag(CollisionLayer layer1, CollisionLayer layer2, CollisionResponse responseType, bool on);

	bool CheckCollision(Collider* receive, Collider* send);
	bool CheckBetweenOBB(OBBCollider* receive, OBBCollider* send);
	bool CheckBetweenSAT(OBBCollider* receive, OBBCollider* send);
	bool CheckBetweenAABB(AABBCollider* receive, AABBCollider* send);
	bool CheckSeparatingAxis(pair<float, float> proj1, pair<float, float> proj2);

	Vector2 CalculateHitPos(OBBCollider* receive, OBBCollider* send, Vector2 normal);

	void ExecuteCollisionFunc(Collider* receive, Collider* send);

public:
	void Init();
	void Update();
	void PostUpdate();
	void Render(HDC hdc);

	void AddCollider(Collider* collider);

	vector<Collider*> GetPlacedColliders(CollisionLayer layer) { return _colliderList[layer]; }
	bool CheckBlockedCollision(Player* player, Vector2 start, Vector2 end, Vector2& outNormal, Vector2& outPos);
};

