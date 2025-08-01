#pragma once

class Actor;

class Component
{
public:
	virtual ~Component() {}

private:
	Actor* _owner = nullptr;

public:
	virtual void InitComponent() {};
	virtual void InitOBB() {};
	virtual void UpdateComponent(float deltaTime) = 0;
	virtual void RenderComponent(HDC hdc) = 0;

	Actor* GetOwner() const { return _owner; }
	void SetOwner(Actor* owner) { _owner = owner; }
	Vector2 GetPos() const;

};

