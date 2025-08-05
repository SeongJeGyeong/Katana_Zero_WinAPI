#pragma once
#include "Collider.h"

class AABBCollider : public Collider
{
	using Super = Collider;
private:
	float fWidth;
	float fHeight;

public:
	virtual void InitAABB(float width, float height) override;
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc) override;

	Vector2 GetAABBMin();
	Vector2 GetAABBMax();
	Vector2 GetCenter() { return GetPos(); }

	void SetHitInfo(Vector2 pos, Vector2 normal) override;
	virtual float GetWidth() override { return fWidth; }
	virtual float GetHeight() override { return fHeight; }
};

