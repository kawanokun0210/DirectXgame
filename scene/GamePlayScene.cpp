#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	engine_ = MyEngine::GetInstance();

	sound_ = new Sound();
	sound_->Initialize();

	input_ = Input::GetInstance();
	input_->Initialize();

	player_ = std::make_unique<Player>();
	player_->Initialize();

	soundDataHandle_ = sound_->LoadWave("Resource/Audio/Alarm01.wav");

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	monsterBallResourceNum_ = 1;
	engine_->SettingTexture("Resource/fence/fence.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);

	camera_ = new Camera();
	camera_->Initialize();

	block_ = Block::GetInstance();
	block_->Initialize();
	block_->LoadStage();
	block_->FindALLObject();

}

void GamePlayScene::Update()
{
	//XINPUT_STATE joyState;
	input_->Update();

	player_->Update();

	sphereMatrix_ = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	directionalLight_.direction = Normalise(directionalLight_.direction);

}

void GamePlayScene::Draw()
{
	player_->Draw(camera_, directionalLight_);
	block_->Draw(camera_, directionalLight_);
}

void GamePlayScene::Finalize()
{
	sound_->Finalize();
	sound_->UnLoad(&soundDataHandle_);
	
	block_->Finalize();

	delete sound_;

	delete camera_;
}