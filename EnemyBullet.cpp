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

	bullet = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

}

void EnemyBullet::Update() {

	bullet.translate.x -= speed_.x;
	bullet.translate.y -= speed_.y;
	bullet.translate.z -= speed_.z;

	if (bullet.translate.z <= 0.0f) {
		material.w -= 0.05f;
	}

	if (material.w <= 0.000f) {
		isDead_ = true;
	}

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::SetSpeed(Vector3 speed) {
	speed_ = speed;
}

void EnemyBullet::SetBullet(Vector3 enemy) {
	bullet.translate.x = enemy.x;
	bullet.translate.y = enemy.y;
	bullet.translate.z = enemy.z;
}

void EnemyBullet::Draw(Camera* camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void EnemyBullet::Finalize() {
	delete object_;
}