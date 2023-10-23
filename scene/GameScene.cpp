#include "GameScene.h"

void GameScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon)
{
	engine_ = engine;
	dxCommon_ = dxCommon;

	sound_ = new Sound();
	sound_->Initialize();

	input_ = new Input();
	input_->Initialize();

	soundDataHandle_ = sound_->LoadWave("Audio/Alarm01.wav");

}

void GameScene::Update() {

}

void GameScene::Draw()
{
	
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
	object_->Finalize();
	sound_->Finalize();
	sound_->UnLoad(&soundDataHandle_);

	delete sphere_;
	delete object_;
	delete sound_;
	delete input_;
}