#pragma once
#include "Component.h"

class Collider : public Component
{
private:
	bool bCollideActive;

protected:
	CollisionLayer eCollisionLayer;
	CollisionResponse eCollisionType;
	ColliderType eColliderType;
	bool bOverlapped = false;
	bool bBlocked = false;

public:
	virtual void InitComponent() {};
	virtual void InitOBB(float width, float height, float rotation) {};
	virtual void InitAABB(float width, float height) {}
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc) override;

	CollisionLayer GetCollisionLayer() { return eCollisionLayer; }
	void SetCollisionLayer(CollisionLayer layer) { eCollisionLayer = layer; }
	CollisionResponse GetCollisionType() { return eCollisionType; }
	void SetCollisionType(CollisionResponse type) { eCollisionType = type; }
	ColliderType GetColliderType() { return eColliderType; }
	void SetColliderType(ColliderType type) { eColliderType = type; }

	bool GetOverlapped() { return bOverlapped; }
	void SetOverlapped(bool overlap) { bOverlapped = overlap; }
	bool GetBlocked() { return bBlocked; }
	void SetBlocked(bool block) { bBlocked = block; }
};

