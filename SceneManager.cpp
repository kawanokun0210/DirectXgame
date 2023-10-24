#include "SceneManager.h"

void SceneManager::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	engine_ = engine;
	dxCommon_ = dxCommon;

	gameScene->Initialize(engine_, dxCommon_);
	titleState->Initialize(engine_, dxCommon_);
}

//ここからタイトルシーン
void SceneManager::TitleUpdate() {
	titleState->Update();
}

void SceneManager::TitleDraw() {
	titleState->Draw();
}

//ここからゲームシーン
void SceneManager::GameUpdate() {
	gameScene->Update();
}

void SceneManager::GameDraw() {
	gameScene->Draw();
}


//全解放処理
void SceneManager::Finalize() {
	gameScene->Finalize();
	titleState->Finalize();

	delete input_;
	delete gameScene;
	delete titleState;
}