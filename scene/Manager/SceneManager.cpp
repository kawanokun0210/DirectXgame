#include "SceneManager.h"

SceneManager::SceneManager(const wchar_t kWindowTitle[], int32_t Hight, int32_t Right) {

	//初期化
	engine_->Initialize(kWindowTitle, Hight, Right);

	//各シーンの配列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[PLAY] = std::make_unique<GameScene>();

	//初期シーンの設定
	currentSceneNo_ = baseScene->GetSceneNo();
}

SceneManager::~SceneManager() {
	delete engine_;
	delete dxCommon_;
};

int SceneManager::Run() {
	while (true)
	{
		//windowのメッセージを最優先で処理させる
		if (WinApp::GetInstance()->Procesmessage())
		{
			break;
		}

		engine_->BeginFrame();

		//シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		//シーン要件チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Initialize(engine_, dxCommon_);
		}

		//更新処理
		sceneArr_[currentSceneNo_]->Update();

		//描画処理
		sceneArr_[currentSceneNo_]->Draw();

		engine_->EndFrame();
	}

	sceneArr_[currentSceneNo_]->Finalize();
	engine_->Finalize();

	CoUninitialize();

	return 0;
}