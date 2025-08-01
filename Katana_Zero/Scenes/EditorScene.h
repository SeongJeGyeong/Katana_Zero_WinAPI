#pragma once
#include "Scene.h"

class Actor;
class TileRenderer;
class Texture;

class EditorScene : public Scene
{
	using Super = Scene;

private:
	HWND	_hwnd;		// 윈도우 핸들
	HWND	_hwndSub;	// 서브 에디터창 핸들

	class EditorSub* _subWindow = nullptr;

	int32 iGridMaxX;
	int32 iGridMaxY;

	int32 iEditorMapSizeX;
	int32 iEditorMapSizeY;

	// vector의 인덱스 : 에디터 씬 그리드 인덱스
	// Vector2 : 타일맵 텍스처의 타일 좌표
	unordered_map<int32, TileInfo>_tileInfoMap[RenderLayer::LAYER_END];
	vector<Texture*> _tileMapList;

public:
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update(float deltatTime) override;
	virtual void Render(HDC hdc) override;

	// y * width + x
	int32 ConvertGridToIndex(Vector2 pos) { return (int32)pos.y * iGridMaxX + pos.x; }
	// x % width, y / width
	Vector2 ConvertIndexToGrid(int32 idx) 
	{
		float x = idx % iGridMaxX;
		float y = idx / iGridMaxX;
		return { x, y };
	}

	void SaveMap();
	void LoadMap();

protected:
	//virtual void loadResource();
};

