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

	bullet.translate.z -= 1.0f;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void EnemyBullet::SetBullet(Transform enemy) {
	bullet.translate.x = enemy.translate.x;
	bullet.translate.y = enemy.translate.y;
	bullet.translate.z = enemy.translate.z;
}

void EnemyBullet::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, bullet, 21, camera, directionalLight, true);

}

void EnemyBullet::Finalize() {
	delete object_;
}