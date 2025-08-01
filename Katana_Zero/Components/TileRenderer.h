#pragma once
#include "Component.h"

class Texture;

class TileRenderer : public Component
{
private:
	int32 iPosX;
	int32 iPosY;

	Texture* _tileMap;

public:
	virtual void InitComponent(int32 x, int32 y);
	virtual void UpdateComponent(float deltaTime) override;
	virtual void RenderComponent(HDC hdc) override;

	void SetTileMap(Texture* texture);
	void SetTilePos(Vector2 pos) { iPosX = pos.x; iPosY = pos.y; }
	void SetTilePos(int32 posX, int32 posY) { iPosX = posX; iPosY = posY; }
};

