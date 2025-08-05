#include "pch.h"
#include "Actor.h"
#include "../../Components/OBBCollider.h"
#include "../../Components/AABBCollider.h"
#include "../../Managers/CollisionManager.h"

void Actor::Init(Vector2 pos)
{
	Super::Init();
	Super::SetPos(pos);
}

void Actor::Update(float deltaTime)
{
}

void Actor::Render(HDC hdc)
{
	if (_collider) _collider->RenderComponent(hdc);
}

void Actor::CreateOBBCollider(float width, float height, float rotation, Vector2 centerOffset, CollisionLayer layer)
{
	_collider = new OBBCollider();
	_collider->InitOBB(width, height, rotation);
	_collider->SetOwner(this);
	_collider->SetCenterOffset(centerOffset);
	_collider->SetCollisionLayer(layer);
	_collider->SetColliderType(ColliderType::OBB);
	CollisionManager::GetInstance()->AddCollider(_collider);
}

void Actor::CreateAABBCollider(float width, float height, Vector2 centerOffset, CollisionLayer layer)
{
	_collider = new AABBCollider();
	_collider->InitAABB(width, height);
	_collider->SetOwner(this);
	_collider->SetCenterOffset(centerOffset);
	_collider->SetCollisionLayer(layer);
	_collider->SetColliderType(ColliderType::AABB);
	CollisionManager::GetInstance()->AddCollider(_collider);
}
