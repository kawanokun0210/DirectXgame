#include "Player.h"

void Player::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	object_ = new Object();

	material = { 1.0f,1.0f,1.0f,1.0f };

	object_->Initialize(dxCommon_,engine_,"Resource/","axis.obj");

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	player = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Player::Update() {

}

void Player::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, player, 2, camera, directionalLight, true);

}

void Player::Finalize() {
	delete object_;
}