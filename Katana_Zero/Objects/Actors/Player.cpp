#include "pch.h"
#include "Player.h"
#include "../../Game/Game.h"
#include "../../Managers/InputManager.h"
#include "../../Scenes/Scene.h"
#include "../../Components/Animator.h"
#include "../../Managers/ResourceManager.h"
#include "../../Components/OBBCollider.h"
#include "../../Managers/CollisionManager.h"

void Player::Init(Vector2 pos)
{
	Super::Init(pos);

	EState = PlayerState::PLAYER_IDLE;
	_animator = new Animator();
	_animator->SetOwner(this);
	_animator->InitComponent(PlayerState::PLAYER_END);
	_animator->AddAnimation(PlayerState::PLAYER_IDLE, ResourceManager::GetInstance()->GetTexture("zero_idle"));

	//CreateOBBCollider(35, 50, 0, CollisionLayer::PLAYER);

	CreateAABBCollider(30, 50, { 0, 10 }, CollisionLayer::PLAYER);
}

void Player::Update(float deltaTime)
{
    vAcceleration = Vector2(0, 0);

    //fJumpForce = clamp(fJumpForce + vGravity.y, -15000.f, 0.f);

    if (InputManager::GetInstance()->GetButtonDown(KeyType::S))
    {
        JumpFactor += 10;
    }
    if (InputManager::GetInstance()->GetButtonDown(KeyType::L))
    {
        JumpFactor -= 10;
    }

    if (bJumpPressed && InputManager::GetInstance()->GetButtonPressed(KeyType::W))
    {
        fJumpPressedTime += deltaTime;

        float newTarget = targetJumpY - 10.f * deltaTime;
        float maxTarget = JumpStartY + maxJumpHeight;

        if (newTarget > maxTarget)
        {
            targetJumpY = newTarget;
        }
        else
        {
            targetJumpY = maxTarget;
        }

        /*  if (fJumpPressedTime < 0.2f)
          {
              float t = fJumpPressedTime / 0.2f;
              fJumpForce = minJumpForce + (maxJumpForce - minJumpForce) * t;
          }
          else
          {
              fPrevPressedTime = fJumpPressedTime;
              bJumped = true;
          }*/


          //bJumpPressed = true;
          //fJumpPressedTime += deltaTime;
          //if (fJumpPressedTime > 0.1f)
          //{
          //    bIsGround = false;
          //    if (fJumpPressedTime >= 0.2f)
          //    {
          //        bJumped = true;
          //    }
          //    else
          //    {
          //        fJumpForce -= 2200.f * deltaTime;
          //    }
          //}
    }

    if (bIsGround && !bJumped && InputManager::GetInstance()->GetButtonDown(KeyType::W))
    {
        bJumped = true;
        bJumpPressed = true;
        bIsFalling = false;
        bIsGround = false;

        fJumpPressedTime = 0.f;

        JumpStartY = GetPos().y;
        targetJumpY = JumpStartY + minJumpHeight;

        //fJumpForce = minJumpForce;
    }

    if (InputManager::GetInstance()->GetButtonUp(KeyType::W))
    {
        bJumpPressed = false;

        //if (fJumpPressedTime < 0.1f)
        //{
        //    fJumpForce = minJumpForce;
        //}
        //bJumped = true;
        //fPrevPressedTime = fJumpPressedTime;
        //fJumpPressedTime = 0;
        //bJumpPressed = false;

        //if (fJumpPressedTime < 0.1f)
        //{
        //    fJumpForce = -100.f;
        //    bIsGround = false;
        //}
        //bJumped = true;
        //fJumpPressedTime = 0;
        //bJumpPressed = false;
    }

    if (InputManager::GetInstance()->GetButtonPressed(KeyType::S))
    {

    }
    if (InputManager::GetInstance()->GetButtonPressed(KeyType::A))
    {
        vAcceleration.x -= fMoveForce / fMass;
        _animator->FlipCurAnimation(true);
    }
    if (InputManager::GetInstance()->GetButtonPressed(KeyType::D))
    {
        vAcceleration.x += fMoveForce / fMass;
        _animator->FlipCurAnimation(false);
    }

    _animator->UpdateComponent(deltaTime);

    //newApplyPhysics(deltaTime);
    ApplyPhysics(deltaTime);
}

void Player::Render(HDC hdc)
{
	_animator->RenderComponent(hdc);
	Super::Render(hdc);

    wstring str = std::format(L"IsGround({0}) IsJumped({1}) JumpForce({2})", bIsGround, bJumped, fJumpForce);
    ::TextOut(hdc, 300, 30, str.c_str(), static_cast<int32>(str.size()));
    wstring str2 = std::format(L"Pos({0}, {1})", GetPos().x, GetPos().y);
    ::TextOut(hdc, 100, 50, str2.c_str(), static_cast<int32>(str2.size()));

    wstring str3 = std::format(L"LastPressedtime({0})", fPrevPressedTime);
    ::TextOut(hdc, 100, 80, str3.c_str(), static_cast<int32>(str3.size()));
}

