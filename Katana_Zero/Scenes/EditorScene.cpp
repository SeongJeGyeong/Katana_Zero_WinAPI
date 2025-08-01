#include "pch.h"
#include "EditorScene.h"
#include "../Managers/InputManager.h"
#include "../Game/Game.h"
#include "EditorSub.h"
#include "../Resources/Texture.h"
#include "../Managers/ResourceManager.h"

void EditorScene::Init()
{
	Super::Init();
	_sceneType = SceneType::EDITOR;

	iGridMaxX = GWinSizeX / TILE_SIZE;
	iGridMaxY = GWinSizeY / TILE_SIZE;

	_subWindow = new EditorSub();
	_subWindow->Init();

	_hwnd = Game::GetInstance()->GetHwnd();
	_hwndSub = Game::GetInstance()->GetHwndSub();
	ShowWindow(_hwndSub, 10);
	UpdateWindow(_hwndSub);

	iEditorMapSizeX = 3840;
	iEditorMapSizeY = 2160;

	_tileMapList = ResourceManager::GetInstance()->GetTileMapList();
}

void EditorScene::Destroy()
{
	if (_subWindow)
	{
		delete _subWindow;
		_subWindow = nullptr;
	}

	Super::Destroy();
}

void EditorScene::Update(float deltatTime)
{
	HWND hwnd = ::GetForegroundWindow();
	if (hwnd == _hwndSub && _subWindow)
	{
		_subWindow->Update();
		return;
	}

	Super::Update(deltatTime);

	if (InputManager::GetInstance()->GetButtonUp(KeyType::ESC))
	{
		Game::GetInstance()->ChangeLobbyScene();
		ShowWindow(Game::GetInstance()->GetHwndSub(), SW_HIDE);
	}

	if (InputManager::GetInstance()->GetButtonPressed(KeyType::LeftMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		// 클릭한 그리드 위치 계산
		int32 x = mousePos.x / TILE_SIZE;
		int32 y = mousePos.y / TILE_SIZE;
		int32 index = ConvertGridToIndex({ (float)x, (float)y });

		_tileInfoMap[_subWindow->GetSelectedLayer()][index] = { _subWindow->GetSelectedTileSet(), _subWindow->GetSelectedTexturePos() };
	}
	else if (InputManager::GetInstance()->GetButtonPressed(KeyType::RightMouse))
	{
		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		// 클릭한 그리드 위치 계산
		int32 x = mousePos.x / TILE_SIZE;
		int32 y = mousePos.y / TILE_SIZE;
		int32 index = ConvertGridToIndex({ (float)x, (float)y });
		_tileInfoMap[_subWindow->GetSelectedLayer()][index] = { -1, {-1, -1} };
	}

	if (InputManager::GetInstance()->GetButtonUp(KeyType::F1))
	{
		SaveMap();
	}
	if (InputManager::GetInstance()->GetButtonUp(KeyType::F2))
	{
		LoadMap();
	}
}

void EditorScene::Render(HDC hdc)
{
	Super::Render(hdc);

	for (int y = 0; y <= iGridMaxY; y++)
	{
		MoveToEx(hdc, 0, y * TILE_SIZE, nullptr);
		LineTo(hdc, GWinSizeX, y * TILE_SIZE);
	}

	for (int x = 0; x <= iGridMaxX; x++)
	{
		MoveToEx(hdc, x * TILE_SIZE, 0, nullptr);
		LineTo(hdc, x * TILE_SIZE, GWinSizeY);
	}

	for (int i = 0; i < RenderLayer::LAYER_END; ++i)
	{
		for (auto it : _tileInfoMap[i])
		{
			if (it.second.vTilePos == Vector2(-1, -1)) continue;

			Vector2 gridPos = ConvertIndexToGrid(it.first);
			Vector2 screenPos = { gridPos.x * TILE_SIZE + TILE_SIZE/2, gridPos.y * TILE_SIZE + TILE_SIZE / 2 };

			_tileMapList[it.second.iTilesetIndex]->RenderSprite(hdc, screenPos, TILE_SIZE, TILE_SIZE, it.second.vTilePos.x * TILE_SIZE, it.second.vTilePos.y * TILE_SIZE);
		}
	}

	if (_subWindow)_subWindow->Render();		
}

void EditorScene::SaveMap()
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"JSON (*.json)\0*.json\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"tilemap";

	//// 파일 이름이 선택되었으면 저장
	if (GetSaveFileName(&ofn)) 
	{
		json jsonfile;
		std::ofstream file(szFileName);

		if (file.is_open()) 
		{
			// 타일 데이터 저장
			for (int32 i = 0; i < RenderLayer::LAYER_END; i++)
			{
				if (_tileInfoMap[i].empty()) continue;

				json TilesByLayer;
				for (auto it : _tileInfoMap[i])
				{
					Vector2 pos = ConvertIndexToGrid(it.first);
					TilesByLayer[to_string(it.first)].push_back(it.second.iTilesetIndex);
					TilesByLayer[to_string(it.first)].push_back(pos.x);
					TilesByLayer[to_string(it.first)].push_back(pos.y);
					TilesByLayer[to_string(it.first)].push_back(it.second.vTilePos.x);
					TilesByLayer[to_string(it.first)].push_back(it.second.vTilePos.y);
				}

				string tileType;

				switch ((RenderLayer)i)
				{
				case RenderLayer::BACKGROUND:
					tileType = "Background";
					break;
				case RenderLayer::FOREGROUND:
					tileType = "Foreground";
					break;
				case RenderLayer::ACTOR:
					tileType = "Actor";
					break;
				default:
					break;
				}
				jsonfile[tileType] = TilesByLayer;
			}

			file << std::setw(4) << jsonfile;
			file.close();
			MessageBox(_hwnd, L"타일맵이 저장되었습니다.", L"저장 완료", MB_OK | MB_ICONINFORMATION);
		}
		else 
		{
			MessageBox(_hwnd, L"파일을 저장할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}

void EditorScene::LoadMap()
{
	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = _hwnd;
	ofn.lpstrFilter = L"JSON (*.json)\0*.json\0모든 파일 (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"tilemap";

	// 파일 이름이 선택되었으면 로드
	if (GetOpenFileName(&ofn)) {
		std::wstring wFileName = szFileName;
		std::wstring fileName(wFileName.begin(), wFileName.end());

		std::ifstream file(fileName);
		if (file.is_open()) 
		{
			json mapData = json::parse(file);

			for (int32 type = 0; type < RenderLayer::LAYER_END; ++type)
			{
				_tileInfoMap[type].clear();
				string tileType;

				switch ((RenderLayer)type)
				{
				case RenderLayer::BACKGROUND:
					tileType = "Background";
					break;
				case RenderLayer::FOREGROUND:
					tileType = "Foreground";
					break;
				case RenderLayer::ACTOR:
					tileType = "Actor";
					break;
				default:
					break;
				}

				for (auto data : mapData[tileType])
				{
					int32 index = ConvertGridToIndex({ data[1], data[2] });
					_tileInfoMap[type][index] = { data[0], {data[3], data[4]} };
				}
			}

			file.close();
			MessageBox(_hwnd, L"타일맵이 로드되었습니다.", L"로드 완료", MB_OK | MB_ICONINFORMATION);
		}
		else {
			MessageBox(_hwnd, L"파일을 로드할 수 없습니다.", L"오류", MB_OK | MB_ICONERROR);
		}
	}
}
