#pragma once
#include "Actor.h"

enum PlayerState
{
	PLAYER_IDLE,
	PLAYER_IDLE_TO_RUN,
	PLAYER_RUN,
	PLAYER_RUN_TO_IDLE,
	PLAYER_JUMP,
	PLAYER_FALL,
	PLAYER_CROUCH,
	PLAYER_ATTACK,

	PLAYER_HURT_BEGIN,
	PLAYER_HURT_LOOP,
	PLAYER_HURT_GROUND,
	PLAYER_HURT_RECOVER,

	PLAYER_END
};

class Animator;

class Player : public Actor
{
	using Super = Actor;
private:
	float _speed = 300.0f;	// 초당 이동속도
	PlayerState EState;
	Animator* _animator = nullptr;

public:
	virtual void Init(Vector2 pos) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual int32 GetCurrentState() override { return (int32)EState; }

	virtual void OnCollisionEnter(Collider* other) override;
};