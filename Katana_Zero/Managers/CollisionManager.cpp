#include "pch.h"
#include "CollisionManager.h"
#include "../Components/Collider.h"
#include "../Components/OBBCollider.h"
#include "../Components/AABBCollider.h"
#include "../Objects/Actors/Actor.h"

void CollisionManager::Init()
{
	// 충돌을 무시할 객체들 세팅
	SetBitFlag(CollisionLayer::PLAYER, CollisionLayer::PLAYER, CollisionResponse::C_IGNORE, true);
	SetBitFlag(CollisionLayer::PLAYER, CollisionLayer::PLAYER_HITBOX, CollisionResponse::C_IGNORE, true);
	SetBitFlag(CollisionLayer::ENEMY, CollisionLayer::ENEMY, CollisionResponse::C_IGNORE, true);
	SetBitFlag(CollisionLayer::ENEMY, CollisionLayer::ENEMY_HITBOX, CollisionResponse::C_IGNORE, true);
	SetBitFlag(CollisionLayer::TILE_FG, CollisionLayer::TILE_FG, CollisionResponse::C_IGNORE, true);

	SetBitFlag(CollisionLayer::PLAYER, CollisionLayer::TILE_FG, CollisionResponse::C_BLOCK, true);
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
						if (COLLISION_BIT_MASK_BLOCK[receiveLayer] & (uint8)(1 << sendLayer))
						{
							sendCollider->GetOwner()->OnCollisionHit(receiveCollider);
							receiveCollider->GetOwner()->OnCollisionHit(sendCollider);
						}
						else
						{
							sendCollider->GetOwner()->OnCollisionBeginOverlap(receiveCollider);
							receiveCollider->GetOwner()->OnCollisionBeginOverlap(sendCollider);
						}
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

void CollisionManager::SetBitFlag(CollisionLayer layer1, CollisionLayer layer2, CollisionResponse responseType, bool on)
{
	switch (responseType)
	{
	case C_IGNORE:
		if (on)
		{
			// 설정
			COLLISION_BIT_MASK_IGNORE[(int8)layer1] |= ((uint8)1 << (int8)layer2);
			COLLISION_BIT_MASK_IGNORE[(int8)layer2] |= ((uint8)1 << (int8)layer1);
		}
		else
		{
			// 해제
			COLLISION_BIT_MASK_IGNORE[(int8)layer1] &= ~((uint8)1 << (int8)layer2);
			COLLISION_BIT_MASK_IGNORE[(int8)layer2] &= ~((uint8)1 << (int8)layer1);
		}
		break;
	case C_BLOCK:
		if (on)
		{
			// 설정
			COLLISION_BIT_MASK_BLOCK[(int8)layer1] |= ((uint8)1 << (int8)layer2);
			COLLISION_BIT_MASK_BLOCK[(int8)layer2] |= ((uint8)1 << (int8)layer1);
		}
		else
		{
			// 해제
			COLLISION_BIT_MASK_BLOCK[(int8)layer1] &= ~((uint8)1 << (int8)layer2);
			COLLISION_BIT_MASK_BLOCK[(int8)layer2] &= ~((uint8)1 << (int8)layer1);
		}
		break;
	default:
		break;
	}
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
	CollisionInfo info;

	pair<Vector2, Vector2> receiveAxes = receive->GetAxes();
	pair<Vector2, Vector2> sendAxes = send->GetAxes();

	Vector2 axes[4] = { receiveAxes.first, receiveAxes.second, sendAxes.first, sendAxes.second };
	Vector2 minAxis;
	float minOverlap = INFINITY;

	for (int i = 0; i < 4; ++i)
	{
		// first : min, second : max
		pair<float, float> proj1 = receive->ProjectionAxis(axes[i]);
		pair<float, float> proj2 = send->ProjectionAxis(axes[i]);

		if (!CheckSeparatingAxis(proj1, proj2))
		{
			return false;
		}

		float overlap = min(proj1.second, proj2.second) - max(proj1.first, proj2.first);
		if (overlap < minOverlap) {
			minOverlap = overlap;
			minAxis = axes[i];

			// 법선 방향 조정 (obb1에서 obb2로 향하도록)
			//Vector2 centerDiff = obb2.center - obb1.center;
			//if (centerDiff.dot(axis) < 0) {
			//	minAxis = -axis;
			//}
		}

	}

	info.hasCollision = true;
	info.normal = minAxis;
	info.PenetrationDepth = minOverlap;
	info.mtv = minAxis * minOverlap;

	info.hitPos = CalculateHitPos(receive, send, info.normal);

	// 모든 축에서 겹치므로 충돌
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

	// 충돌체끼리 중심을 잇는 벡터
	Vector2 n = sendCenter - receiveCenter;

	// 충돌체의 길이, 높이의 절반
	Vector2 receiveExtent = (receiveMax - receiveMin) * 0.5f;
	Vector2 sendExtent = (sendMax - sendMin) * 0.5f;

	// n.x가 1이거나 -1이면 x축이 충돌
	double overlapX = receiveExtent.x + sendExtent.x - abs(n.x);
	// n.y가 1이거나 -1이면 y축이 충돌
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
		// 사각형끼리 겹치는 위치는 2군데
		int32 contactCount = 2;
		Vector2 contactPoint1 = Vector2(max(receiveMin.x, sendMin.x), (n.x < 0 ? receiveMin.y : receiveMax.y));
		Vector2 contactPoint2 = Vector2(max(receiveMax.x, sendMax.x), contactPoint1.y);
	}

	return true;

	// send의 down이 receive의 up보다 높을 때(값이 더 작을 때)
	// send의 up이 receive의 down보다 낮을 때(값이 더 클 때)
	// send의 left가 receive의 right보다 오른쪽일 때(값이 더 클 때)
	// send의 right가 receive의 left보다 왼쪽일 때(값이 더 작을 때)
	//if (sendMax.y < receiveMin.y ||
	//	receiveMax.y < sendMin.y ||
	//	receiveMax.x < sendMin.x ||
	//	sendMax.x < receiveMin.x)
	//{
	//	// 충돌하지 않았음
	//	return false;
	//}

	//return true;
}

