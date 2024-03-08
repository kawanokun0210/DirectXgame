#include "Camera.h"

void Camera::Initialize() {
	transform_ = { 
		{1.0f,1.0f,1.0f},//scale
		{0.0f,0.0f,0.0f},//rotate
		{0.0f,0.0f,-10.0f}//translate
	};
}

void Camera::UpdateMatrix() {

	cameraMatrix = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	matView = Inverse(cameraMatrix);

	smatView = MakeIdentity4x4();

	matProjection = MakePerspectiveFovMatrix(0.45f, float(winApp->kCilentWidth) / float(winApp->kCilentHeight), nearZ, farZ);

	smatProjection = MakeOrthographicmatrix(0.0f, 0.0f, float(winApp->kCilentWidth), float(winApp->kCilentHeight), 0.0f, 100.0f);
}