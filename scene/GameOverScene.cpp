#include "GameOverScene.h"

void GameOverScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
}

void GameOverScene::Update() {
	time++;

	if (time >= 60) {
		sceneNo = TITLE;
	}
}

void GameOverScene::Draw() {

}

void GameOverScene::Finalize() {

}