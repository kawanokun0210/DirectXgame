#include "SceneManager.h"

void SceneManager::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	engine_ = engine;
	dxCommon_ = dxCommon;

	input_->Initialize();

	gameScene->Initialize(engine_, dxCommon_);
	titleState->Initialize(engine_, dxCommon_);
	clearState->Initialize(engine_, dxCommon_);
	overState->Initialize(engine_, dxCommon_);
}

//ここからタイトルシーン
void SceneManager::TitleUpdate() {
	input_->Update();
	titleState->Update();
	gameScene->SetPlayer();

	if(input_->TriggerKey(DIK_RETURN)){
		sceneState = PLAY;
	}

}

void SceneManager::TitleDraw() {
	titleState->Draw();
}

//ここからゲームシーン
void SceneManager::GameUpdate() {
	gameScene->Update();

	input_->Update();

	if (gameScene->IsCollision(gameScene->GetAABB1(),gameScene->GetAABB2())) {
		sceneState = CLEAR;
	}

	if (gameScene->GetPlayerPosY() <= -10.0f) {
		gameScene->SetPlayerPos(-2.8f);
		sceneState = OVER;
	}

}

void SceneManager::GameDraw() {
	gameScene->Draw();
}

//ここからクリアシーン
void SceneManager::ClearUpdate() {
	clearState->Update();
	input_->Update();

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneState = TITLE;
	}
}

void SceneManager::ClearDraw() {
	clearState->Draw();
}

//ここからゲームオーバー
void SceneManager::GameOverUpdate() {
	overState->Update();
	input_->Update();

	if (input_->TriggerKey(DIK_RETURN)) {
		sceneState = TITLE;
	}
}

void SceneManager::GameOverDraw() {
	overState->Draw();
}

//全解放処理
void SceneManager::Finalize() {
	gameScene->Finalize();
	titleState->Finalize();
	clearState->Finalize();
	overState->Finalize();

	delete input_;
	delete gameScene;
	delete titleState;
	delete clearState;
	delete overState;
}