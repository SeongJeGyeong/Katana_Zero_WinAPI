#pragma once
#include "../Utils/Singleton.h"

class Collider;
class OBBCollider;
class AABBCollider;

class CollisionManager : public Singleton<CollisionManager>
{
	friend Singleton<CollisionManager>;
private:
	CollisionManager(){}

	vector<Collider*> _colliderList[(int32)CollisionLayer::END];

	uint8 COLLISION_BIT_MASK_IGNORE[(int32)CollisionLayer::END] = {};

	void SetIgnoreFlag(CollisionLayer type, CollisionLayer ignore);

	bool CheckCollision(Collider* receive, Collider* send);
	bool CheckBetweenOBB(OBBCollider* receive, OBBCollider* send);
	bool CheckBetweenAABB(AABBCollider* receive, AABBCollider* send);
	bool CheckSeparatingAxis(pair<float, float> proj1, pair<float, float> proj2);

	void ExecuteCollisionFunc(Collider* receive, Collider* send);

public:
	void Init();
	void Update();
	void PostUpdate();
	void Render(HDC hdc);

	void AddCollider(Collider* collider);
};

