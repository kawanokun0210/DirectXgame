#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	engine_ = MyEngine::GetInstance();

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	monsterBallResourceNum_ = 1;
	engine_->SettingTexture("Resource/fence/fence.png", 3);

	engine_->SettingTexture("Resource/human/white.png", 4);

	engine_->SettingTexture("Resource/circle.png", 5);

	engine_->SettingTexture("Resource/rostock_laage_airport_4k.dds", 6);

	level = new LevelEditor();
	level->LoadJsonFile();

	camera_ = new Camera();
	camera_->Initialize();
	//cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

}

void GamePlayScene::Update()
{

	sphereMatrix_ = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	directionalLight_.direction = Normalise(directionalLight_.direction);

	camera_->Update();

}

void GamePlayScene::Draw()
{
	level->Draw(camera_,directionalLight_);
}

void GamePlayScene::Finalize()
{
	delete camera_;
	delete level;
}