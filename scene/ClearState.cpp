#include "ClearState.h"

void ClearState::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	engine_ = engine;
	dxCommon_ = dxCommon;

	for (int i = 0; i < 1; i++) {
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

	spriteData_.LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[0] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.RightDown[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };

	cubeResourceNum_ = 3;

}

void ClearState::Update() {
	directionalLight_.direction = Normalise(directionalLight_.direction);
}

void ClearState::Draw() {
	for (int i = 0; i < 1; i++) {
		sprite_[i]->Draw(spriteData_.LeftTop[i], spriteData_.RightDown[i], spriteTransform_, spriteData_.material, cubeResourceNum_, directionalLight_);
	}
}

void ClearState::Finalize() {
	for (int i = 0; i < 1; i++) {
		delete sprite_[i];
	}
}
