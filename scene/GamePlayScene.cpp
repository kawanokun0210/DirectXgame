#include "GamePlayScene.h"

void GamePlayScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}

void GamePlayScene::Update()
{
	
}

void GamePlayScene::Draw()
{

}

void GamePlayScene::Finalize()
{
	
}