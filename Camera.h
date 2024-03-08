#pragma once
#include "Vertex.h"
#include "MatrixCalculation.h"
#include "WinApp.h"

class Camera
{
public:
	
	void Initialize();

	void UpdateMatrix();

public:

	Transform GetTransform() { return transform_; }

private:
	WinApp* winApp_;

	// 垂直方向視野角
	float fovAngleY = 45.0f * 3.141592654f / 180.0f;
	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;

	Transform transform_;

	Matrix4x4 matView;
	Matrix4x4 matProjection;

	Matrix4x4 smatView;
	Matrix4x4 smatProjection;

	Matrix4x4 cameraMatrix;

};

