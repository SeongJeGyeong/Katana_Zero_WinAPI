#include "pch.h"
#include "Player.h"
#include "../../Game/Game.h"
#include "../../Managers/InputManager.h"
#include "../../Scenes/Scene.h"
#include "../../Components/Animator.h"
#include "../../Managers/ResourceManager.h"
#include "../../Components/OBBCollider.h"

void Player::Init(Vector2 pos)
{
	Super::Init(pos);

	EState = PlayerState::PLAYER_IDLE;
	_animator = new Animator();
	_animator->SetOwner(this);
	_animator->InitComponent(PlayerState::PLAYER_END);
	_animator->AddAnimation(PlayerState::PLAYER_IDLE, ResourceManager::GetInstance()->GetTexture("zero_idle"));

	CreateOBBCollider(30, 50, 0, CollisionLayer::PLAYER, CollisionType::C_OVERLAP);
}

void Player::Update(float deltaTime)
{
	// �̵� �Ÿ� = �ð� * �ӵ�
	// 1�ʴ� speed��ŭ �̵�
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::W))
	{
		vPos.y -= (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
	{
		vPos.y += (deltaTime * _speed);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
	{
		vPos.x -= (deltaTime * _speed);
		_animator->FlipCurAnimation(true);
	}
	if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
	{
		vPos.x += (deltaTime * _speed);
		_animator->FlipCurAnimation(false);
	}

	_animator->UpdateComponent(deltaTime);
}

void Player::Render(HDC hdc)
{
	_animator->RenderComponent(hdc);
	Super::Render(hdc);
}

void Player::OnCollisionEnter(Collider* other)
{
	_collider->SetOverlapped(true);
}