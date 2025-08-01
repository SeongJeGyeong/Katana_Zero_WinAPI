#include "pch.h"
#include "Texture.h"
#include "../Game/Game.h"

void Texture::Load(wstring filePath, int32 alpha)
{
	// WinAPI 텍스처 로딩하는 방법
	HDC hdc = ::GetDC(Game::GetInstance()->GetHwnd());

	_textureHdc = ::CreateCompatibleDC(hdc);
	_bitmap = (HBITMAP)::LoadImageW(
		nullptr,
		filePath.c_str(),
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION
	);
	if (_bitmap == 0)
	{
		::MessageBox(Game::GetInstance()->GetHwnd(), filePath.c_str(), L"Invalid Texture Load", MB_OK);
		return;
	}

	HBITMAP prev = (HBITMAP)::SelectObject(_textureHdc, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);

	iSizeX = bit.bmWidth;
	iSizeY = bit.bmHeight;
	iTileSizeX = iSizeX;
	iTileSizeY = iSizeY;
	iAlpha = alpha;

	// 좌우 반전된 hdc도 하나 저장
	_flippedTextureHdc = ::CreateCompatibleDC(_textureHdc);
	HBITMAP flippedBitmap = ::CreateCompatibleBitmap(_textureHdc, iSizeX, iSizeY);
	(HBITMAP)::SelectObject(_flippedTextureHdc, flippedBitmap);

	StretchBlt(
		_flippedTextureHdc,
		0, 0, iSizeX, iSizeY,
		_textureHdc,
		iSizeX - 1, 0, -iSizeX, iSizeY,
		SRCCOPY
	);
}

void Texture::LoadSprite(wstring filePath, int32 tileSizeX, int32 tileSizeY, int32 frameCount, float duration, bool loop, int32 alpha)
{
	Load(filePath, alpha);

	iTileSizeX = tileSizeX;
	iTileSizeY = tileSizeY;
	iFrameCount = frameCount;
	fDuration = duration;
	bLoop = loop;
}

void Texture::Render(HDC hdc, Vector2 pos)
{
	BLENDFUNCTION op;
	op.BlendOp = AC_SRC_OVER;
	op.BlendFlags = 0;
	op.SourceConstantAlpha = iAlpha;
	op.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(hdc,
		(int32)pos.x - (iSizeX * 0.5f),
		(int32)pos.y - (iSizeY * 0.5f),
		iSizeX,
		iSizeY,
		_textureHdc,
		(int32)0,
		(int32)0,
		iSizeX,
		iSizeY,
		op);
}

void Texture::RenderStretched(HDC hdc, Vector2 pos, int32 stretchWidth, int32 stretchHeight)
{
	BLENDFUNCTION op;
	op.BlendOp = AC_SRC_OVER;
	op.BlendFlags = 0;
	op.SourceConstantAlpha = iAlpha;
	op.AlphaFormat = AC_SRC_ALPHA;

	if (bIsFlipped)
	{
		AlphaBlend(hdc,
			(int32)pos.x - (stretchWidth * 0.5f),
			(int32)pos.y - (stretchHeight * 0.5f),
			stretchWidth,
			stretchHeight,
			_flippedTextureHdc,
			(int32)0,
			(int32)0,
			iSizeX,
			iSizeY,
			op);
	}
	else
	{
		AlphaBlend(hdc,
			(int32)pos.x - (stretchWidth * 0.5f),
			(int32)pos.y - (stretchHeight * 0.5f),
			stretchWidth,
			stretchHeight,
			_textureHdc,
			(int32)0,
			(int32)0,
			iSizeX,
			iSizeY,
			op);
	}
}

void Texture::RenderSprite(HDC hdc, Vector2 pos)
{
	BLENDFUNCTION op;
	op.BlendOp = AC_SRC_OVER;
	op.BlendFlags = 0;
	op.SourceConstantAlpha = iAlpha;
	op.AlphaFormat = AC_SRC_ALPHA;

	if (bIsFlipped)
	{
		AlphaBlend(hdc,
			(int32)pos.x - (iTileSizeX * 0.5f),
			(int32)pos.y - (iTileSizeY * 0.5f),
			iTileSizeX,
			iTileSizeY,
			_flippedTextureHdc,
			iCurFrame * iTileSizeX,
			0,
			iTileSizeX,
			iTileSizeY,
			op);
	}
	else
	{
		AlphaBlend(hdc,
			(int32)pos.x - (iTileSizeX * 0.5f),
			(int32)pos.y - (iTileSizeY * 0.5f),
			iTileSizeX,
			iTileSizeY,
			_textureHdc,
			iCurFrame * iTileSizeX,
			0,
			iTileSizeX,
			iTileSizeY,
			op);
	}
}

void Texture::RenderStretchedSprite(HDC hdc, Vector2 pos, int32 magnification)
{
	BLENDFUNCTION op;
	op.BlendOp = AC_SRC_OVER;
	op.BlendFlags = 0;
	op.SourceConstantAlpha = iAlpha;
	op.AlphaFormat = AC_SRC_ALPHA;

	int32 stretchedWidth = iTileSizeX * magnification;
	int32 stretchedHeight = iTileSizeY * magnification;

	if (bIsFlipped)
	{
		AlphaBlend(hdc,
			(int32)pos.x - (stretchedWidth * 0.5f),
			(int32)pos.y - (stretchedHeight * 0.5f),
			stretchedWidth,
			stretchedHeight,
			_flippedTextureHdc,
			(iFrameCount - iCurFrame - 1) * iTileSizeX,
			0,
			iTileSizeX,
			iTileSizeY,
			op);
	}
	else
	{
		AlphaBlend(hdc,
			(int32)pos.x - (stretchedWidth * 0.5f),
			(int32)pos.y - (stretchedHeight * 0.5f),
			stretchedWidth,
			stretchedHeight,
			_textureHdc,
			iCurFrame * iTileSizeX,
			0,
			iTileSizeX,
			iTileSizeY,
			op);
	}
}

void Texture::RenderSprite(HDC hdc, Vector2 pos, int32 tileSizeX, int32 tileSizeY, int32 originX, int32 originY)
{
	BLENDFUNCTION op;
	op.BlendOp = AC_SRC_OVER;
	op.BlendFlags = 0;
	op.SourceConstantAlpha = iAlpha;
	op.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(hdc,
		(int32)pos.x - (tileSizeX * 0.5f),
		(int32)pos.y - (tileSizeY * 0.5f),
		tileSizeX,
		tileSizeY,
		_textureHdc,
		originX,
		originY,
		tileSizeX,
		tileSizeY,
		op);
}