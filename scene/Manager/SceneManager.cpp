#include "SceneManager.h"

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;

	return &instance;
}

SceneManager::SceneManager() {
	engine_ = MyEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	//各シーンの配列
	sceneArr_[TITLE] = std::make_unique<GameTitleScene>();
	sceneArr_[PLAY] = std::make_unique<GamePlayScene>();
	sceneArr_[CLEAR] = std::make_unique<GameClearScene>();
	sceneArr_[OVER] = std::make_unique<GameOverScene>();

	//初期シーンの設定
	currentSceneNo_ = baseScene->GetSceneNo();

	//初期シーンの初期化処理
	sceneArr_[currentSceneNo_]->Initialize();
}

SceneManager::~SceneManager() {
	
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
			sceneArr_[currentSceneNo_]->Initialize();
			sceneArr_[prevSceneNo_]->Finalize();
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