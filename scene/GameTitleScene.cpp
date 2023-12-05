#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	identity = IdentityQuaternion();
	conj = Conjugate(q1);
	inv = Inverse(q1);
	normal = Normalize(q1);
	mul1 = Multiply(q1, q2);
	mul2 = Multiply(q2, q1);
	norm = Norm(q1);

}

void GameTitleScene::Update() {
	input_->Update();

	ImGui::Begin("MT4_01_03");
	ImGui::Text("identity");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", identity.x, identity.y, identity.z, identity.w);
	ImGui::Text("conjugate");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", conj.x, conj.y, conj.z, conj.w);
	ImGui::Text("Inverse");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", inv.x, inv.y, inv.z, inv.w);
	ImGui::Text("normalize");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", normal.x, normal.y, normal.z, normal.w);
	ImGui::Text("mul1");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", mul1.x, mul1.y, mul1.z, mul1.w);
	ImGui::Text("mul2");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", mul2.x, mul2.y, mul2.z, mul2.w);
	ImGui::Text("norm");
	ImGui::Text("%4.2f", norm);
	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}