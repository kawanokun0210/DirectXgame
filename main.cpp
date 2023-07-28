#include "Engine.h"
#include "Triangle.h"

const wchar_t kWindowTitle[] = { L"CG2_WinMain" };

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//初期化
	WinApp* win_ = nullptr;

	MyEngine* Engine = new MyEngine;

	Engine->Initialization(win_, kWindowTitle, 1280, 720);

	Engine->VariableInitialize();

	MSG msg{};
	//ウィンドウのxボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		//Windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

			//ゲームの処理
			Engine->BeginFrame();

			Engine->Update();
			
			Engine->Draw();

			Engine->EndFrame();

		}

	}

	Engine->Finalize();

	return 0;
}
