#pragma once

#include "../UI/UIWorkTool.h"

class Scene
{
public:
	virtual ~Scene(){}

	virtual void Init();
	virtual void Destroy();
	virtual void Update(float deltatTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hdc);

	SceneType GetSceneType() { return _sceneType; }

protected:
	UIWorkTool _UI;
	SceneType _sceneType = SceneType::NONE;
};

