#pragma once
#include "Scene.h"

class Player;
class Tile_FG;

class GameScene : public Scene
{
	using Super = Scene;

public:
	GameScene(string mapFileName);

private:
	Player* _player = nullptr;
	vector<Tile_FG*> _tileList;

	void LoadTileActors(json tileData);

public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltaTime) override;
	virtual void PostUpdate(float deltatTime) override;
	virtual void Render(HDC hdc) override;
};