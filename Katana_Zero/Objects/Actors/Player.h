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
	PlayerState EState;
	Animator* _animator = nullptr;

	Vector2 vGravity = Vector2(0, 1000.f);
	Vector2 vVelocity = {};
	Vector2 vAcceleration = {};

	float fMoveForce = 2000;
	float fMass = 1;

	float fJumpPressedTime = 0.f;
	float fPrevPressedTime = 0.f;

	float fJumpForce = 0;
	bool bIsGround = false;
	bool bJumped = false;
	bool bJumpPressed = false;
	bool bIsFalling = true;
	float JumpFactor = -100;
	float JumpAccumFactor = -10;
	float minJumpForce = -2000.f;
	float maxJumpForce = -3000.f;

	float minJumpHeight = -80.f;
	float maxJumpHeight = -200.f;
	float targetJumpY = 0.f;
	float JumpStartY = 0.f;

public:
	virtual void Init(Vector2 pos) override;
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC hdc) override;

	virtual int32 GetCurrentState() override { return (int32)EState; }

	virtual void OnCollisionHit(Collider* other) override;

	void ApplyPhysics(float deltaTime);
	void newApplyPhysics(float deltaTime);
};