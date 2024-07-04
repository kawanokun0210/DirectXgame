#include "MapFloor.h"

void MapFloor::Initialize() {
	engine_ = MyEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	model_ = new Object();
	model_->Initialize("./Resource/", "block.obj");
}

void MapFloor::Update() {

}

void MapFloor::Draw(Camera* camera, DirectionalLight light) {
	model_->Draw(2, camera, light, true);
}

void MapFloor::Finalize() {
	delete model_;
}