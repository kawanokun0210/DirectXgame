#include "GameClearScene.h"

void GameClearScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	input_ = Input::GetInstance();
	input_->Initialize();

}

void GameClearScene::Update() {
	
	input_->Update();

	if (input_->PushKey(DIK_RETURN)) {
		sceneNo = TITLE;
	}

}

void GameClearScene::Draw() {

}

void GameClearScene::Finalize() {

}