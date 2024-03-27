#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	engine_->SettingTexture("Resource/uvChecker.png", 3);
	engine_->SettingTexture("Resource/fence/fence.png", 4);
	engine_->SettingTexture("Resource/circle.png", 5);
	engine_->SettingTexture("Resource/particle.png", 6);

	directionalLight_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight_.direction = { 0.0f,-1.0f,0.0f };
	directionalLight_.intensity = 1.0f;

	object_[0] = new Object();
	object_[0]->Initialize(dxCommon_, engine_, "Resource/fence", "fence.obj");

	object_[1] = new Object();
	object_[1]->Initialize(dxCommon_, engine_, "Resource/", "plane.obj");

	for (int i = 0; i < 2; i++) {
		objectTransform_[i] = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		objectMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	objectTransform_[1] = { {0.4f,0.4f,0.4f},{0.0f,0.0f,0.0f},{1.0f,-1.0f,0.0f} };

	particle_[0] = new Particle();
	particle_[0]->Initialize(dxCommon_, engine_, 1, "Resource/", "plane.obj");

	particle_[1] = new Particle();
	particle_[1]->Initialize(dxCommon_, engine_, 2, "Resource/", "plane.obj");

	PTrans = { { 0.4f,0.4f,0.4f }, { 0.0f, 0.0f, 0.0f }, { 1.0f,-1.0f,0.0f } };

	std::mt19937 randomEngine(seedGenerator());

	for (uint32_t index = 0; index < 10; ++index) {
		particles[index] = particle_[0]->MakeNewParticle(randomEngine);
		particles_[index] = particle_[1]->MakeNewParticleSetPos(randomEngine,PTrans);
	}

	camera_ = new Camera();
	camera_->Initialize(dxCommon_);

}

void GameTitleScene::Update() {
	input_->Update();



	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

	for (int i = 0; i < 2; i++) {
		object_[i]->Draw(objectMaterial_[i], objectTransform_[i], 3, camera_, directionalLight_, true);
	}

	particle_[0]->Draw(&particles[0], 5, camera_);
	particle_[1]->Draw(&particles_[0], 3, camera_);

}

void GameTitleScene::Finalize() {
	for (int i = 0; i < 2; i++) {
		object_[i]->Finalize();
	}
	delete object_[0];
	delete object_[1];

	delete particle_[0];
	delete particle_[1];

	delete camera_;
}