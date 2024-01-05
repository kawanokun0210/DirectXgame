#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	time = 0;

}

void GameTitleScene::Update() {
	input_->Update();

	if (input_->PushKey(DIK_RETURN)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}