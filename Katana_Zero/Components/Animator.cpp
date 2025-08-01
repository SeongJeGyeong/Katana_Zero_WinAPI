#include "pch.h"
#include "Animator.h"
#include "../Objects/Actors/Actor.h"
#include "../Resources/Texture.h"

void Animator::InitComponent(int32 endIndex)
{
	_animationList.resize(endIndex);
}

void Animator::UpdateComponent(float deltaTime)
{
	Texture* animSprite = _animationList[GetOwner()->GetCurrentState()];

	fElapsedTime += deltaTime;
	if (fElapsedTime >= animSprite->GetDuration())
	{
		fElapsedTime = 0.f;
		animSprite->SetCurFrameCount(animSprite->GetCurFrameCount() + 1);
		if (animSprite->IsSpriteEnd() && animSprite->IsLoop())
		{
			animSprite->SetCurFrameCount(0);
		}
	}
}

void Animator::RenderComponent(HDC hdc)
{
	Texture* animSprite = _animationList[GetOwner()->GetCurrentState()];
	animSprite->RenderStretchedSprite(hdc, GetPos(), 2);
}

void Animator::AddAnimation(int32 Index, Texture* sprite)
{
	_animationList[Index] = sprite;
}

void Animator::SetAnimation(int32 Index)
{
	_currentAnimation = _animationList[Index];
}

bool Animator::IsAnimationFinished()
{
	return false;
}

void Animator::FlipCurAnimation(bool flip)
{
	_animationList[GetOwner()->GetCurrentState()]->SetIsFlipped(flip);
}
