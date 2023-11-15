#pragma once
#include "BaseScene.h"
#include "Input.h"
#include "MatrixCalculation.h"

class GameTitleScene : public BaseScene 
{
public:

	void Initialize(MyEngine* engine, DirectXCommon* dxCommon) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	MyEngine* engine_;
	DirectXCommon* dxCommon_;
	Input* input_;

	int time;

	Vector3 axis;
	float angle;
	Matrix4x4 rotateMatrix;

};