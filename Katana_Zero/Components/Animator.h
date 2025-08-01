#pragma once
#include "Component.h"

class Texture;
//template<typename StateEnum>
class Animator : public Component
{
private:
	//static_assert(is_enum_v<StateEnum>, "열거형만 설정할 수 있습니다");

	vector<Texture*> _animationList;
	Texture* _currentAnimation = nullptr;
	float fElapsedTime = 0.f;

	//bool bPlaying = true;

public:
	virtual void InitComponent(int32 endIndex);
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc) override;

	void AddAnimation(int32 Index, Texture* sprite);
	void SetAnimation(int32 Index);

	bool IsAnimationFinished();

	void FlipCurAnimation(bool flip);
};

