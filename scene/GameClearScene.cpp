#include "GameClearScene.h"

void GameClearScene::Initialize() {

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