bool CollisionManager::CheckSeparatingAxis(pair<float, float> proj1, pair<float, float> proj2)
{
	if (proj1.second < proj2.first || proj2.second < proj1.first)
	{
		return false; // 분리축 발견
	}

	return true;
}

Vector2 CollisionManager::CalculateHitPos(OBBCollider* receive, OBBCollider* send, Vector2 normal)
{
	std::vector<Vector2> vertices1 = receive->GetVertices();
	std::vector<Vector2> vertices2 = send->GetVertices();

	// 법선 방향으로 가장 가까운 꼭짓점들 찾기
	float maxDist1 = -INFINITY;
	float maxDist2 = -INFINITY;
	Vector2 closestVertex1, closestVertex2;

	// obb1에서 법선 방향으로 가장 먼 점
	for (auto& vertex : vertices1) {
		float dist = vertex.Dot(normal);
		if (dist > maxDist1) {
			maxDist1 = dist;
			closestVertex1 = vertex;
		}
	}

	// obb2에서 법선 반대 방향으로 가장 먼 점
	Vector2 invNormal = { -normal.x,-normal.y };
	for (auto& vertex : vertices2) {
		float dist = vertex.Dot(invNormal);
		if (dist > maxDist2) {
			maxDist2 = dist;
			closestVertex2 = vertex;
		}
	}

	// 두 점의 중점을 접촉점으로 사용
	return (closestVertex1 + closestVertex2) * 0.5f;
}

void CollisionManager::ExecuteCollisionFunc(Collider* receive, Collider* send)
{
	//send->GetCollider()->
}
