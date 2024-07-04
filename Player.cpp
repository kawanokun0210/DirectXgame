#include "Player.h"

void Player::Initialize() {

	input_ = Input::GetInstance();
	input_->Initialize();

	model_ = new Object();
	model_->Initialize("Resource/fence", "fence.obj");

	transform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{-2.0f,0.0f,0.0f} };
	model_->SetTransform(transform_);

	//ジャンプ用
	acceleration_ = -0.02f;
	isJump_ = false;

}

void Player::Update() {
	input_->Update();

	//移動処理
	Move();

	//ジャンプ処理
	Jump();

}

void Player::Draw(Camera* camera, DirectionalLight directionalLight) {

	model_->Draw(3, camera, directionalLight, true);

}

void Player::Finalize() {
	delete model_;
}

//ここから細かい関数
//移動
void Player::Move() {

	if (input_->PushKey(DIK_D)) {
		model_->transform_.translate.x += speed_;
	}

	if (input_->PushKey(DIK_A)) {
		model_->transform_.translate.x -= speed_;
	}

}

//ジャンプ
void Player::Jump() {

	if (input_->PushKey(DIK_SPACE) && isJump_ == false) {
		isJump_ = true;
		fallSpeed_ = 0.38f;
	}

	if (isJump_ == true) {
		fallSpeed_ += acceleration_;
		model_->transform_.translate.y += fallSpeed_;
	}

}