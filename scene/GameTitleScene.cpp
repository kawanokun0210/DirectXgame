#include "GameTitleScene.h"

void GameTitleScene::Initialize(MyEngine* engine, DirectXCommon* dxCommon) {
	dxCommon_ = dxCommon;
	engine_ = engine;
	input_ = Input::GetInstance();
	input_->Initialize();

	from0 = Normalise(Vector3{ 1.0f,0.7f,0.5f });
	to0 = { -from0.x,-from0.y,-from0.z };
	from1 = Normalise(Vector3{ -0.6f,0.9f,0.2f });
	to1 = Normalise({ 0.4f,0.7f,-0.5f });
	rotateMatrix0 = DirectionToDirection(Normalise({ 1.0f,0.0f,0.0f }), Normalise({ -1.0f,0.0f,0.0f }));
	rotateMatrix1 = DirectionToDirection(from0, to0);
	rotateMatrix2 = DirectionToDirection(from1, to1);

}

void GameTitleScene::Update() {
	input_->Update();

	ImGui::Begin("MT4_01_02");
	ImGui::Text("rotateMatrix0");
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix0.m[0][0], rotateMatrix0.m[0][1], rotateMatrix0.m[0][2], rotateMatrix0.m[0][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix0.m[1][0], rotateMatrix0.m[1][1], rotateMatrix0.m[1][2], rotateMatrix0.m[1][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix0.m[2][0], rotateMatrix0.m[2][1], rotateMatrix0.m[2][2], rotateMatrix0.m[2][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix0.m[3][0], rotateMatrix0.m[3][1], rotateMatrix0.m[3][2], rotateMatrix0.m[3][3]);

	ImGui::Text("rotateMatrix1");
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix1.m[0][0], rotateMatrix1.m[0][1], rotateMatrix1.m[0][2], rotateMatrix1.m[0][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix1.m[1][0], rotateMatrix1.m[1][1], rotateMatrix1.m[1][2], rotateMatrix1.m[1][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix1.m[2][0], rotateMatrix1.m[2][1], rotateMatrix1.m[2][2], rotateMatrix1.m[2][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix1.m[3][0], rotateMatrix1.m[3][1], rotateMatrix1.m[3][2], rotateMatrix1.m[3][3]);

	ImGui::Text("rotateMatrix2");
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix2.m[0][0], rotateMatrix2.m[0][1], rotateMatrix2.m[0][2], rotateMatrix2.m[0][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix2.m[1][0], rotateMatrix2.m[1][1], rotateMatrix2.m[1][2], rotateMatrix2.m[1][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix2.m[2][0], rotateMatrix2.m[2][1], rotateMatrix2.m[2][2], rotateMatrix2.m[2][3]);
	ImGui::Text("%4.3f %4.3f %4.3f %4.3f", rotateMatrix2.m[3][0], rotateMatrix2.m[3][1], rotateMatrix2.m[3][2], rotateMatrix2.m[3][3]);
	ImGui::End();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = PLAY;
	}
}

void GameTitleScene::Draw() {

}

void GameTitleScene::Finalize() {

}