#include "GamePlayScene.h"

void GamePlayScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	sound_ = new Sound();
	sound_->Initialize();

	input_ = Input::GetInstance();
	input_->Initialize();

	soundDataHandle_ = sound_->LoadWave("Resource/Audio/Alarm01.wav");

	for (int i = 0; i < 2; i++)
	{
		transform_[i] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	}

	triangleDrawA_ = false;
	triangleDrawB_ = false;

	spriteData_[0].LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[0].RightDown[0] = { 320.0f,180.0f,0.0f,1.0f };
	spriteData_[0].LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[0].RightDown[1] = { 320.0f,180.0f,0.0f,1.0f };
	spriteData_[0].material = { 1.0f,1.0f,1.0f,1.0f };

	spriteData_[1].LeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[1].RightDown[0] = { 960.0f,410.0f,0.0f,1.0f };
	spriteData_[1].LeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_[1].RightDown[1] = { 960.0f,410.0f,0.0f,1.0f };
	spriteData_[1].material = { 1.0f,1.0f,1.0f,1.0f };

	spriteTransform_[0] = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
	spriteTransform_[1] = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	spriteDraw_ = false;

	sphereTransform_ = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	sphereDraw_ = false;

	objectDraw_ = false;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	engine_->SettingTexture("Resource/uvChecker.png", 2);

	monsterBallResourceNum_ = 1;
	engine_->SettingTexture("Resource/fence/fence.png", 3);

	engine_->SettingTexture("Resource/circle.png", 4);

	engine_->SettingTexture("Resource/particle.png", 5);


	for (int i = 0; i < 2; i++)
	{
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon_, engine_);
	}

	camera_ = new Camera();
	camera_->Initialize(dxCommon_);
	//cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

}

void GamePlayScene::Update()
{
	//XINPUT_STATE joyState;
	input_->Update();

	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);

	directionalLight_.direction = Normalise(directionalLight_.direction);

}

void GamePlayScene::Draw()
{
	for (int i = 0; i < 1; i++)
	{
		sprite_[0]->Draw(spriteData_[0].LeftTop[i], spriteData_[0].RightDown[i], spriteTransform_[i], spriteData_[0].material, 2, directionalLight_);
		sprite_[1]->Draw(spriteData_[1].LeftTop[i], spriteData_[1].RightDown[i], spriteTransform_[i], spriteData_[1].material, 3, directionalLight_);
	}
}

void GamePlayScene::Finalize()
{
	
	for (int i = 0; i < 2; i++)
	{
		//sprite_[i]->Finalize();
		delete sprite_[i];
	}

	delete camera_;
	delete sound_;

}