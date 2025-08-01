#pragma once
#include "Collider.h"

class OBBCollider : public Collider
{
	using Super = Collider;
private:
	float fWidth;
	float fHeight;
	float fRotation = 0.f;
	vector<Vector2> _Vertices;

public:
	virtual void InitOBB(float width, float height, float rotation) override;
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc) override;

	pair<Vector2, Vector2> GetAxes() const;
	vector<Vector2> GetVertices() const;

	pair<float, float> ProjectionAxis(const Vector2& axis) const;
};

