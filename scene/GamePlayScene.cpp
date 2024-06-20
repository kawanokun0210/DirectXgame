#include "GamePlayScene.h"

void GamePlayScene::Initialize()
{
	engine_ = MyEngine::GetInstance();

	sound_ = new Sound();
	sound_->Initialize();

	input_ = Input::GetInstance();
	input_->Initialize();

	soundDataHandle_ = sound_->LoadWave("Resource/Audio/Alarm01.wav");

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	monsterBallResourceNum_ = 1;
	engine_->SettingTexture("Resource/fence/fence.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);


	object_[0] = new Object();

	object_[0]->Initialize("Resource/fence", "fence.obj");

	object_[1] = new Object();

	object_[1]->Initialize("Resource/", "plane.obj");

	for (int i = 0; i < 2; i++) {
		objectTransform_[i] = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		objectMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	camera_ = new Camera();
	camera_->Initialize();

}

void GamePlayScene::Update()
{
	//XINPUT_STATE joyState;
	input_->Update();

	sphereMatrix_ = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	directionalLight_.direction = Normalise(directionalLight_.direction);

}

void GamePlayScene::Draw()
{

	for (int i = 0; i < 2; i++) {
		object_[i]->Draw(objectMaterial_[i], objectTransform_[i], 3, camera_, directionalLight_, true);
	}

}

void GamePlayScene::Finalize()
{

	for (int i = 0; i < 2; i++) {
		object_[i]->Finalize();
	}
	delete object_[0];
	delete object_[1];

	sound_->Finalize();
	sound_->UnLoad(&soundDataHandle_);

	delete sound_;

	delete camera_;
}