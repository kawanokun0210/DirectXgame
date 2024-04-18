#include "Enemy.h"

Enemy::~Enemy() {
	delete object_;
	delete state;
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

	enemy.translation_ = {enemyTranslateX(randomEngine),enemyTranslateY(randomEngine),enemyTranslateZ(randomEngine)};
	enemy.scale_ = { 1.0f,1.0f,1.0f };

	isAlive_ = false;

	state = new EnemyStateApproah();
	state->SetEnemy(this);
}

void Enemy::Update() {

	state->Update();

	if (enemy.translation_.z <= 0.0f) {
		material.w -= 0.005f;
	}

	if (material.w <= 0.000f) {
		isDead_ = true;
	}

}

void Enemy::Draw(Camera* camera, DirectionalLight directionalLight) {

	object_->Draw(material, enemy, 20, camera, directionalLight, true);

}

void Enemy::SetPosition(Vector3 speed) {
	enemy.translation_ = Add(enemy.translation_, speed);
}

void Enemy::ChangeState(EnemyState* newEnemyState) {
	delete state;

	state = newEnemyState;
	state->SetEnemy(this);
}

bool Enemy::SetAlive(bool a) {
	isAlive_ = a;
	return isAlive_;
}

void Enemy::Finalize() {
	delete object_;
}