#include "GamePlayScene.h"

void GamePlayScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	player_ = new Player();
	player_->Initialize(engine_,dxCommon_);
}

void GamePlayScene::Update()
{
	
}

void GamePlayScene::Draw()
{
	player_->Draw(cameraTransform_, directionalLight_);
}

void GamePlayScene::Finalize()
{
	player_->Finalize();
}