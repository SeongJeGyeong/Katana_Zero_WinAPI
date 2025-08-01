#include "pch.h"
#include "TileRenderer.h"
#include "../Resources/Texture.h"

void TileRenderer::InitComponent(int32 x, int32 y)
{
	iPosX = x;
	iPosY = y;
}

void TileRenderer::UpdateComponent(float deltaTime)
{
}

void TileRenderer::RenderComponent(HDC hdc)
{
	_tileMap->RenderSprite(hdc, GetPos(), TILE_SIZE, TILE_SIZE, iPosX * TILE_SIZE, iPosY * TILE_SIZE);
}

void TileRenderer::SetTileMap(Texture* texture)
{
	_tileMap = texture;
}
