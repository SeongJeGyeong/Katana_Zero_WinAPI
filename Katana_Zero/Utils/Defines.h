#pragma once
//#include <windef.h>

constexpr int GWinSizeX = 1280;
constexpr int GWinSizeY = 720;

constexpr int SubWinSizeX = 992;
constexpr int SubWinSizeY = 492;

const int TILE_SIZE = 32;
const int TileMapWidth = 42;
const int TileMapHeight = 23;

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

#define SMALL_NUMBER			(1.e-8f)

float RadianToDegree(float radian);
float DegreeToRadian(float degree);
bool IsInPoint(RECT rect, POINT pos);

enum CollisionLayer
{
	PLAYER,
	ENEMY,
	TILE_FG,
	TILE_BG,
	PLAYER_HITBOX,
	ENEMY_HITBOX,
	END,
};

enum CollisionResponse
{
	C_IGNORE,
	C_OVERLAP,
	C_BLOCK
};

enum ColliderType
{
	AABB,
	OBB,
	LINE
};

enum SceneType
{
	LOBBY,
	EDITOR,
	GAME,
	NONE,
};

enum RenderLayer
{
	BACKGROUND,
	FOREGROUND,
	ACTOR,
	LAYER_END,
};

struct SpriteInfo
{
	int32 iTileSizeX;
	int32 iTileSizeY;
	int32 iLastFrame = 0;
	int32 iCurFrame = 0;
	float dur = 0;
	bool loop = false;
};

struct Vector2
{
	// 좌표
	float x = 0;
	float y = 0;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(POINT pt) : x((float)pt.x), y((float)pt.y) {}

	// 사칙연산 정의
	// +, - , *, / 
	Vector2 operator+(const Vector2& other)
	{
		Vector2 ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}
	Vector2 operator-(const Vector2& other)
	{
		Vector2 ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}
	Vector2 operator*(float ratio)
	{
		Vector2 ret;
		ret.x = x * ratio;
		ret.y = y * ratio;
		return ret;
	}

	void operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(float ratio)
	{
		x *= ratio;
		y *= ratio;
	}
	bool operator==(const Vector2& other) const
	{
		return (x == other.x && y == other.y);
	}

	// 벡터의 길이 구하기
	// 엔진 살펴보면, Legnth, LengthSquared (벡터는 보통 길이를 리턴하는 함수가 2종류)
	// sqrt(루트) 연산은 생각보다 비싸기 때문에, 꼭 진짜 길이가 필요한 경우가 아니면,
	// LengthSquared (제곱 길이) 버전을 사용하는게 더 좋다.
	float Length()
	{
		return ::sqrt(x * x + y * y);
	}

	// 길이의 제곱 버전
	float LengthSquared()
	{
		return (x * x + y * y);
	}

	// 자기자신을 정규화
	void Normalize()
	{
		// 벡터의 크기를 구해서, 크기만큼 나누기 해주면 된다.
		float length = Length();

		// 언리얼 엔진은 풀 오픈소스. 언리얼 참고해서 공부해도 좋습니다.
		//length 가 0이거나 엄청 작을수 있다
		if (length < SMALL_NUMBER)
			return;	// 굳이 정규화안하고 리턴

		// 정규화.
		x /= length;
		y /= length;
	}

	// 정규화된 복사된 벡터
	Vector2 GetNormalize()
	{
		Vector2 normalize = *this;
		normalize.Normalize();
		return normalize;
	}

	// 내적
	float Dot(const Vector2& other)
	{
		return x * other.x + y * other.y;
	}

	// radian 각도만큼 회전시키자.
	Vector2 Rotate(float radian)
	{
		float cosA = ::cosf(radian);
		float sinA = ::sinf(radian);

		return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
	}

	// 외적
	float Cross(const Vector2& other)
	{
		//2D 라서, 스칼라값만 반환을 한다. 
		// 법선 벡터가 필요한게 아니고, 부호를 판단하는 스칼라값이 필요하다.
		return x * other.y - y * other.x;
	}
};

struct TileInfo
{
	int32 iTilesetIndex;
	Vector2 vTilePos;
};


// 선분과 사각형의 충돌체크
bool LineIntersectsAABB(Vector2 p0, Vector2 p1, const RECT& rect, Vector2& outNormal, Vector2& outPos, float& t);