#include "Player.h"

void Player::Initialize() {

	input_ = Input::GetInstance();
	input_->Initialize();

	model_ = new Object();
	model_->Initialize("Resource/fence", "fence.obj");

	transform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	material_ = { 1.0f,1.0f,1.0f,1.0f };

}

void Player::Update() {
	input_->Update();

	//移動処理
	Move();

}

void Player::Draw(Camera* camera, DirectionalLight directionalLight) {

	model_->Draw(material_, transform_, 3, camera, directionalLight, true);

}

void Player::Finalize() {
	delete model_;
}

//ここから細かい関数
void Player::Move() {

	if (input_->PushKey(DIK_D)) {
		transform_.translate.x += speed_;
	}

	if (input_->PushKey(DIK_A)) {
		transform_.translate.x -= speed_;
	}

}