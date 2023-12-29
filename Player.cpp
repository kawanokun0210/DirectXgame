#include "Player.h"

void Player::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	object_ = new Object();
	input_ = Input::GetInstance();
	input_->Initialize();

	material = { 1.0f,1.0f,1.0f,1.0f };

	object_->Initialize(dxCommon_,engine_,"Resource/","player.obj");

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	player = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Player::Update() {
	input_->Update();

	//左右の移動
	if (input_->PushKey(DIK_A)) {
		player.translate.x -= 0.1f;
	}
	else if (input_->PushKey(DIK_D)) {
		player.translate.x += 0.1f;
	}

	//上下の移動
	if (input_->PushKey(DIK_S)) {
		player.translate.y -= 0.1f;
	}
	else if (input_->PushKey(DIK_W)) {
		player.translate.y += 0.1f;
	}

	//左右移動の限界
	if (player.translate.x <= -11.8f) {
		player.translate.x += 0.1f;
	}
	else if (player.translate.x >= 11.8f) {
		player.translate.x -= 0.1f;
	}

	//上下移動の限界
	if (player.translate.y <= -6.2f) {
		player.translate.y += 0.1f;
	}
	else if (player.translate.y >= 6.2f) {
		player.translate.y -= 0.1f;
	}

}

void Player::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, player, 2, camera, directionalLight, true);

}

void Player::Finalize() {
	delete object_;
}