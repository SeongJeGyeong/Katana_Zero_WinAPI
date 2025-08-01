#pragma once
#include "Scene.h"

class Actor;
class TileRenderer;
class Texture;

class EditorScene : public Scene
{
	using Super = Scene;

private:
	HWND	_hwnd;		// ������ �ڵ�
	HWND	_hwndSub;	// ���� ������â �ڵ�

	class EditorSub* _subWindow = nullptr;

	int32 iGridMaxX;
	int32 iGridMaxY;

	int32 iEditorMapSizeX;
	int32 iEditorMapSizeY;

	// vector�� �ε��� : ������ �� �׸��� �ε���
	// Vector2 : Ÿ�ϸ� �ؽ�ó�� Ÿ�� ��ǥ
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

