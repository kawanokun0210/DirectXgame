#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	time = 0;

	//MT
	rotation = MakeRotateAxisAngleQuaternion(Normalise(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f);
	Vector3 pointY = { 2.1f,-0.9f,1.3f };
	rotateMatrix = MakeRotateMatrix(rotation);
	rotateByQuaternion = RotateVector(pointY, rotation);
	rotateByMatrix = VectorTransform(pointY, rotateMatrix);

}

void GameTitleScene::Update() {
	input_->Update();

	ImGui::Begin("MT4_01_04");
	ImGui::Text("rotation");
	ImGui::Text("%4.2f %4.2f %4.2f %4.2f", rotation.x, rotation.y, rotation.z, rotation.w);
	ImGui::Text("rotateMatrix");
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
	ImGui::Text("rotateByQuaternion");
	ImGui::Text("%4.2f %4.2f %4.2f", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	ImGui::Text("rotateByMatrix");
	ImGui::Text("%4.2f %4.2f %4.2f", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);
	ImGui::End();

	time++;

	if (input_->TriggerKey(DIK_SPACE) || time == 60) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}