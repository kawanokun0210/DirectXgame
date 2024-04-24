#include "PlayerBullet.h"

PlayerBullet::~PlayerBullet() {
	delete object_;
}

void PlayerBullet::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	object_ = new Object();
	object_->Initialize(dxCommon_, engine_, "Resource/", "player.obj");

	material = { 1.0f,1.0f,1.0f,1.0f };

	bullet.scale_ = { 1.0f,1.0f,1.0f };
	bullet.translation_ = {0.0f,0.0f,0.0f};

}

void PlayerBullet::Update() {

	bullet.translation_.z += 1.0f;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::SetBullet(Transform player) {
	bullet.translation_.x = player.translation_.x;
	bullet.translation_.y = player.translation_.y;
	bullet.translation_.z = player.translation_.z;
}

void PlayerBullet::Draw(Camera* camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void PlayerBullet::Finalize() {
	delete object_;
}