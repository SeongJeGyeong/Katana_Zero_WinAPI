#include "pch.h"
#include "CollisionManager.h"
#include "../Components/Collider.h"
#include "../Components/OBBCollider.h"
#include "../Components/AABBCollider.h"
#include "../Objects/Actors/Actor.h"

void CollisionManager::Init()
{
	// �浹�� ������ ��ü�� ����
	SetIgnoreFlag(CollisionLayer::PLAYER, CollisionLayer::PLAYER);
	SetIgnoreFlag(CollisionLayer::PLAYER, CollisionLayer::PLAYER_HITBOX);
	SetIgnoreFlag(CollisionLayer::ENEMY, CollisionLayer::ENEMY);
	SetIgnoreFlag(CollisionLayer::ENEMY, CollisionLayer::ENEMY_HITBOX);
	SetIgnoreFlag(CollisionLayer::TILE_FG, CollisionLayer::TILE_FG);
}

void CollisionManager::Update()
{
	for (int32 receiveLayer = 0; receiveLayer < CollisionLayer::END; ++receiveLayer)
	{
		for (auto receiveCollider : _colliderList[(CollisionLayer)receiveLayer])
		{
			for (int32 sendLayer = 0; sendLayer < CollisionLayer::END; ++sendLayer)
			{
				if (COLLISION_BIT_MASK_IGNORE[receiveLayer] & (uint8)(1 << sendLayer)) continue;

				for (auto sendCollider : _colliderList[(CollisionLayer)sendLayer])
				{
					if (CheckCollision(receiveCollider, sendCollider))
					{
						sendCollider->GetOwner()->OnCollisionEnter(receiveCollider);
						receiveCollider->GetOwner()->OnCollisionEnter(sendCollider);
					}
				}
			}


		}
	}

}

void CollisionManager::Render(HDC hdc)
{
}

void CollisionManager::AddCollider(Collider* collider)
{
	_colliderList[collider->GetCollisionLayer()].push_back(collider);
}

void CollisionManager::PostUpdate()
{
}

void CollisionManager::SetIgnoreFlag(CollisionLayer type, CollisionLayer ignore)
{
	COLLISION_BIT_MASK_IGNORE[(int8)type] |= ((uint8)1 << (int8)ignore);
	COLLISION_BIT_MASK_IGNORE[(int8)ignore] |= ((uint8)1 << (int8)type);
}

bool CollisionManager::CheckCollision(Collider* receive, Collider* send)
{
	ColliderType receiveType = receive->GetColliderType();
	ColliderType sendType = send->GetColliderType();

	if (receiveType == ColliderType::OBB && receiveType == sendType)
	{
		return CheckBetweenOBB(static_cast<OBBCollider*>(receive), static_cast<OBBCollider*>(send));
	}
	else if (receiveType == ColliderType::AABB && receiveType == sendType)
	{
		return CheckBetweenAABB(static_cast<AABBCollider*>(receive), static_cast<AABBCollider*>(send));
	}

	return false;
}

bool CollisionManager::CheckBetweenOBB(OBBCollider* receive, OBBCollider* send)
{
	pair<Vector2, Vector2> receiveAxes = receive->GetAxes();
	pair<Vector2, Vector2> sendAxes = send->GetAxes();

	if (!CheckSeparatingAxis(receive->ProjectionAxis(receiveAxes.first), send->ProjectionAxis(receiveAxes.first)))
	{
		return false;
	}
	if (!CheckSeparatingAxis(receive->ProjectionAxis(receiveAxes.second), send->ProjectionAxis(receiveAxes.second)))
	{
		return false;
	}

	if (!CheckSeparatingAxis(receive->ProjectionAxis(sendAxes.first), send->ProjectionAxis(sendAxes.first)))
	{
		return false;
	}
	if (!CheckSeparatingAxis(receive->ProjectionAxis(sendAxes.second), send->ProjectionAxis(sendAxes.second)))
	{
		return false;
	}

	// ��� �࿡�� ��ġ�Ƿ� �浹
	return true;
}

bool CollisionManager::CheckBetweenAABB(AABBCollider* receive, AABBCollider* send)
{
	Vector2 receiveMin = receive->GetAABBMin();
	Vector2 receiveMax = receive->GetAABBMax();

	Vector2 sendMin = send->GetAABBMin();
	Vector2 sendMax = send->GetAABBMax();

	Vector2 receiveCenter = receive->GetCenter();
	Vector2 sendCenter = send->GetCenter();

	// �浹ü���� �߽��� �մ� ����
	Vector2 n = sendCenter - receiveCenter;

	// �浹ü�� ����, ������ ����
	Vector2 receiveExtent = (receiveMax - receiveMin) * 0.5f;
	Vector2 sendExtent = (sendMax - sendMin) * 0.5f;

	// n.x�� 1�̰ų� -1�̸� x���� �浹
	double overlapX = receiveExtent.x + sendExtent.x - abs(n.x);
	// n.y�� 1�̰ų� -1�̸� y���� �浹
	double overlapY = receiveExtent.y + sendExtent.y - abs(n.y);

	if (overlapX < 0 || overlapY < 0) return false;
	Vector2 normal;
	if (overlapX < overlapY)
	{
		normal = (n.x < 0) ? Vector2(-1, 0) : Vector2(1, 0);

		double penetration = overlapX;
		int32 contactCount = 2;
		Vector2 contactPoint1 = Vector2((n.x < 0 ? receiveMin.x : receiveMax.x), max(receiveMin.y, sendMin.y));
		Vector2 contactPoint2 = Vector2(contactPoint1.x, min(receiveMax.y, sendMax.y));
	}
	else
	{
		normal = (n.y < 0) ? Vector2(0, -1) : Vector2(0, 1);

		double penetration = overlapY;
		// �簢������ ��ġ�� ��ġ�� 2����
		int32 contactCount = 2;
		Vector2 contactPoint1 = Vector2(max(receiveMin.x, sendMin.x), (n.x < 0 ? receiveMin.y : receiveMax.y));
		Vector2 contactPoint2 = Vector2(max(receiveMax.x, sendMax.x), contactPoint1.y);
	}

	return true;

	// send�� down�� receive�� up���� ���� ��(���� �� ���� ��)
	// send�� up�� receive�� down���� ���� ��(���� �� Ŭ ��)
	// send�� left�� receive�� right���� �������� ��(���� �� Ŭ ��)
	// send�� right�� receive�� left���� ������ ��(���� �� ���� ��)
	//if (sendMax.y < receiveMin.y ||
	//	receiveMax.y < sendMin.y ||
	//	receiveMax.x < sendMin.x ||
	//	sendMax.x < receiveMin.x)
	//{
	//	// �浹���� �ʾ���
	//	return false;
	//}

	//return true;
}

bool CollisionManager::CheckSeparatingAxis(pair<float, float> proj1, pair<float, float> proj2)
{
	if (proj1.second < proj2.first || proj2.second < proj1.first)
	{
		return false; // �и��� �߰�
	}

	return true;
}

void CollisionManager::ExecuteCollisionFunc(Collider* receive, Collider* send)
{
	//send->GetCollider()->
}
