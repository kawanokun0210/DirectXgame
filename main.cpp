#include "Engine.h"
#include "SceneManager.h"
#include "TextureManager.h"

const wchar_t kWindowTitle[] = { L"CG2_WinMain" };

//Windowsアプリでのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	D3DResourceLeakChecker leakCheck;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> device;

	//COM初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//初期化
	MyEngine* engine = new MyEngine;
	engine->Initialize(kWindowTitle, 1280, 720);

	SceneManager* sceneManager = new SceneManager();
	sceneManager->Initialize(engine, engine->GetDirectXCommon());

	TextureManager* textureManager = new TextureManager();
	textureManager->Initialize(engine);

	while (true)
	{
		//windowのメッセージを最優先で処理させる
		if (WinApp::GetInstance()->Procesmessage())
		{
			break;
		}

		//ゲームの処理
		engine->BeginFrame();

		switch (sceneManager->GetScene())
		{
		case TITLE:

			sceneManager->TitleUpdate();

			sceneManager->TitleDraw();

			break;

		case PLAY:

			sceneManager->GameUpdate();

			sceneManager->GameDraw();

			break;

		case CLEAR:

			sceneManager->ClearUpdate();

			sceneManager->ClearDraw();

			break;

		case OVER:

			sceneManager->GameOverUpdate();

			sceneManager->GameOverDraw();

			break;

		}
		engine->EndFrame();
	}

	//解放処理

	engine->Finalize();

	sceneManager->Finalize();

	textureManager->Finalize();

	delete engine;
	delete sceneManager;
	delete textureManager;

	CoUninitialize();

	return 0;
}