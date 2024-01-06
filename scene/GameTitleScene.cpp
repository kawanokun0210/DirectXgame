#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	textureManager_ = new TextureManager();
	textureManager_->Initialize(engine_);

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	spriteData_.LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[0] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };

	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	sprite_ = new Sprite();
	sprite_->Initialize(dxCommon_, engine_);

}

void GameTitleScene::Update() {
	input_->Update();

	if (input_->PushKey(DIK_RETURN)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {
	for (int i = 0; i < 1; i++)
	{
		sprite_->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, 3, directionalLight_);
	}
}

void GameTitleScene::Finalize() {
	delete sprite_;
	delete textureManager_;
}