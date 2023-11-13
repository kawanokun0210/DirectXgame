#include "GameClearScene.h"

void GameClearScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
}

void GameClearScene::Update() {
	time++;

	if (time >= 60) {
		sceneNo = TITLE;
	}
}

void GameClearScene::Draw() {

}

void GameClearScene::Finalize() {

}