#include "Enemy.h"

Enemy::~Enemy() {
	delete object_;
}

void Enemy::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	object_ = new Object();

	material = { 1.0f,1.0f,1.0f,1.0f };

	object_->Initialize(dxCommon_, engine_, "Resource/", "player.obj");

	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> enemyTranslateX(-11.8f, 11.8f);
	std::uniform_real_distribution<float> enemyTranslateY(-6.2f, 6.2f);
	std::uniform_real_distribution<float> enemyTranslateZ(40.0f, 60.0f);

	enemy = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{enemyTranslateX(randomEngine),enemyTranslateY(randomEngine),enemyTranslateZ(randomEngine)} };

	isAlive_ = false;
}

void Enemy::Update() {

	enemy.translate.z -= 0.2f;

	if (enemy.translate.z <= 0.0f) {
		material.w -= 0.005f;
	}

	if (material.w <= 0.0f) {
		isDead_ = true;
	}

}

void Enemy::Draw(Transform camera, DirectionalLight directionalLight) {

	object_->Draw(material, enemy, 2, camera, directionalLight, true);

}

bool Enemy::SetAlive(bool a) {
	isAlive_ = a;
	return isAlive_;
}

void Enemy::Finalize() {
	delete object_;
}