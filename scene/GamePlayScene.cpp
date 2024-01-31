#include "GamePlayScene.h"

void GamePlayScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	input_ = Input::GetInstance();
	input_->Initialize();

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

	particle = new Particle();
	particle->Initialize(dxCommon_, engine_, "Resource/", "plane.obj");

	spriteData_[0].LeftTop[0] = {0.0f,0.0f,0.0f,1.0f};
	spriteData_[0].RightDown[0] = {60.0f,60.0f,0.0f,1.0f};
	spriteData_[0].LeftTop[1] = {0.0f,0.0f,0.0f,1.0f};
	spriteData_[0].RightDown[1] = {60.0f,60.0f,0.0f,1.0f};
	spriteData_[0].material = {1.0f,1.0f,1.0f,1.0f};

	spriteData_[1].LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[1].RightDown[0] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_[1].LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[1].RightDown[1] = { 1280.0f,720.0f,0.0f,1.0f };
	spriteData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	spriteTransform_[0] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	spriteTransform_[1] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{30.0f,0.0f,0.0f} };

	for (int i = 0; i < 3; i++) {
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

	enemySpornTimer = 0;
	enemyCount = 0;

	count = 0;
	isShot = false;

	particleSporn = false;
	particleCount = 0;

	sceneChangeTimer = 3000;
	boxTimer = 3000;
	scoar = 0;
	boxScoar = 0;

	isEnemyAttack = true;
	enemyCoolDown = 0;

	isChange = true;
}

void GamePlayScene::Update()
{
	input_->Update();

	player_->Update();
	skydome_->Update();

	if (isChange == true) {
		spriteData_[1].material.w -= 0.01f;
	}

	if (spriteData_[1].material.w <= 0.0f) {
		isChange = false;
	}

	if (isChange == false) {
		sceneChangeTimer--;
		PlayerAttack();
		EnemyAttack();
		EnemySporn();
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	for (Enemy* enemy : enemy_) {
		for (PlayerBullet* bullet : bullets_) {
			aabb1 = AABBadd(bullet->GetBullet().translate, { 1.0f,1.0f,1.0f });
			aabb2 = AABBadd(enemy->GetPosition().translate, { 1.0f,1.0f,1.0f });
			if (IsCollision(aabb1, aabb2)) {
				enemy->SetAlive(false);

				scoar += 100;
				boxScoar += 100;

				particle->Initialize(dxCommon_, engine_, "Resource/", "plane.obj");

				std::mt19937 random(generator());

				for (uint32_t index = 0; index < 10; ++index) {
					particles[index] = particle->MakeNewParticleSetPos(random, enemy->GetPosition());
				}
			}
		}
	}

	enemy_.remove_if([](Enemy* enemy) {
		if (enemy->GetAlive() == false || enemy->IsDead() == true) {
			delete enemy;
			return true;
		}
		return false;
		});

	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}

	eachTimer[1] = sceneChangeTimer / 100;
	boxTimer = boxTimer % 1;

	eachTimer[0] = sceneChangeTimer / 1000;
	boxTimer = boxTimer % 10;

	for (int i = 0; i < 2; i++) {
		eachTimer[i] %= 10;
	}

	if (sceneChangeTimer <= 0) {
		bullets_.remove_if([](PlayerBullet* bullet) {
			delete bullet;
			return true;
			});

		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			delete bullet;
			return true;
			});

		enemy_.remove_if([](Enemy* enemy) {

			delete enemy;
			return true;
			});

		sceneNo = CLEAR;
	}

}

void GamePlayScene::Draw()
{
	player_->Draw(cameraTransform_, directionalLight_);
	skydome_->Draw(cameraTransform_, directionalLight_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(cameraTransform_, directionalLight_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(cameraTransform_, directionalLight_);
	}

	for (Enemy* enemy : enemy_) {
		enemy->Draw(cameraTransform_, directionalLight_);
	}

	if (isChange == false) {
		for (int s = 0; s < 10; s++) {
			if (eachTimer[0] == s) {
				for (int i = 0; i < 1; i++)
				{
					sprite_[0]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[0], spriteData_[0].material, s + 6, directionalLight_);
				}
			}
			if (eachTimer[1] == s) {
				for (int i = 0; i < 1; i++)
				{
					sprite_[1]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[1], spriteData_[0].material, s + 6, directionalLight_);
				}
			}
		}
	}

	particle->Draw(&particles[0], 4, cameraTransform_);

	if (isChange == true) {
		for (int i = 0; i < 1; i++)
		{
			sprite_[2]->Draw(spriteData_[1].LeftTop[i], spriteData_[1].RightDown[i], spriteTransform_[0], spriteData_[1].material, 2, directionalLight_);
		}
	}

}

void GamePlayScene::EnemySporn() {

	if (enemyCount <= 2) {

		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(engine_, dxCommon_);

		newEnemy->SetAlive(true);

		enemy_.push_back(newEnemy);

		enemyCount++;
	}
	else if (enemyCount > 2) {
		enemySpornTimer++;
		if (enemySpornTimer >= 180) {
			enemyCount = 0;
			enemySpornTimer = 0;
		}
	}

}

void GamePlayScene::PlayerAttack() {
	if (input_->PushKey(DIK_SPACE)) {
		isShot = true;
	}
	else {
		isShot = false;
	}

	if (isShot == true && count == 0) {

		count++;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(engine_, dxCommon_);

		newBullet->SetBullet(player_->GetPosition());

		bullets_.push_back(newBullet);
	}
	else if (count != 0) {
		count++;
		if (count == 60) {
			count = 0;
		}
	}
}

void GamePlayScene::EnemyAttack() {

	for (Enemy* enemy : enemy_) {
		if (isEnemyAttack == true) {

			isEnemyAttack = false;

			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize(engine_, dxCommon_);

			newBullet->SetBullet(enemy->GetPosition());

			enemyBullets_.push_back(newBullet);
		}
		else if (isEnemyAttack == false) {
			enemyCoolDown++;
			if (enemyCoolDown >= 60) {
				enemyCoolDown = 0;
				isEnemyAttack = true;
			}
		}
	}

}

bool GamePlayScene::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.x >= aabb2.max.x && aabb1.max.x <= aabb2.min.x && //左右
		aabb1.min.y >= aabb2.max.y && aabb1.max.y <= aabb2.min.y &&
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)
	{
		return true;
	}
	return false;
}

AABB GamePlayScene::AABBadd(Vector3 a, Vector3 objectSize) {
	AABB aabb{};
	for (int i = 0; i < 100; i++) {
		aabb.min = { 1.0f * objectSize.x,1.0f * objectSize.y,-1.0f * objectSize.z };
		aabb.max = { -1.0f * objectSize.x,-1.0f * objectSize.y,1.0f * objectSize.z };

		aabb.min.x += a.x;
		aabb.min.y += a.y;
		aabb.min.z += a.z;

		aabb.max.x += a.x;
		aabb.max.y += a.y;
		aabb.max.z += a.z;

	}
	return aabb;
}

void GamePlayScene::Finalize()
{
	delete textureManager_;
	player_->Finalize();
	skydome_->Finalize();
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (int i = 0; i < 3; i++) {
		delete sprite_[i];
	}
	delete particle;
}