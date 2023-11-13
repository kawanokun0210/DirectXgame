#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
}

void GameTitleScene::Update() {
	time++;

	if (time >= 60) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}