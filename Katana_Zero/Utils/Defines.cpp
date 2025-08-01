#include "pch.h"
#include "Defines.h"

float RadianToDegree(float radian)
{
	float degree = radian * (180.0f / std::numbers::pi_v<float>);

	degree = fmodf(degree, 360.0f);
	if (degree < 0.0f)
		degree += 360.0f;

	return degree;
}

float DegreeToRadian(float degree)
{
	float radian = degree * (std::numbers::pi_v<float> / 180.0f);

	radian = fmodf(radian, std::numbers::pi_v<float> *2);
	if (radian < 0.0f)
		radian += std::numbers::pi_v<float> *2;

	return radian;
}

bool IsInPoint(RECT rect, POINT pos)
{
	if (pos.x < rect.left)
		return false;

	if (pos.x > rect.right)
		return false;

	if (pos.y < rect.top)
		return false;

	if (pos.y > rect.bottom)
		return false;
	return true;
}

bool LineIntersectsAABB(Vector2 p0, Vector2 p1, const RECT& rect, Vector2& outNormal, Vector2& outPos, float& t)
{
	Vector2 dir = p1 - p0;

	float tMin = 0.0f;
	float tMax = 1.0f;
	Vector2 normal;

	// x축 검사
	if (dir.x != 0)
	{
		float tx1 = (rect.left - p0.x) / dir.x;
		float tx2 = (rect.right - p0.x) / dir.x;

		Vector2 nx = (tx1 < tx2) ? Vector2(-1, 0) : Vector2(1, 0);
		if (tx1 > tx2) std::swap(tx1, tx2);

		if (tx1 > tMin)
		{
			normal = nx;
		}

		tMin = max(tMin, tx1);
		tMax = min(tMax, tx2);

		if (tMin > tMax)
			return false; // 충돌 안 함
	}
	else if (p0.x < rect.left || p0.x > rect.right)
	{
		return false; // Y축으로 평행하고 밖에 있음
	}

	// y축 검사
	if (dir.y != 0)
	{
		float ty1 = (rect.top - p0.y) / dir.y;
		float ty2 = (rect.bottom - p0.y) / dir.y;

		// 시작 지점으로부터 떨어져있는 거리 비율
		// ty1 가 ty2보다 작다면, 위에서 아래 방향이다.
		Vector2 ny = (ty1 < ty2) ? Vector2(0, -1) : Vector2(0, 1);

		if (ty1 > ty2) std::swap(ty1, ty2);

		if (ty1 > tMin)
		{
			normal = ny;
		}

		tMin = max(tMin, ty1);
		tMax = min(tMax, ty2);

		if (tMin > tMax)
			return false; // 충돌 안 함
	}
	else if (p0.y < rect.top || p0.y > rect.bottom)
	{
		return false;
	}

	// 선분이 사각형 내부에 있는 경우 예외처리
	// 선분의 진행방향에 따라서 축 기준으로 조금더 기울어진 방향으로 normal 을 보정해준다.
	if (normal == Vector2(0, 0))
	{
		if (fabs(dir.x) > fabs(dir.y))
		{
			normal = (dir.x > 0) ? Vector2(-1, 0) : Vector2(1, 0);
		}
		else
		{
			normal = (dir.y > 0) ? Vector2(0, -1) : Vector2(0, 1);
		}
	}

	//if (tMin > tMax || tMin > 1.0f || tMin < 0.0f)
	//    return false;

	// 충돌 결과 저장
	outPos = p0 + dir * tMin;

	// 예외처리 하나더.
	outPos += normal * 0.01f; // 약간의 오프셋을 주어 정확한 충돌 위치를 보정

	outNormal = normal;

	t = tMin;

	return true;
}
