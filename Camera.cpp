#include "Camera.h"

void Camera::Initialize() {
	winApp_ = WinApp::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();

	transform_ = { 
		{1.0f,1.0f,1.0f},//scale
		{0.0f,0.0f,0.0f},//rotate
		{0.0f,0.0f,-10.0f}//translate
	};
}

void Camera::CreateBuffer() {
	constBuffer_ = dxCommon_->CreateBufferResource(dxCommon_->GetDevice(), sizeof(CameraData));

	constBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData_));
}

void Camera::TransferMatrix() {
	cameraData_->view = matView;
	cameraData_->projection = matProjection;

	cameraData_->sview = smatView;
	cameraData_->sprojection = smatProjection;
}

void Camera::UpdateMatrix() {

	cameraMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	matView = Inverse(cameraMatrix);

	smatView = MakeIdentity4x4();

	matProjection = MakePerspectiveFovMatrix(0.45f, float(winApp_->kClientWidth) / float(winApp_->kClientHeight), nearZ, farZ);

	smatProjection = MakeOrthographicmatrix(0.0f, 0.0f, float(winApp_->kClientWidth), float(winApp_->kClientHeight), 0.0f, 100.0f);
}