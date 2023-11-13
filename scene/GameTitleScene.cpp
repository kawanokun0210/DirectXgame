#include "GameTitleScene.h"

void TitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
}

void TitleScene::Update() {
	time++;

	if (time >= 60) {
		sceneNo = PLAY;
	}
}

void TitleScene::Draw() {

}

void TitleScene::Finalize() {

}