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

	bullet = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

}

void PlayerBullet::Update() {

	bullet.translate.z += 1.0f;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::SetBullet(Transform player) {
	bullet.translate.x = player.translate.x;
	bullet.translate.y = player.translate.y;
	bullet.translate.z = player.translate.z;
}

void PlayerBullet::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void PlayerBullet::Finalize() {
	delete object_;
}