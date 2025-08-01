#pragma once
#include "Component.h"

class Collider : public Component
{
private:
	bool bCollideActive;

protected:
	CollisionLayer eCollisionLayer;
	CollisionType eCollisionType;
	ColliderType eColliderType;
	bool bOverlapped = false;

public:
	virtual void InitComponent() {};
	virtual void InitOBB(float width, float height, float rotation) {};
	virtual void InitAABB(float width, float height) {}
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc) override;

	CollisionLayer GetCollisionLayer() { return eCollisionLayer; }
	void SetCollisionLayer(CollisionLayer layer) { eCollisionLayer = layer; }
	CollisionType GetCollisionType() { return eCollisionType; }
	void SetCollisionType(CollisionType type) { eCollisionType = type; }
	ColliderType GetColliderType() { return eColliderType; }
	void SetColliderType(ColliderType type) { eColliderType = type; }

	void SetOverlapped(bool overlap) { bOverlapped = overlap; }
};

