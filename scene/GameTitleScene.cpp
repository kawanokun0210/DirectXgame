#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	//MT
	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f,0.0f,0.71f }, 3.141592f);
	interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	interpolate4 = Slerp(rotation0, rotation1, 1.0f);


}

void GameTitleScene::Update() {
	input_->Update();

	ImGui::Begin("MT4_01_05");
	ImGui::Text("interpolate0");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", interpolate0.x, interpolate0.y, interpolate0.z, interpolate0.w);
	ImGui::Text("interpolate1");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", interpolate1.x, interpolate1.y, interpolate1.z, interpolate1.w);
	ImGui::Text("interpolate2");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", interpolate2.x, interpolate2.y, interpolate2.z, interpolate2.w);
	ImGui::Text("interpolate3");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", interpolate3.x, interpolate3.y, interpolate3.z, interpolate3.w);
	ImGui::Text("interpolate4");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", interpolate4.x, interpolate4.y, interpolate4.z, interpolate4.w);
	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}