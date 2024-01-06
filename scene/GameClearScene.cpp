#include "GameClearScene.h"

void GameClearScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	input_ = Input::GetInstance();
	input_->Initialize();

	textureManager_ = new TextureManager();
	textureManager_->Initialize(engine_);

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	spriteData_[0].LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[0].RightDown[0] = { 180.0f,180.0f,0.0f,1.0f };
	spriteData_[0].LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[0].RightDown[1] = { 180.0f,180.0f,0.0f,1.0f };
	spriteData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	spriteTransform_[0] = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
	spriteTransform_[1] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{90.0f,0.0f,0.0f} };
	spriteTransform_[2] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{180.0f,0.0f,0.0f} };
	spriteTransform_[3] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{270.0f,0.0f,0.0f} };
	spriteTransform_[4] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{360.0f,0.0f,0.0f} };

	for (int i = 0; i < 5; i++) {
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

}

void GameClearScene::Update() {

	input_->Update();

	eachScoar[5] = scoar / 1;
	boxScoar = boxScoar % 1;

	eachScoar[4] = scoar / 10;
	boxScoar = boxScoar % 10;

	eachScoar[3] = scoar / 100;
	boxScoar = boxScoar % 100;

	eachScoar[2] = scoar / 1000;
	boxScoar = boxScoar % 1000;

	eachScoar[1] = scoar / 10000;
	boxScoar = boxScoar % 10000;

	eachScoar[0] = scoar / 100000;
	boxScoar = boxScoar % 100000;

	for (int i = 0; i < 6; i++) {
		eachScoar[i] %= 10;
	}

	if (input_->PushKey(DIK_RETURN)) {
		sceneNo = TITLE;
	}

}

void GameClearScene::Draw() {

	for (int s = 0; s < 10; s++) {
		if (eachScoar[1] == s && eachScoar[1] != 0) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[0]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[0], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[2] == s && eachScoar[2] != 0) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[1]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[1], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[3] == s) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[2]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[2], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[4] == s) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[3]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[3], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
		if (eachScoar[5] == s) {
			for (int i = 0; i < 1; i++)
			{
				sprite_[4]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[4], spriteData_[0].material, s + 6, directionalLight_);
			}
		}
	}

}

void GameClearScene::Finalize() {
	for (int i = 0; i < 5; i++) {
		delete sprite_[i];
	}
	delete textureManager_;
}