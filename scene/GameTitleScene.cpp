#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	axis = Normalise({ 1.0f,1.0f,1.0f });
	angle = 0.44f;
	rotateMatrix = MakeRotateAxisAngle(axis, angle);

}

void GameTitleScene::Update() {
	input_->Update();

	ImGui::Begin("MT4_01_01");
	ImGui::InputFloat4("Matrix0", &rotateMatrix.m[0][0]);
	ImGui::InputFloat4("Matrix1", &rotateMatrix.m[1][0]);
	ImGui::InputFloat4("Matrix2", &rotateMatrix.m[2][0]);
	ImGui::InputFloat4("Matrix3", &rotateMatrix.m[3][0]);
	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}