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

	for (int i = 0; i < 2; i++) {
		objectTransform_[0] = { {1.0f * ObjectSize[0],1.0f * ObjectSize[0],1.0f * ObjectSize[0]},{0.0f,0.0f,0.0f},{0.0f,-1.0f,0.0f} };
		objectTransform_[1] = { {1.0f * ObjectSize[1],1.0f * ObjectSize[0],1.0f * ObjectSize[0]},{0.0f,0.0f,0.0f},{-5.5f,-3.8f,0.0f} };
		objectMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	playerTransform_ = { {1.0f * ObjectSize[2],1.0f * ObjectSize[2],1.0f * ObjectSize[2]},{0.0f,0.0f,0.0f},{-5.0f,-2.8f,0.0f} };
	playerMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	objectDraw_ = true;
	drop = true;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	texture_ = 0;
	uvResourceNum_ = 0;
	engine_->SettingTexture("Resource/uvChecker.png", uvResourceNum_);

	cubeResourceNum_ = 1;
	engine_->SettingTexture("Resource/cube.jpg", cubeResourceNum_);

}

void GameScene::Update() {

	input_->Update();

	MatrixUpdate();

	playerAcceleration_ = -0.02f;

	if (drop == true && isJump_ == false) {
		playerTransform_.translate.num[1] -= 0.01f;
	}

	if (input_->PushKey(DIK_LEFT)) {
		playerTransform_.translate.num[0] -= 0.1f;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		playerTransform_.translate.num[0] += 0.1f;
	}

	if (input_->TriggerKey(DIK_SPACE) && isJump_ == false) {
		isJump_ = true;
		playerSpeed_ = 0.38f;
	}

	if (isJump_ == true) {
		playerSpeed_ += playerAcceleration_;
		playerTransform_.translate.num[1] += playerSpeed_;
	}



	for (int i = 0; i < 2; i++) {
		aabb1 = AABBadd(playerTransform_.translate, ObjectSize[2], ObjectSize[2], ObjectSize[2]);
		aabb2 = AABBadd(objectTransform_[i].translate, ObjectSize[i], ObjectSize[0], ObjectSize[0]);
		if (IsCollision(aabb1, aabb2)) {
			isJump_ = false;
			drop = false;
			playerSpeed_ = 0;
			break;
		}
		else {
			isJump_ = true;
			drop = true;
		}
	}


}

void GameScene::Draw()
{
	object_[0]->Draw(objectMaterial_[0], objectTransform_[0], cubeResourceNum_, cameraTransform_, directionalLight_);
	object_[1]->Draw(playerMaterial_, playerTransform_, texture_, cameraTransform_, directionalLight_);
	object_[2]->Draw(objectMaterial_[1], objectTransform_[1], cubeResourceNum_, cameraTransform_, directionalLight_);
}

void GameScene::TDInitialize(DirectXCommon* dxCommon, MyEngine* engine) {

	sound_ = new Sound();
	sound_->Initialize();

	input_ = new Input();
	input_->Initialize();

	for (int i = 0; i < kMaxObject; i++) {
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

bool GameScene::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.num[0] >= aabb2.max.num[0] && aabb1.max.num[0] <= aabb2.min.num[0] && //左右
		aabb1.min.num[1] >= aabb2.max.num[1] && aabb1.max.num[1] <= aabb2.min.num[1] //上下
		) {
		return true;
	}
	return false;
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

	for (int i = 0; i < kMaxObject; i++)
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

AABB GameScene::AABBadd(Vector3 a, float sizeX, float sizeY, float sizeZ) {
	AABB aabb{};
	for (int i = 0; i < 2; i++) {
		aabb.min = { 1.0f * sizeX,1.0f * sizeY,-1.0f * sizeZ };
		aabb.max = { -1.0f * sizeX,-1.0f * sizeY,1.0f * sizeZ };

		aabb.min.num[0] += a.num[0];
		aabb.min.num[1] += a.num[1];
		aabb.min.num[2] += a.num[2];

		aabb.max.num[0] += a.num[0];
		aabb.max.num[1] += a.num[1];
		aabb.max.num[2] += a.num[2];

	}
	return aabb;
}