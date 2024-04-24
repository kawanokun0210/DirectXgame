#include "EnemyBullet.h"

EnemyBullet::~EnemyBullet() {
	delete object_;
}

void EnemyBullet::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;

	object_ = new Object();
	object_->Initialize(dxCommon_, engine_, "Resource/", "player.obj");

	material = { 1.0f,1.0f,1.0f,1.0f };

	bullet.translation_ = {0.0f,0.0f,0.0f};

}

void EnemyBullet::Update() {

	bullet.translation_.z -= 1.0f;

	if (bullet.translation_.z <= 0.0f) {
		material.w -= 0.05f;
	}

	if (material.w <= 0.000f) {
		isDead_ = true;
	}

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::SetBullet(Transform enemy) {
	bullet.translation_.x = enemy.translation_.x;
	bullet.translation_.y = enemy.translation_.y;
	bullet.translation_.z = enemy.translation_.z;
}

void EnemyBullet::Draw(Camera* camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void EnemyBullet::Finalize() {
	delete object_;
}