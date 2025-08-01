#pragma once

class Texture
{
private:
	// �ؽ�ó�� �׸������� ����
	HDC		_textureHdc = 0;
	HDC		_flippedTextureHdc = 0;
	HBITMAP _bitmap;
	int32 iAlpha = 255;	// �����ϰ� �������ϴ� ����
	int32 iSizeX = 0;
	int32 iSizeY = 0;

	bool bIsSprite = false;
	bool bIsFlipped = false;

	// ��������Ʈ
private:
	int32 iTileSizeX;
	int32 iTileSizeY;
	int32 iFrameCount = 0;
	int32 iCurFrame = 0;
	float fDuration = 0;
	bool bLoop = false;

public:
	void Load(wstring filePath, int32 alpha = 255);
	void LoadSprite(wstring filePath, int32 tileSizeX, int32 tileSizeY, int32 frameCount, float duration, bool loop, int32 alpha = 255);

	// �ش� ���ҽ��� �׷����� �κ�
	void Render(HDC hdc, Vector2 pos);
	void RenderStretched(HDC hdc, Vector2 pos, int32 stretchWidth, int32 stretchHeight);
	void RenderSprite(HDC hdc, Vector2 pos);
	void RenderStretchedSprite(HDC hdc, Vector2 pos, int32 magnification);
	void RenderSprite(HDC hdc, Vector2 pos, int32 tileSizeX, int32 tileSizeY, int32 originX, int32 originY);

	int32 GetTextureSizeX() { return iSizeX; }
	int32 GetTextureSizeY() { return iSizeY; }

	int32 GetAlpha() { return iAlpha; }
	void SetAlpha(int32 transparent) { iAlpha = transparent; }

	bool GetIsSprite() { return bIsSprite; }
	void SetIsSprite(bool sprite) { bIsSprite = sprite; }

	bool GetIsFlipped() { return bIsFlipped; }
	void SetIsFlipped(bool flip) { bIsFlipped = flip; }

	int32 GetCurFrameCount() { return iCurFrame; }
	void SetCurFrameCount(int32 frame) { iCurFrame = frame; }
	int32 GetMaxFrameCount() { return iFrameCount; }
	float GetDuration() { return fDuration; }

	bool IsSpriteEnd() { return iCurFrame == iFrameCount; }
	bool IsLoop() { return bLoop; }

};

