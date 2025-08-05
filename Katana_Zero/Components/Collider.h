#pragma once
#include "Component.h"

class Collider : public Component
{
private:
	bool bCollideActive;
	Vector2 vHittedNormal;
	Vector2 vHitPos;
	Vector2 vHitMTV;

	Vector2 vCenter;

protected:
	CollisionLayer eCollisionLayer;
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
	ColliderType GetColliderType() { return eColliderType; }
	void SetColliderType(ColliderType type) { eColliderType = type; }

	virtual float GetWidth() { return 0; }
	virtual float GetHeight() { return 0; }

	Vector2 GetCenterOffset();
	void SetCenterOffset(Vector2 center) { vCenter = center; }

	bool GetOverlapped() { return bOverlapped; }
	void SetOverlapped(bool overlap) { bOverlapped = overlap; }
	bool GetBlocked() { return bBlocked; }
	void SetBlocked(bool block) { bBlocked = block; }

	Vector2 GetHittedNormal() { return vHittedNormal; }
	void SetHittedNormal(Vector2 normal) { vHittedNormal = normal; }
	Vector2 GetHitPos() { return vHitPos; }
	void SetHitPos(Vector2 pos) { vHitPos = pos; }
	Vector2 GetHitMTV() { return vHitMTV; }
	void SetHitMTV(Vector2 mtv) { vHitMTV = mtv; }

	virtual void SetHitInfo(Vector2 pos, Vector2 normal) {}
	Vector2 newPos;
};

