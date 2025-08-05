#include "pch.h"
#include "GameScene.h"
#include "../Objects/Actors/Player.h"
#include "../Objects/Actors/Tile_FG.h"

GameScene::GameScene(string mapFileName)
{
	fs::path directory = fs::current_path() / L"../GameResources/Json/";
	fs::path path = directory / mapFileName;
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		MessageBox(nullptr, L"Failed to open JSON file", L"Error", MB_OK);
		return;
	}

	json data = json::parse(file);
	LoadTileActors(data);

	file.close();
}

void GameScene::LoadTileActors(json tileData)
{
	for (auto data : tileData["Background"])
	{
		Vector2 screenPos = { (float)data[1] * TILE_SIZE + TILE_SIZE / 2, (float)data[2] * TILE_SIZE + TILE_SIZE / 2 };

		Tile_FG* tile = new Tile_FG();
		tile->Init({ screenPos.x, screenPos.y }, { data[3], data[4] }, data[0]);
		_tileList.push_back(tile);
	}

	for (auto data : tileData["Foreground"])
	{
		Vector2 screenPos = { (float)data[1] * TILE_SIZE + TILE_SIZE / 2, (float)data[2] * TILE_SIZE + TILE_SIZE / 2 };

		Tile_FG* tile = new Tile_FG();
		tile->Init({ screenPos.x, screenPos.y }, { data[3], data[4] }, data[0]);
		tile->CreateAABBCollider(32, 32, { 0, 0 }, CollisionLayer::TILE_FG);
		_tileList.push_back(tile);
	}

	for (auto data : tileData["Actor"])
	{
		Vector2 screenPos = { (float)data[1] * TILE_SIZE + TILE_SIZE / 2, (float)data[2] * TILE_SIZE + TILE_SIZE / 2 };

		Tile_FG* tile = new Tile_FG();
		tile->Init({ screenPos.x, screenPos.y }, { data[3], data[4] }, data[0]);
		_tileList.push_back(tile);
	}
}

void GameScene::Init()
{
	Super::Init();

	_player = new Player();
	_player->Init({ GWinSizeX / 2, GWinSizeY / 2 });
}

void GameScene::Destroy()
{
	Super::Destroy();
}

void GameScene::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (_player)
	{
		_player->Update(deltaTime);
	}

	for (auto tile : _tileList)
	{
		tile->Update(deltaTime);
	}
}

void GameScene::PostUpdate(float deltaTime)
{
	Super::PostUpdate(deltaTime);
}

void GameScene::Render(HDC hdc)
{
	Super::Render(hdc);

	for (auto tile : _tileList)
	{
		tile->Render(hdc);
	}

	if (_player)
	{
		_player->Render(hdc);
	}
}