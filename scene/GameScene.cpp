#include "GameScene.h"

void GameScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	TDInitialize(dxCommon_, engine_);

	//scale
	//rotate
	//translate

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-20.0f} };

	soundDataHandle_ = sound_->LoadWave("Audio/Alarm01.wav");

	objectTransform_ = { {0.8f,0.5f,0.8f},{0.0f,0.0f,1.575f},{0.0f,-1.0f,0.0f} };
	objectMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	
	playerTransform_ = { {0.8f,0.5f,0.8f},{0.0f,0.0f,1.575f},{0.0f,1.8f,0.0f} };
	playerMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	objectDraw_ = true;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	texture_ = 0;
	uvResourceNum_ = 0;
	engine_->SettingTexture("Resource/uvChecker.png", uvResourceNum_);


}

void GameScene::Update() {

	input_->Update();

	MatrixUpdate();

	if (input_->PushKey(DIK_LEFT)) {
		objectTransform_.rotate.num[1] -= 0.05f;
	}

	

}

void GameScene::Draw()
{
	object_[0]->Draw(objectMaterial_, objectTransform_, texture_, cameraTransform_, directionalLight_);

	object_[1]->Draw(playerMaterial_, playerTransform_, texture_, cameraTransform_, directionalLight_);
}

void GameScene::TDInitialize(DirectXCommon* dxCommon, MyEngine* engine) {

	sound_ = new Sound();
	sound_->Initialize();

	input_ = new Input();
	input_->Initialize();

	for (int i = 0; i < 2; i++) {
		object_[i] = new Object();
		object_[i]->Initialize(dxCommon, engine);
	}

	for (int i = 0; i < 2; i++)
	{
		triangle_[i] = new Triangle();
		triangle_[i]->Initialize(dxCommon, engine);
	}

	for (int i = 0; i < 2; i++)
	{
		sprite_[i] = new Sprite();
		sprite_[i]->Initialize(dxCommon, engine);
	}

	sphere_ = new Sphere();
	sphere_->Initialize(dxCommon, engine);

}

void GameScene::MatrixUpdate() {
	Matrix4x4 sphereAffine = MakeAffineMatrix(sphereTransform_.scale, sphereTransform_.rotate, sphereTransform_.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(dxCommon_->GetWin()->kClientWidth) / float(dxCommon_->GetWin()->kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix_, Multiply(viewMatrix, projectionMatrix));

	worldMatrix_ = worldViewProjectionMatrix;
	sphereMatrix_ = Multiply(sphereAffine, Multiply(viewMatrix, projectionMatrix));

	directionalLight_.direction = Normalise(directionalLight_.direction);
}

void GameScene::Finalize()
{
	for (int i = 0; i < 2; i++)
	{
		//triangle_[i]->Finalize();
		delete triangle_[i];
	}

	for (int i = 0; i < 2; i++)
	{
		//sprite_[i]->Finalize();
		delete sprite_[i];
	}

	sphere_->Finalize();

	for (int i = 0; i < 2; i++)
	{
		object_[i]->Finalize();
		delete object_[i];
	}
	sound_->Finalize();
	sound_->UnLoad(&soundDataHandle_);

	delete sphere_;
	delete sound_;
	delete input_;
}