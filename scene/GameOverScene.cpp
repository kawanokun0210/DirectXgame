#include "GameOverScene.h"

void GameOverScene::Initialize() {
	
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