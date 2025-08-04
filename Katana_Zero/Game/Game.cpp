#include "pch.h"
#include "Game.h"
#include "../Managers/InputManager.h"
#include "../Managers/TimeManager.h"
#include "../Managers/ResourceManager.h"
#include "../Scenes/Scene.h"
#include "../Scenes/LobbyScene.h"
#include "../Scenes/EditorScene.h"
#include "../Scenes/EditorSub.h"
#include "../Scenes/GameScene.h"
#include "../Managers/CollisionManager.h"

void Game::Init(HWND hwnd, HWND hsubwnd)
{
	_hwnd = hwnd;
	_hwndSub = hsubwnd;
	_hdc = ::GetDC(hwnd);	// 기본 도화지 넘겨받기

	// 더블 버퍼링 버퍼 추가
	::GetClientRect(hwnd, &_rect);

	_hdcBack = ::CreateCompatibleDC(_hdc); // 기본 hdc와 호환되는 DC를 생성

	// 이게 추가로 필요한 이유는 생성된 호환 hdc는 우리가 원하는 크기의 버퍼가 아니라 아주 작은
	// 버퍼이다. 우리는 우리 게임의 전체 정보를 그릴 도화지가 필요한거라
	// 별도의 비트맵을 만들어서, 여기다가 정보를 그릴것이다.
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // hdc와 호환되는 비트맵 생성

	// 여기다가 생성된 백버퍼 HDC와 게임사이즈에 맞는 텍스쳐를 연결한다.
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC와 BMP를 연결
	::DeleteObject(prev);

	// 외부 폰트를 시스템에 임시로 등록
	fs::path directory = fs::current_path() / L"../GameResources";
	AddFontResourceEx((directory / L"Fonts/DungGeunMo.ttf").c_str(), FR_PRIVATE, 0);

	_font = CreateFont(
		(int32)16, 0, 0, 0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"둥근모꼴"
	);

	// 게임과 관련된 초기화가 필요한 항목들은 여기서
	InputManager::GetInstance()->Init(_hwnd, _hwndSub);	// 객체를 생성
	TimeManager::GetInstance()->Init();
	ResourceManager::GetInstance()->Init(_hwnd, directory);
	CollisionManager::GetInstance()->Init();

	_currScene = new LobbyScene();
	_currScene->Init();
}

void Game::Destroy()
{
	InputManager::DestroyInstance();
	TimeManager::DestroyInstance();
	ResourceManager::DestroyInstance();
	CollisionManager::DestroyInstance();

	if (_currScene)
	{
		_currScene->Destroy();
	}
	SAFE_DELETE(_currScene);
}

void Game::Update()
{
	// CurScene 변경 요청이 있으면, 현재 씬을 변경먼저 하고, 해당 씬 업데이트를 수행한다.
	if (_nextScene)
	{
		if (_currScene)
		{
			_currScene->Destroy();
			delete _currScene;
			_currScene = nullptr;
		}

		_currScene = _nextScene;
		_currScene->Init();

		_nextScene = nullptr;
	}

	InputManager::GetInstance()->Update();
	TimeManager::GetInstance()->Update();

	if (_currScene)
	{
		_currScene->Update(TimeManager::GetInstance()->GetDeltaTime());
		_currScene->PostUpdate(TimeManager::GetInstance()->GetDeltaTime());

		CollisionManager::GetInstance()->Update();
	}

	if (InputManager::GetInstance()->GetButtonDown(KeyType::F1))
	{
		bDebugMode = !bDebugMode;
	}
}

void Game::Render()
{
	if (_currScene)
	{
		_currScene->Render(_hdcBack);
	}

	uint32 fps = TimeManager::GetInstance()->GetFps();
	float deltaTime = TimeManager::GetInstance()->GetDeltaTime();
	SetTextColor(_hdcBack, RGB(0, 0, 0));
	{

		POINT mousePos = InputManager::GetInstance()->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 300, 10, str.c_str(), static_cast<int32>(str.size()));
	}

	{
		wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		::TextOut(_hdcBack, 5, 10, str.c_str(), static_cast<int32>(str.size()));
	}


	// 여분 도화지에 렌더링 끝.
	// 한번 그림을 다 그렸으니, 이제는 프론트 버퍼에 복사.
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사

	// 프론트 버퍼에 복사가 끝났으면, 백버퍼는 초기화
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, _background);
}

void Game::ChangeGameScene()
{
	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	_nextScene = new GameScene("test2.json");
	_background = WHITENESS;
}

void Game::ChangeLobbyScene()
{
	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	_nextScene = new LobbyScene();
	_background = BLACKNESS;
}

void Game::ChangeEditorScene()
{
	if (_nextScene)
	{
		delete _nextScene;
		_nextScene = nullptr;
	}

	_nextScene = new EditorScene();
	_background = WHITENESS;
}

void Game::ExitGame()
{
	DestroyWindow(_hwndSub);
	DestroyWindow(_hwnd);
}