void Player::OnCollisionHit(Collider* other)
{
	_collider->SetBlocked(true);
}

void Player::ApplyPhysics(float deltaTime)
{
    Vector2 normalGravity = vGravity.GetNormalize();
    float GravityLength = vVelocity.Dot(normalGravity);

    if (bJumped && !bIsFalling)
    {
        float currentY = GetPos().y;
        currentY -= 1.f * deltaTime;
        if (currentY <= targetJumpY)
        {
            currentY = targetJumpY;
            bJumped = false;
            bIsFalling = true;
            vVelocity.y = 0;
        }
        vVelocity.y = currentY;
    }
    else if (bIsFalling)
    {
        vVelocity.y += vGravity.y * deltaTime;

        //vAcceleration += vGravity;
    }
    else if (bIsGround)
    {
        if (!bJumpPressed) bJumped = false;
        //vVelocity -= normalGravity * GravityLength;
        fJumpForce = 0;
    }

    //if (bIsGround)
    //{
    //    if(!bJumpPressed) bJumped = false;
    //    vVelocity -= normalGravity * GravityLength;
    //    fJumpForce = 0;
    //}
    //else
    //{
    //    vAcceleration.y += fJumpForce;
    //    vAcceleration += vGravity;
    //    fJumpForce += 8000.f * deltaTime;
    //}

    vVelocity.x += vAcceleration.x * deltaTime;

    //vVelocity.y += fJumpForce;

    float upFactor = 400.f;
    float sideFactor = 150.f;

    //Vector2 gravityVector = normalGravity * GravityLength;
    //Vector2 sideVec = vVelocity - gravityVector;
    //float sideLength = sideVec.Length();

    //if (GravityLength > upFactor) gravityVector = normalGravity * upFactor;

    //if (sideLength > sideFactor) sideVec = sideVec.GetNormalize() * sideFactor;
    //else if (sideLength < -sideFactor) sideVec = sideVec.GetNormalize() * sideFactor;

    if (vVelocity.x > sideFactor)
    {
        vVelocity.x = sideFactor;
    }
    else if (vVelocity.x < -sideFactor)
    {
        vVelocity.x = -sideFactor;
    }

    float friction = 0.95f;
    vVelocity.x *= friction;
    if (abs(vVelocity.x) < 1.0f)
        vVelocity.x = 0.f;

    //vVelocity = gravityVector + sideVec;

    Vector2 oldPos = _collider->GetCenterOffset();
    //Vector2 newPos = oldPos + (vVelocity * deltaTime);
    Vector2 newPos;
    newPos.x = oldPos.x + (vVelocity.x * deltaTime);
    newPos.y = vVelocity.y;

    Vector2 normal;
    Vector2 hitPos;

    if (!bIsGround && CollisionManager::GetInstance()->CheckBlockedCollision(this, newPos, { newPos.x, newPos.y + 1 }, normal, hitPos))
    {
        Vector2 dir = newPos - oldPos;

        bIsGround = (dir.y > 0);
    }

    if (CollisionManager::GetInstance()->CheckBlockedCollision(this, oldPos, newPos, normal, hitPos))
    {
        newPos.y = hitPos.y;
    }
    newPos.y -= 10;

    SetPos(newPos);
}

void Player::newApplyPhysics(float deltaTime)
{
    Vector2 currentPos = _collider->GetCenterOffset();

    // 점프 중이면 위로 보간 이동
    if (bJumped && !bIsFalling)
    {
        currentPos.y -= 300.f * deltaTime;

        if (currentPos.y <= targetJumpY)
        {
            currentPos.y = targetJumpY;
            bJumped = false;
            bIsFalling = true;
            vVelocity.y = 0;
        }
    }
    else if (bIsFalling)
    {
        // 낙하
        vAcceleration += vGravity;
        vVelocity += vAcceleration * deltaTime;
        currentPos += vVelocity * deltaTime;
    }
    else if (bIsGround)
    {
        // 땅에 있을 때 중력 제거
        vVelocity = Vector2(0.f, 0.f);
    }

    Vector2 oldPos = _collider->GetCenterOffset();
    Vector2 newPos = oldPos + (vVelocity * deltaTime);
    // 충돌 처리
    Vector2 normal, hitPos;

    if (!bIsGround && CollisionManager::GetInstance()->CheckBlockedCollision(this, newPos, { newPos.x, newPos.y + 1 }, normal, hitPos))
    {
        Vector2 dir = newPos - oldPos;

        bIsGround = (dir.y > 0);
    }

    if (CollisionManager::GetInstance()->CheckBlockedCollision(this, oldPos, newPos, normal, hitPos))
    {
        newPos = hitPos;
    }
    newPos.y -= 10;

    SetPos(newPos);
}
