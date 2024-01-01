#include "GamePlayScene.h"

void GamePlayScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	textureManager_ = new TextureManager();
	textureManager_->Initialize(engine_);

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-30.0f} };

	player_ = new Player();
	player_->Initialize(engine_, dxCommon_);

	skydome_ = new SkyDome();
	skydome_->Initialize(engine_, dxCommon_);

}

void GamePlayScene::Update()
{
	player_->Update();
	skydome_->Update();
	EnemySporn();

	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
		});

	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}
}

void GamePlayScene::Draw()
{
	player_->Draw(cameraTransform_, directionalLight_);
	skydome_->Draw(cameraTransform_, directionalLight_);
	for (Enemy* enemy : enemy_) {
		enemy->Draw(cameraTransform_, directionalLight_);
	}
}

void GamePlayScene::EnemySporn() {

	if (enemyCount <= 2) {

		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(engine_, dxCommon_);

		enemy_.push_back(newEnemy);

		enemyCount++;
	}
	else if(enemyCount > 2){
		enemySpornTimer++;
		if (enemySpornTimer >= 180) {
			enemyCount = 0;
			enemySpornTimer = 0;
		}
	}

}

void GamePlayScene::Finalize()
{
	delete textureManager_;
	player_->Finalize();
	skydome_->Finalize();
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
}